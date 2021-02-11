#ifndef _BBTREE_HPP_
#define _BBTEEE_HPP_

#include "geometry/geometry_define.hpp"
#include "bbnode.hpp"
#include <functional>
#include <set>
#include <list>
#include <iostream>

namespace carpio {

template<class _Tp, class _Ref, class _Ptr>
class _BinaryTree_iterator_ {
public:
	typedef St size_type;
	typedef St difference_type;
	//typedef bidirectional_iterator_tag iterator_category;

	//typedef _BinaryTree_iterator_<_Tp, _Tp&, _Tp*> iterator;
	//typedef _BinaryTree_iterator_<_Tp, const _Tp&, const _Tp*> const_iterator;
	typedef _BinaryTree_iterator_<_Tp, _Ref, _Ptr> _Self;

	typedef _Tp value_type;
	typedef _Ptr pointer;
	typedef _Ref reference;

	_Tp* _ptr;

	_BinaryTree_iterator_() {
		_ptr = nullptr;
	}
	_BinaryTree_iterator_(_Tp* _x) {
		this->_ptr = _x;
	}
	_BinaryTree_iterator_(const _Self& _x) {
		this->_ptr = _x._ptr;
	}

	void _incr() {
		_Tp* fn;
		if (_ptr != nullptr) {
			if (_ptr->rchild != nullptr) {
				_ptr = _ptr->rchild->leftmost();
				return;
			}
			fn = _ptr->father;
			while (fn && _ptr == fn->rchild) {
				_ptr = fn;
				fn = fn->father;
			}
			_ptr = fn;
		}
	}

	void _decr() {
		_Tp* fn;
		if (_ptr != NULL) {
			if (_ptr->lchild) {
				_ptr = _ptr->lchild->rightmost();
				return;
			}
			fn = _ptr->father;
			while (fn && _ptr == fn->lchild) {
				_ptr = fn;
				fn = _ptr->father;
			}
		}
	}

	bool operator==(const _Self& _x) const {
		return _ptr == _x._ptr;
	}
	bool operator!=(const _Self& _x) const {
		return _ptr != _x._ptr;
	}

	reference operator*() const {
		return (*_ptr);
	}

	pointer operator->() const {
		return &(operator*());
	}

	_Self& operator++() {
		this->_incr();
		return *this;
	}

	_Self operator++(int) {
		_Self __tmp = *this;
		this->_incr();
		return __tmp;
	}

	_Self& operator--() {
		this->_decr();
		return *this;
	}

	_Self operator--(int) {
		_Self __tmp = *this;
		this->_decr();
		return __tmp;
	}

	bool isExist() {
		return _ptr != nullptr;
	}
};

//===============================================
template<typename BOX>
class BBTree_ {
public:
	typedef BBNode_<BOX> Node;
	typedef BBNode_<BOX>* pNode;
	typedef const BBNode_<BOX>* const_pNode;
	typedef Node& reference;
	typedef const Node& const_reference;
	typedef Node* pointer;
	typedef const Node* const_pointer;
	typedef _BinaryTree_iterator_<Node, Node&, Node*> iterator;
	typedef _BinaryTree_iterator_<Node, const Node&, const Node*> const_iterator;

	typedef St difference_type;
	typedef St size_type;

	typedef std::function<void(pNode)> Fun;
	typedef std::function<void(pNode, bool&)> Fun_flag;
	typedef std::function<void(const_pNode, bool&)> Func_flag;

	typedef void (*pFun_BBTree)(pNode, utPointer);
	typedef void (*pFun_BBTree_flag)(Int&, pNode, utPointer);
protected:
	pNode _root;
public:
	iterator root() {
		return _root->lchild;
	}
	const_iterator root() const {
		return _root->lchild;
	}
	/****************************************************************
	 *  constructor
	 ****************************************************************/
	BBTree_(){
		_root = nullptr;
	}
	BBTree_(const BBTree_<BOX>& a){
		this->_root = nullptr;
		_copy(this->_root, a._root);
	}
	template<class CONTAINER>
	BBTree_(const CONTAINER& set_box){
		_root = nullptr;
		std::list<pNode> lc;
		for (auto iter = set_box.begin(); iter != set_box.end(); ++iter) {
			lc.push_back(new BBNode_<BOX>((*iter)));
		}
		std::list<pNode> lf;
		Int flag = 1;
		std::list<pNode>* lcr = &lc;
		std::list<pNode>* lfr = &lf;
		//int idx = 0;
		while (!lcr->empty()) {
			bottomup_build((*lcr), (*lfr), flag);
			std::list<pNode>* tmpr = lcr;
			lcr = lfr;
			lfr = tmpr;
		}
	}


