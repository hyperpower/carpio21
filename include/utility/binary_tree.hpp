/*
 * BinaryTree.h
 *
 *  Created on: Dec 22, 2014
 *      Author: zhou
 */

#ifndef _BINARYTREE_HPP_
#define _BINARYTREE_HPP_

#include "type_define.h"
// #include "Iterator.h"

namespace carpio {

template<typename TYPE>
class BinaryTreeNode{
protected:
    typedef BinaryTreeNode<TYPE>* pNode;
public:
    TYPE  value;
    pNode lchild;
    pNode rchild;
    pNode father;
    
    BinaryTreeNode():
            lchild(nullptr), rchild(nullptr), father(nullptr){
        value = TYPE();
    }
    
    BinaryTreeNode(const TYPE &data): 
        value(data), 
        lchild(nullptr), rchild(nullptr), father(nullptr){}
    
    BinaryTreeNode(const TYPE &data, pNode lc, pNode rc, pNode fc):
        value(data), 
        lchild(lc), rchild(rc), father(fc){}

    pNode leftmost(){
        pNode ptn = this;
        while (ptn->lchild != nullptr)
            ptn = ptn->lchild;
        return ptn;
    }
    pNode rightmost(){
        pNode ptn = this;
        while (ptn->rchild != nullptr)
            ptn = ptn->rchild;
        return ptn;
    }

    St height() const{
        return 1 + std::max(height(lchild), height(rchild));
    }
    St height(pNode cur) const{
        if (cur == nullptr)
            return 0;
        else
            return 1 + MAX(height(cur->lchild), height(cur->rchild));
    }
};


//This is the end of Class BinaryTreeNode
//-----------------------------------------------

template<class _Tp, class _Ref, class _Ptr>
class _BinaryTree_iterator {
public:
    typedef St size_type;
    typedef St difference_type;
    typedef bidirectional_iterator_tag iterator_category;

    typedef _BinaryTree_iterator<_Tp, _Tp&, _Tp*> iterator;
    typedef _BinaryTree_iterator<_Tp, const _Tp&, const _Tp*> const_iterator;
    typedef _BinaryTree_iterator<_Tp, _Ref, _Ptr> _Self;

    typedef _Tp  value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;
    typedef BinaryTreeNode<_Tp> _Node;

    BinaryTreeNode<_Tp>* _ptr;

    _BinaryTree_iterator() {
        _ptr = nullptr;
    }
    _BinaryTree_iterator(BinaryTreeNode<_Tp>* _x) {
        this->_ptr = _x;
    }
    _BinaryTree_iterator(const iterator& _x) {
        this->_ptr = _x._ptr;
    }

