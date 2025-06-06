/*
 * BinaryTree.h
 *
 *  Created on: Dec 22, 2014
 *      Author: zhou
 */

#ifndef _BINARYTREE_HPP_
#define _BINARYTREE_HPP_

#include <algorithm>
#include <functional>
#include "type_define.hpp"
// #include "Iterator.h"

namespace carpio {

template<class TYPE, std::size_t S>
class TreeNode_{
};

template<typename TYPE>
class TreeNode_<TYPE, 2>{
public:
    typedef TreeNode_<TYPE, 2>          Node;
    typedef TreeNode_<TYPE, 2>*        pNode;
    typedef const TreeNode_<TYPE, 2>* cpNode;
    typedef TYPE value_type;
    typedef std::size_t size_type;

    typedef TreeNode_<TYPE, 2> Self;

    static const int _LEFT_  = 0;
    static const int _RIGHT_ = 1;

public:
    TYPE  value;

    pNode father;
    pNode left_child;
    pNode right_child;

    TreeNode_():
        father(nullptr), left_child(nullptr), right_child(nullptr){}
    
    TreeNode_(const TYPE &data):
        value(data), father(nullptr), left_child(nullptr), right_child(nullptr){} 
    
    TreeNode_(const TYPE &data, pNode lc, pNode rc, pNode fc):
        value(data){
        this->left_child  = lc;
        this->right_child = rc;
        this->father      = fc;
    }

    ~TreeNode_(){}


    pNode child(int idx){
        switch (idx)
        {
        case 0:
            return this->left_child; 
            break;
        case 1:
            return this->right_child; 
            break;
        default:
            return nullptr;   
            break;
        }
    }
    cpNode child(int idx) const{
        switch (idx)
        {
        case 0:
            return this->left_child; 
            break;
        case 1:
            return this->right_child; 
            break;
        default:
            return nullptr;
            break;
        }
    }
    pNode sibling(){
        if(this->father != nullptr){
            if(this->is_left_child()){
                return this->father->right_child;
            }else if(this->is_right_child()){
                return this->father->left_child;
            }
        }
        return nullptr;
    }
    cpNode sibling() const{
        if(this->father != nullptr){
            if(this->is_left_child()){
                return this->father->right_child;
            }else if(this->is_right_child()){
                return this->father->left_child;
            }
        }
        return nullptr;
    }
    pNode left_most(){
        pNode cur = this;
        while (cur->left_child != nullptr)
            cur = cur->left_child;
        return cur;
    }
    pNode right_most(){
        pNode cur = this;
        while (cur->right_child != nullptr)
            cur = cur->right_child;
        return cur;
    }
    cpNode left_most() const{
        cpNode cur = this;
        while (cur->left_child != nullptr)
            cur = cur->left_child;
        return cur;
    }
    cpNode right_most() const{
        cpNode cur = this;
        while (cur->right_child != nullptr)
            cur = cur->right_child;
        return cur;
    }
    size_type height() const{
        return 1 + std::max<size_type>(this->height(this->left_child), this->height(this->right_child));
    }

    size_type height(cpNode cur) const{
        if (cur == nullptr)
            return 0;
        else
            return 1 + std::max<size_type>(height(cur->left_child), height(cur->right_child));
    }

    void destory(pNode& cur) {
        if (cur != nullptr) {
            destory(cur->left_child);
            destory(cur->right_child);
            delete cur;
            cur = nullptr;
        }
    }
    