	/****************************************************************
	 *  destructor
	 ****************************************************************/
	~BBTree_(){
		_destory(_root);
	}
	/****************************************************************
	 *  operator
	 ****************************************************************/
	BBTree_<BOX>& operator=(const BBTree_<BOX>& o){
		_destory(this->_root);
		this->_root = nullptr;
		_copy(this->_root, o._root);
		return *this;
	}
	/****************************************************************
	 *  traversal
	 ****************************************************************/
	void PreOrder_flag(Int&, pFun_BBTree_flag, utPointer);
	void PreOrder_flag(Fun_flag);
	void PreOrder_flag(Func_flag) const;
	void PreOrder(pFun_BBTree visit, utPointer utp){
		_preorder(_root, visit, utp);
	}
	void PreOrder(Fun fun){
		_preorder(_root, fun);
	}
	void InOrder(pFun_BBTree, utPointer);
	void InOrder(Fun);
	void InOrder(Fun) const;
	void PostOrder(pFun_BBTree, utPointer);
	/****************************************************************
	 *  iterator
	 ****************************************************************/
	iterator begin() {
		pNode pnode = _root->lchild;
		if (pnode == nullptr) {
			return _root;
		}
		if (pnode->lchild) {
			while (pnode->lchild)
				pnode = pnode->lchild;
		}
		return pnode;
	}
	const_iterator begin() const {
		pNode pnode = _root->lchild;
		if (pnode == nullptr) {
			return _root;
		}
		if (pnode->lchild) {
			while (pnode->lchild)
				pnode = pnode->lchild;
		}
		return pnode;
	}
	iterator end() {
		return _root;
	}
	const_iterator end() const {
		return _root;
	}
	/****************************************************************
	 *  other
	 ****************************************************************/
	bool empty() const {
		return _root->lchild == nullptr;
	}
	size_type size() const {
		return 0 + _size(_root->lchild);
	}
	void reverse() {
		_reverse(_root->lchild);
	}
	void clear() {
		_destory(_root->lchild);
	}
	size_type height() const {
		return 0 + _height(_root->lchild);
	}
	/****************************************************************
	 *  builder
	 ****************************************************************/
	void bottomup_build(std::list<pNode>& lc, std::list<pNode>& lf, Int& flag);
	// show ======================================
protected:
	void _printtree(BBNode_<BOX> *cur, int layer);
	void _preorder(pNode cur, pFun_BBTree visit, utPointer utp){
		if (cur != nullptr) {
			(*visit)(cur->m_value, utp);
			_preorder(cur->lchild, visit, utp);
			_preorder(cur->rchild, visit, utp);
		}
	}

	void _preorder(pNode cur, Fun fun) {
		if (cur != nullptr) {
			fun(cur);
			_preorder(cur->lchild, fun);
			_preorder(cur->rchild, fun);
		}
	}
	void _preorder_flag(Int&, pNode, pFun_BBTree_flag, utPointer);

	void _preorder_flag(bool&, pNode, Fun_flag);
	void _preorder_flag(bool&, const_pNode, Func_flag) const;

	void _inorder(pNode, pFun_BBTree, utPointer);
	void _inorder(pNode, Fun);
	void _inorder(pNode, Fun) const;
	void _postorder(pNode, pFun_BBTree, utPointer);