    void _incr() {
        BinaryTreeNode<_Tp>* fn;
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
        BinaryTreeNode<_Tp>* fn;
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

    bool operator==(const _BinaryTree_iterator& _x) const {
        return _ptr == _x._ptr;
    }
    bool operator!=(const _BinaryTree_iterator& _x) const {
        return _ptr != _x._ptr;
    }

    reference operator*() const {
        return ((_Node*) _ptr)->value;
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

    bool isExist(){
        return _ptr!=nullptr;
    }
};

//===============================================
template<typename TYPE>
class BinaryTree {
public:
    typedef TYPE& reference;
    typedef const TYPE& const_reference;
    typedef TYPE* pointer;
    typedef const TYPE* const_pointer;
    typedef _BinaryTree_iterator<TYPE, TYPE&, TYPE*> iterator;
    typedef _BinaryTree_iterator<TYPE, const TYPE&, const TYPE*> const_iterator;

    typedef St size_type;
    typedef St difference_type;
protected:
    typedef BinaryTreeNode<TYPE>          Node;
    typedef BinaryTreeNode<TYPE>*        pNode;
    typedef const BinaryTreeNode<TYPE>* cpNode;
    typedef void (*pFun_BinaryTree)(pNode, utPointer);

    pNode _root;

    void _preorder(pNode, pFun_BinaryTree, utPointer);
    void _inorder(pNode, pFun_BinaryTree, utPointer);
    void _postorder(pNode, pFun_BinaryTree, utPointer);

    void _destory(pNode&);
    void _copy(pNode&, const pNode&);
    void _reverse(pNode);
    size_type _height(pNode) const;
    size_type _size(const pNode&) const;

    iterator root();
    const_iterator root() const;
public:
    //constructor================================
    BinaryTree();
    BinaryTree(const BinaryTree<TYPE>&);
    //destructor ================================
    ~BinaryTree();
    //operator= =================================
    BinaryTree<TYPE>& operator=(const BinaryTree<TYPE>&);
    //Traversal =================================
    void pre_order(pFun_BinaryTree, utPointer);
    void in_order(pFun_BinaryTree, utPointer);
    void post_order(pFun_BinaryTree, utPointer);
    //iterator===================================
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    //===========================================

    bool empty() const;
    size_type size() const;
    void reverse();
    void clear();
    size_type height() const;
};

template<typename TYPE>
BinaryTree<TYPE>::BinaryTree() {
    _root = new Node();
}

template<typename TYPE>
BinaryTree<TYPE>::BinaryTree(const BinaryTree<TYPE>& a) {
    this->_root = nullptr;
    _copy(this->_root, a._root);
}

template<typename TYPE>
BinaryTree<TYPE>::~BinaryTree() {
    _destory(_root);
}

template<typename TYPE>
BinaryTree<TYPE>& BinaryTree<TYPE>::operator=(
        const BinaryTree<TYPE>& original) {
    _destory(this->_root);
    this->_root=nullptr;
    _copy(this->_root, original._root);
    return *this;
}

template<typename TYPE>
void BinaryTree<TYPE>::_destory(pNode& Current) {
    if (Current != nullptr) {
        _destory(Current->lchild);
        _destory(Current->rchild);
        delete Current;
        Current = nullptr;
    }
}

template<class TYPE>
void BinaryTree<TYPE>::_copy(pNode& Current, const pNode& original) {
    if (Current == nullptr) {
        Current = new Node(original->value);
    }
    if (original->lchild != nullptr) {
        Current->lchild = new Node(original->lchild->value);
        Current->lchild->father = Current;
        _copy(Current->lchild, original->lchild);
    }
    if (original->rchild != nullptr) {
        Current->rchild = new Node(original->rchild->value);
        Current->rchild->father = Current;
        _copy(Current->rchild, original->rchild);
    }
}

template<typename TYPE>
void BinaryTree<TYPE>::_preorder(pNode Current, pFun_BinaryTree visit,
        utPointer utp) {
    if (Current != nullptr) {
        (*visit)(Current->value, utp);
        _preorder(Current->lchild, visit, utp);
        _preorder(Current->rchild, visit, utp);
    }
}

template<class TYPE>
void BinaryTree<TYPE>::pre_order(pFun_BinaryTree visit, utPointer utp) {
    _preorder(_root->lchild, visit, utp);
}

template<typename TYPE>
void BinaryTree<TYPE>::_postorder(pNode Current, pFun_BinaryTree visit,
        utPointer utp) {
    if (Current != nullptr) {
        _postorder(Current->lchild, visit, utp);
        _postorder(Current->rchild, visit, utp);
        (*visit)(Current->value, utp);
    }
}

template<class TYPE>
void BinaryTree<TYPE>::post_order(pFun_BinaryTree visit, utPointer utp) {
    _postorder(_root->lchild, visit, utp);
}

template<typename TYPE>
void BinaryTree<TYPE>::_inorder(pNode Current, pFun_BinaryTree visit,
        utPointer utp) {
    if (Current != nullptr) {
        _inorder(Current->lchild, visit, utp);
        (*visit)(Current->value, utp);
        _inorder(Current->rchild, visit, utp);
    }
}

template<class TYPE>
void BinaryTree<TYPE>::in_order(pFun_BinaryTree visit, utPointer utp) {
    _inorder(_root->lchild, visit, utp);
}

template<class TYPE>
_BinaryTree_iterator<TYPE, const TYPE&, const TYPE*> BinaryTree<TYPE>::begin() const {
    Node *pnode = _root->lchild;
    if (pnode == nullptr) {
        return _root;
    }
    if (pnode->lchild) {
        while (pnode->lchild)
            pnode = pnode->lchild;
    }
    return pnode;
}

template<class TYPE>
_BinaryTree_iterator<TYPE, TYPE&, TYPE*> BinaryTree<TYPE>::begin() {
    Node *pnode = _root->lchild;
    if (pnode == nullptr) {
        return _root;
    }
    if (pnode->lchild) {
        while (pnode->lchild)
            pnode = pnode->lchild;
    }
    return pnode;
}
template<class TYPE>
_BinaryTree_iterator<TYPE, TYPE&, TYPE*> BinaryTree<TYPE>::root() {
    return _root->lchild;
}
template<class TYPE>
_BinaryTree_iterator<TYPE, const TYPE&, const TYPE*> BinaryTree<TYPE>::root() const {
    return _root->lchild;
}

template<class TYPE>
_BinaryTree_iterator<TYPE, const TYPE&, const TYPE*> BinaryTree<TYPE>::end() const {
    return _root;
}

template<class TYPE>
_BinaryTree_iterator<TYPE, TYPE&, TYPE*> BinaryTree<TYPE>::end() {
    return _root;
}



template<typename TYPE>
bool BinaryTree<TYPE>::empty() const {
    return _root->lchild == nullptr;
}

template<typename TYPE>
LarusDef::size_type BinaryTree<TYPE>::_size(const pNode& Current) const {
    if (Current == nullptr) {
        return 0;
    } else {
        return _size(Current->lchild) + _size(Current->rchild) + 1;
    }
}

template<typename TYPE>
LarusDef::size_type BinaryTree<TYPE>::size() const {
    return _size(_root->lchild);
}

template<class TYPE>
void BinaryTree<TYPE>::reverse() {
    _reverse(_root->lchild);
}

template<class TYPE>
void BinaryTree<TYPE>::_reverse(pNode Current) {
    if (Current != nullptr) {
        pNode temp = Current->lchild;
        Current->lchild = Current->rchild;
        Current->rchild = temp;
        _reverse(Current->lchild);
        _reverse(Current->rchild);
    }
}

template<class TYPE>
void BinaryTree<TYPE>::clear() {
    _destory(_root->lchild);
    //_root = nullptr;
}

template<class TYPE>
int BinaryTree<TYPE>::_height(pNode Current) const {
    if (Current == nullptr)
        return 0;
    else
        return 1 + MAX(_height(Current->lchild), _height(Current->rchild));
}

template<class TYPE>
LarusDef::size_type BinaryTree<TYPE>::height() const {
    return _height(_root->lchild);
}

}

#endif /* UTILITY_BINARYTREE_H_ */