    template <typename Callable, typename... Args>
    void _pre_order(pNode cur, Callable &&op, Args &&... args){
	    if (cur != nullptr) {
	    	std::invoke(op, cur, args...);
	    	_pre_order(cur->left_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	_pre_order(cur->right_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    }
    }

    template <typename Callable, typename... Args>
    void pre_order(Callable &&op, Args &&... args){
        this->_pre_order(this, std::forward<Callable>(op), std::forward<Args>(args)...);
    }

    pNode copy() const{
        pNode dst = new Node(this->value);
        copy(this, dst);
        return dst;
    }
    
    void copy(cpNode src, pNode dst) const{
        ASSERT(src != nullptr);
        ASSERT(dst != nullptr);

        dst->value = src->value;
       
        if (src->left_child != nullptr){
            dst->left_child = new Node(src->left_child->value);
            dst->left_child->father = dst;
            copy(src->left_child, dst->left_child);
        }
        if (src->right_child != nullptr){
            dst->right_child = new Node(src->right_child->value);
            dst->right_child->father = dst;
            copy(src->right_child, dst->right_child);
        }
    }

    bool is_leaf() const {
		_RETURN_VAL_IF_FAIL(this->left_child == nullptr, false);
		_RETURN_VAL_IF_FAIL(this->right_child == nullptr, false);
		return true;
	}
    bool has_child() const{
        return !(this->is_leaf());
    }
    bool has_children() const{
        return this->left_child != nullptr 
                && this->right_child != nullptr;
    }
    bool has_child(int d) const{
        if(d == _LEFT_){
		    _RETURN_VAL_IF_FAIL(this->left_child == nullptr, true);
        }
        if(d == _RIGHT_){
		    _RETURN_VAL_IF_FAIL(this->right_child == nullptr, true);
        }
        return false;
    }
    bool is_left_child() const {
		_RETURN_VAL_IF_FAIL(this->father != nullptr, false);
        return this->father->left_child == this;
    }
    bool is_right_child() const {
		_RETURN_VAL_IF_FAIL(this->father != nullptr, false);
        return this->father->right_child == this;
    }
    bool is_child(int d) const{
		_RETURN_VAL_IF_FAIL(this->father != nullptr, false);
        if(d == _LEFT_){
            return this->father->left_child == this;
        }else{
            return this->father->right_child == this;
        }
    }
    bool has_sibling() const{
        if(this->is_left_child()){
            return this->father->right_child != nullptr;
        }else if(this->is_right_child()){
            return this->father->left_child != nullptr;
        }else{
            return false;
        }
    }

    bool has_sibling(int d) const{
        if(this->is_left_child()){
            return (d == _RIGHT_) && (this->father->right_child != nullptr);
        }else if(this->is_right_child()){
            return (d == _LEFT_) && (this->father->left_child != nullptr);
        }else{
            return false;
        }
    }

    static int Reflect(int d){
        if(d == _LEFT_){
            return _RIGHT_;
        }else{
            return _LEFT_;
        }
    }
};


template<typename TYPE>
typename TreeNode_<TYPE, 2>::size_type Height(const TreeNode_<TYPE, 2>* cur){
    if (cur == nullptr)
        return 0;
    else
        return 1 + std::max<typename TreeNode_<TYPE, 2>::size_type>(Height(cur->left_child), Height(cur->right_child));
}

template<typename TYPE>
std::ostream& operator<<(std::ostream& stream, const TreeNode_<TYPE, 2>& node) {
    if(node.father == nullptr){
        stream << "father = null";
    }else{
        stream << "father = " << node.father;
    }
    stream << std::endl;
    if(node.left_child == nullptr){
        stream << "left   = null";
    }else{
        stream << "left = " << node.left_child;
    }
    stream << std::endl;
    if(node.right_child == nullptr){
        stream << "right  = null";
    }else{
        stream << "right = " << node.right_child;
    }
    stream << std::endl;
    stream << "value  = " << node.value; 
	return stream;
}


//===============================================
//concept{
    //require public member : value
    //require public member : father
    //require public member : left_child 
    //require public member : right_child
//}
template<typename NODE>
class BinaryTree_ {
public:
    typedef typename NODE::value_type  value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    typedef BinaryTree_<NODE> Self;
    // typedef _BinaryTree_iterator<TYPE, TYPE&, TYPE*> iterator;
    // typedef _BinaryTree_iterator<TYPE, const TYPE&, const TYPE*> const_iterator;

    typedef typename NODE::size_type size_type;
    // typedef std::size_t difference_type;
    typedef NODE          Node;
    typedef Node*        pNode;
    typedef const Node* cpNode;
protected:
    pNode _end;
protected:

    template <typename Callable, typename... Args>
    void _pre_order(pNode current, Callable &&op, Args &&... args){
	    if (current != nullptr) {
	    	std::invoke(op, current, args...);
	    	_pre_order(current->left_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	_pre_order(current->right_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    }
    }
    template <typename Callable, typename... Args>
    void _pre_order(cpNode current, Callable &&op, Args &&... args) const{
	    if (current != nullptr) {
	    	std::invoke(op, current, args...);
	    	_pre_order(current->left_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	_pre_order(current->right_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    }
    }
    template <typename Callable, typename... Args>
    void _in_order(pNode current, Callable &&op, Args &&... args){
	    if (current != nullptr) {
	    	_in_order(current->left_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	std::invoke(op, current, args...);
	    	_in_order(current->right_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    }
    }
    template <typename Callable, typename... Args>
    void _in_order(cpNode current, Callable &&op, Args &&... args) const{
	    if (current != nullptr) {
	    	_in_order(current->left_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	std::invoke(op, current, args...);
	    	_in_order(current->right_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    }
    }
    template <typename Callable, typename... Args>
    void _post_order(pNode current, Callable &&op, Args &&... args){
	    if (current != nullptr) {
	    	_post_order(current->left_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	_post_order(current->right_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	std::invoke(op, current, args...);
	    }
    }
    template <typename Callable, typename... Args>
    void _post_order(cpNode current, Callable &&op, Args &&... args) const{
	    if (current != nullptr) {
	    	_post_order(current->left_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	_post_order(current->right_child, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	std::invoke(op, current, args...);
	    }
    }

    void _destory(pNode&);
    void _copy(pNode&, const pNode&);
    void _reverse(pNode);
    size_type _height(pNode) const;
    size_type _size(const pNode&) const;

    // iterator root();
    // const_iterator root() const;
public:
    //constructor================================
    BinaryTree_(){
        _end  = new Node();
    }
    BinaryTree_(const Self& o){
        _end  = new Node();
        if(!(o.empty())){
            _end->leftc = o.root()->copy();
        }
    }
    //destructor ================================
    ~BinaryTree_(){
        delete _end;
    }
    //operator= =================================
    Self& operator=(const Self& o){
        if(o.empty()){
            return *this;
        }        
        if(!(this->empty())){
            delete this->root();
        }
        this->root(o.root()->copy());
        return *this;
    }

    cpNode root() const{
        return _end->left_child;
    }
    pNode root(){
        return _end->left_child;
    }
    void root(pNode pn){
        _end->left_child = pn;
        _end->left_child->father = _end;
    }
    //Traversal =============================
    template <typename Callable, typename... Args>
    void pre_order(Callable &&op, Args &&... args){
        this->_pre_order(this->root(), std::forward<Callable>(op), std::forward<Args>(args)...);
    }
    template <typename Callable, typename... Args>
    void in_order(Callable &&op, Args &&... args){
        this->_in_order(this->root(), std::forward<Callable>(op), std::forward<Args>(args)...);
    }
    template <typename Callable, typename... Args>
    void in_order(Callable &&op, Args &&... args) const{
        this->_in_order(this->root(), std::forward<Callable>(op), std::forward<Args>(args)...);
    }
    template <typename Callable, typename... Args>
    void post_order(Callable &&op, Args &&... args){
        this->_post_order(this->root(), std::forward<Callable>(op), std::forward<Args>(args)...);
    }
    //iterator===================================
    // iterator begin();
    // const_iterator begin() const;
    // iterator end();
    // const_iterator end() const;
    //===========================================

    bool empty() const{
       return _end->left_child == nullptr; 
    }
    size_type size() const;
    void reverse();
    void clear();
    size_type height() const{
        return this->root()->height();
    }
    pNode neighbor(pNode cur, int d){
        pNode ca = nullptr;            //common ancestor
        if (cur->father != nullptr && cur->father != this->_end
                && !(cur->has_sibling(d))) {
            ca = neighbor(cur->father, d);
        } else if (cur->father != nullptr && cur->father != this->_end){
            ca = cur->father;
        } else {
            ca = nullptr;
        }
        pNode pt = nullptr;
        if (ca != nullptr && cur->father != nullptr && cur->father == ca) {
            pt = ca->child(d);
        } else if (ca != nullptr) {
            pt = ca->child(Node::Reflect(d));
            if(pt == nullptr){
                pt = ca->child(d);
            }
        }
        return pt;
    }

    pNode common_ancestor(pNode cur, int d){
        pNode ca = nullptr;            //common ancestor
        if (cur->father != nullptr && cur->father != this->_end
                && !(cur->has_sibling(d))) {
            ca = common_ancestor(cur->father, d);
        } else if (cur->father != nullptr && cur->father != this->_end){
            ca = cur->father;
        } else {
            ca = nullptr;
        }
        return ca;
    }
};

// template<typename TYPE>
// BinaryTree<TYPE>::BinaryTree(const BinaryTree<TYPE>& a) {
//     this->_root = nullptr;
//     _copy(this->_root, a._root);
// }

// template<typename TYPE>
// BinaryTree<TYPE>::~BinaryTree() {
//     _destory(_root);
// }

// template<typename TYPE>
// BinaryTree<TYPE>& BinaryTree<TYPE>::operator=(
//         const BinaryTree<TYPE>& original) {
//     _destory(this->_root);
//     this->_root=nullptr;
//     _copy(this->_root, original._root);
//     return *this;
// }

// template<typename TYPE>
// void BinaryTree<TYPE>::_destory(pNode& Current) {
//     if (Current != nullptr) {
//         _destory(Current->lchild);
//         _destory(Current->rchild);
//         delete Current;
//         Current = nullptr;
//     }
// }

// template<class TYPE>
// void BinaryTree<TYPE>::_copy(pNode& Current, const pNode& original) {
//     if (Current == nullptr) {
//         Current = new Node(original->value);
//     }
//     if (original->lchild != nullptr) {
//         Current->lchild = new Node(original->lchild->value);
//         Current->lchild->father = Current;
//         _copy(Current->lchild, original->lchild);
//     }
//     if (original->rchild != nullptr) {
//         Current->rchild = new Node(original->rchild->value);
//         Current->rchild->father = Current;
//         _copy(Current->rchild, original->rchild);
//     }
// }






// template<class TYPE>
// _BinaryTree_iterator<TYPE, const TYPE&, const TYPE*> BinaryTree<TYPE>::begin() const {
//     Node *pnode = _root->lchild;
//     if (pnode == nullptr) {
//         return _root;
//     }
//     if (pnode->lchild) {
//         while (pnode->lchild)
//             pnode = pnode->lchild;
//     }
//     return pnode;
// }

// template<class TYPE>
// _BinaryTree_iterator<TYPE, TYPE&, TYPE*> BinaryTree<TYPE>::begin() {
//     Node *pnode = _root->lchild;
//     if (pnode == nullptr) {
//         return _root;
//     }
//     if (pnode->lchild) {
//         while (pnode->lchild)
//             pnode = pnode->lchild;
//     }
//     return pnode;
// }
// template<class TYPE>
// _BinaryTree_iterator<TYPE, TYPE&, TYPE*> BinaryTree<TYPE>::root() {
//     return _root->lchild;
// }
// template<class TYPE>
// _BinaryTree_iterator<TYPE, const TYPE&, const TYPE*> BinaryTree<TYPE>::root() const {
//     return _root->lchild;
// }

// template<class TYPE>
// _BinaryTree_iterator<TYPE, const TYPE&, const TYPE*> BinaryTree<TYPE>::end() const {
//     return _root;
// }

// template<class TYPE>
// _BinaryTree_iterator<TYPE, TYPE&, TYPE*> BinaryTree<TYPE>::end() {
//     return _root;
// }





// template<typename TYPE>
// LarusDef::size_type BinaryTree<TYPE>::_size(const pNode& Current) const {
//     if (Current == nullptr) {
//         return 0;
//     } else {
//         return _size(Current->lchild) + _size(Current->rchild) + 1;
//     }
// }

// template<typename TYPE>
// LarusDef::size_type BinaryTree<TYPE>::size() const {
//     return _size(_root->lchild);
// }

// template<class TYPE>
// void BinaryTree<TYPE>::reverse() {
//     _reverse(_root->lchild);
// }

// template<class TYPE>
// void BinaryTree<TYPE>::_reverse(pNode Current) {
//     if (Current != nullptr) {
//         pNode temp = Current->lchild;
//         Current->lchild = Current->rchild;
//         Current->rchild = temp;
//         _reverse(Current->lchild);
//         _reverse(Current->rchild);
//     }
// }

// template<class TYPE>
// void BinaryTree<TYPE>::clear() {
//     _destory(_root->lchild);
//     //_root = nullptr;
// }


}

#endif /* UTILITY_BINARYTREE_H_ */