	void _destory(pNode& cur) {
		if (cur != nullptr) {
			_destory(cur->lchild);
			_destory(cur->rchild);
			delete cur;
			cur = nullptr;
		}
	}
	void _copy(pNode& cur, const pNode& original) {
		if (cur == nullptr) {
			cur = new Node(original->m_value);
		}
		if (original->lchild != nullptr) {
			cur->lchild = new Node(original->lchild->m_value);
			cur->lchild->father = cur;
			_copy(cur->lchild, original->lchild);
		}
		if (original->rchild != nullptr) {
			cur->rchild = new Node(original->rchild->m_value);
			cur->rchild->father = cur;
			_copy(cur->rchild, original->rchild);
		}
	}
	void _reverse(pNode cur) {
		if (cur != nullptr) {
			pNode temp = cur->lchild;
			cur->lchild = cur->rchild;
			cur->rchild = temp;
			_reverse(cur->lchild);
			_reverse(cur->rchild);
		}
	}
	size_type _height(pNode cur) const {
		if (cur == nullptr)
			return 0;
		else
			return 1 + std::max(_height(cur->lchild), _height(cur->rchild));
	}
	size_type _size(const pNode& cur) const {
		if (cur == nullptr) {
			return 0;
		} else {
			return _size(cur->lchild) + _size(cur->rchild) + 1;
		}
	}


public:
	void print_tree();
	void output_vtk_height(const std::string& fn, Int hei);
};


template<typename BOX>
void BBTree_<BOX>::_preorder_flag(Int& flag, pNode cur, pFun_BBTree_flag visit,
		utPointer utp) {
	if (cur != nullptr) {
		(*visit)(flag, cur, utp);
		if (flag == -1) { //-------------------------
			return;
		}
		_preorder_flag(flag, cur->lchild, visit, utp);
		_preorder_flag(flag, cur->rchild, visit, utp);
	}
}

template<typename BOX>
void BBTree_<BOX>::_preorder_flag(bool& flag, pNode cur, Fun_flag visit) {
	if (cur != nullptr) {
		visit(cur, flag);
		if (!flag) { //-------------------------
			return;
		}
		_preorder_flag(flag, cur->lchild, visit);
		_preorder_flag(flag, cur->rchild, visit);
	}
}

template<typename BOX>
void BBTree_<BOX>::_preorder_flag(bool& flag, const_pNode cur,
		Func_flag visit) const {
	if (cur != nullptr) {
		visit(cur, flag);
		if (!flag) { //-------------------------
			return;
		}
		_preorder_flag(flag, cur->lchild, visit);
		_preorder_flag(flag, cur->rchild, visit);
	}
}

template<class BOX>
void BBTree_<BOX>::PreOrder_flag(Int& flag, pFun_BBTree_flag visit,
		utPointer utp) {
	_preorder_flag(flag, _root->lchild , visit, utp);
}

template<class BOX>
void BBTree_<BOX>::PreOrder_flag(Fun_flag visit) {
	bool flag = true;
	_preorder_flag(flag, _root->lchild , visit);
}

template<class BOX>
void BBTree_<BOX>::PreOrder_flag(Func_flag visit) const {
	bool flag = true;
	_preorder_flag(flag, _root->lchild , visit);
}

template<typename BOX>
void BBTree_<BOX>::_postorder(pNode cur, pFun_BBTree visit, utPointer utp) {
	if (cur != nullptr) {
		_postorder(cur->lchild, visit, utp);
		_postorder(cur->rchild, visit, utp);
		(*visit)(cur->m_value, utp);
	}
}

template<class BOX>
void BBTree_<BOX>::PostOrder(pFun_BBTree visit, utPointer utp) {
	_postorder(_root, visit, utp);
}

template<typename BOX>
void BBTree_<BOX>::_inorder(pNode cur, pFun_BBTree visit, utPointer utp) {
	if (cur != nullptr) {
		_inorder(cur->lchild, visit, utp);
		(*visit)(cur, utp);
		_inorder(cur->rchild, visit, utp);
	}
}

template<typename BOX>
void BBTree_<BOX>::_inorder(pNode cur, Fun visit) {
	if (cur != nullptr) {
		_inorder(cur->lchild, visit);
		visit(cur);
		_inorder(cur->rchild, visit);
	}
}
template<typename BOX>
void BBTree_<BOX>::_inorder(pNode cur, Fun visit) const {
	if (cur != nullptr) {
		_inorder(cur->lchild, visit);
		visit(cur);
		_inorder(cur->rchild, visit);
	}
}

template<class BOX>
void BBTree_<BOX>::InOrder(pFun_BBTree visit, utPointer utp) {
	_inorder(_root->lchild , visit, utp);
}

template<class BOX>
void BBTree_<BOX>::InOrder(Fun visit) {
	_inorder(_root->lchild , visit);
}

template<class BOX>
void BBTree_<BOX>::InOrder(Fun visit) const {
	_inorder(_root->lchild , visit);
}

template<typename BOX>
void BBTree_<BOX>::_printtree(BBNode_<BOX> *cur, int layer) {
	int i;
	if (cur == NULL) {
		return;
	}
	_printtree(cur->rchild, layer + 1);
	for (i = 0; i < layer; i++) {
		if (layer == 1) {
			std::cout << "  R>";
		} else {
			std::cout << "   ";
		}
	}
	std::cout << "O" << '\n';
	_printtree(cur->lchild, layer + 1);
}

template<typename BOX>
void BBTree_<BOX>::print_tree() {
	_return_if_fail(_root != nullptr);
	_printtree(this->_root, 1);
}



template<class BOX>
void BBTree_<BOX>::bottomup_build(std::list<pNode>& lc, std::list<pNode>& lf, Int& flag) {
	lf.clear();
	if (lc.size() == 1) {
		_root = new BBNode_<BOX>();
		_root->lchild = (*(lc.begin()));
		_root->lchild->father = _root;
		return;
	}
	if (flag == 1) {
		for (auto iter = lc.begin(); iter != lc.end(); ++iter, ++iter) {
			auto iterp = iter;
			iterp++;
			if (iterp != lc.end()) {
				BOX _box((*iter)->_box, (*iterp)->_box);
				BBNode_<BOX>* fnode = new BBNode_<BOX>(_box, *iter, *iterp,
						nullptr);
				lf.push_back(fnode);
				(*iter)->father = fnode;
				(*iterp)->father = fnode;
			} else {
				flag = 0;
				lf.push_back((*iter));
				return;
			}
		}
	} else {
		for (auto iter  = lc.rbegin();
				  iter != lc.rend(); ++iter, ++iter) {
			auto iterp = iter;
			iterp++;
			if (iterp != lc.rend()) {
				BOX _box((*iter)->_box, (*iterp)->_box);
				BBNode_<BOX>* fnode = new BBNode_<BOX>(_box, *iter, *iterp,
						nullptr);
				lf.push_back(fnode);
				(*iter)->father = fnode;
				(*iterp)->father = fnode;
			} else {
				flag = 1;
				lf.push_back((*iter));
				return;
			}
		}
	}
}

}

#endif
