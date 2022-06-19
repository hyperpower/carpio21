/*
 * BinaryTree.h
 *
 *  Created on: Dec 22, 2014
 *      Author: zhou
 */

#ifndef _BINARYTREE_HPP_
#define _BINARYTREE_HPP_

#include "type_define.hpp"
// #include "Iterator.h"

namespace carpio {

template<class TYPE, std::size_t S>
class TreeNode_{
};

template<typename TYPE>
class TreeNode_<TYPE, 2>{
public:
    typedef TreeNode_<TYPE, 2>   Node;
    typedef TreeNode_<TYPE, 2>* pNode;
    typedef const TreeNode_<TYPE, 2>* cpNode;
    typedef TYPE value_type;
    typedef std::size_t size_type;

    typedef TreeNode_<TYPE, 2> Self;
public:
    TYPE  value;
    pNode f;
    pNode leftc;
    pNode rightc;

    TreeNode_():
        f(nullptr), leftc(nullptr), rightc(nullptr){}
    
    TreeNode_(const TYPE &data):
        value(data), f(nullptr), leftc(nullptr), rightc(nullptr){} 
    
    TreeNode_(const TYPE &data, pNode lc, pNode rc, pNode fc):
        value(data){
        leftc  = lc;
        rightc = rc;
        father = fc;
    }

    ~TreeNode_(){
        this->destory(this->leftc);
        this->destory(this->rightc);
        if(this->f != nullptr){
            if(this == this->f->left_child()){
                this->f->left_child(nullptr);
            }
            if(this == this->f->right_child()){
                this->f->right_child(nullptr);
            }
        }
    }
    pNode child(std::size_t idx){
        switch (idx)
        {
        case 0:
            return this->left_child; 
            break;
        case 1:
            return this->right_child; 
            break;
        default:
            break;
        }
    }
    

    pNode left_child(){
        return this->leftc;
    }
    pNode right_child(){
        return this->rightc;
    }
    cpNode left_child() const{
        return this->leftc;
    }
    cpNode right_child() const{
        return this->rightc;
    }
    void left_child(pNode node){
        this->leftc = node;
    }
    void right_child(pNode node){
        this->rightc = node;
    }

    void father(pNode node){
        this->f = node;
    }

    pNode leftmost(){
        pNode ptn = this;
        while (ptn->lchild != nullptr)
            ptn = ptn->lchild;
        return ptn;
    }
    pNode rightmost(){
        pNode ptn = this;
        while (ptn->rightc != nullptr)
            ptn = ptn->rightc;
        return ptn;
    }

    St height() const{
        return 1 + std::max(height(this->leftc), height(this->rightc));
    }
    St height(pNode cur) const{
        if (cur == nullptr)
            return 0;
        else
            return 1 + MAX(height(cur->left_child()), height(cur->right_child()));
    }

    void destory(pNode& Current) {
        if (Current != nullptr) {
            destory(Current->leftc);
            destory(Current->rightc);
            delete Current;
            Current = nullptr;
        }
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
       
        if (src->leftc != nullptr){
            dst->leftc = new Node(src->leftc->value);
            dst->leftc->f = dst;
            copy(src->leftc, dst->leftc);
        }
        if (src->rightc != nullptr){
            dst->rightc = new Node(src->rightc->value);
            dst->rightc->f = dst;
            copy(src->rightc, dst->rightc);
        }
    }
};

//===============================================
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

    // typedef void (*pFun_BinaryTree)(pNode, utPointer);
    template <typename Callable, typename... Args>
    void _pre_order(pNode current, Callable &&op, Args &&... args){
	    if (current != nullptr) {
	    	std::invoke(op, current, args...);
	    	_pre_order(current->leftc, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	_pre_order(current->rightc, std::forward<Callable>(op), std::forward<Args>(args)...);
	    }
    }
    template <typename Callable, typename... Args>
    void _in_order(pNode current, Callable &&op, Args &&... args){
	    if (current != nullptr) {
	    	_in_order(current->leftc, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	std::invoke(op, current, args...);
	    	_in_order(current->rightc, std::forward<Callable>(op), std::forward<Args>(args)...);
	    }
    }
    template <typename Callable, typename... Args>
    void _post_order(pNode current, Callable &&op, Args &&... args){
	    if (current != nullptr) {
	    	_post_order(current->leftc, std::forward<Callable>(op), std::forward<Args>(args)...);
	    	_post_order(current->rightc, std::forward<Callable>(op), std::forward<Args>(args)...);
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
        return _end->leftc;
    }
    pNode root(){
        return _end->leftc;
    }
    void root(pNode pn){
        _end->leftc = pn;
        _end->leftc->f = _end;
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
       return _end->leftc == nullptr; 
    }
    size_type size() const;
    void reverse();
    void clear();
    size_type height() const;
protected:
};

template<class NODE, class COMP = std::less<typename NODE::value_type> >
class SortedBinaryTree_ : public BinaryTree_<NODE>{
public:
    typedef typename NODE::value_type  value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    typedef BinaryTree_<NODE> Base;
    typedef SortedBinaryTree_<NODE, COMP> Self;

    typedef NODE          Node;
    typedef Node*        pNode;
    typedef const Node* cpNode;

public:
    SortedBinaryTree_():Base(){}

    SortedBinaryTree_(const Self& o){
        this->_end = new Node();
        this->root(o.root()->copy());
    }

    void insert(const_reference value){
        if(this->empty()){
            this->root(new Node(value));
            return;
        }else{
            this->_insert(this->_end->leftc, value);
        }
    }
protected:
    void _insert(pNode& cur, const_reference value){
        if (cur == nullptr) {
            cur = new Node(value);
            return;
        } else if (COMP{}(value, cur->value)) {
            _insert(cur->leftc, value);
            cur->leftc->f = cur;
        } else if (!COMP{}(value, cur->value)){   //data >= Current->m_value
            _insert(cur->rightc, value);
            cur->rightc->f = cur;
        } else {
            return;
        }
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

// template<typename TYPE>
// void BinaryTree<TYPE>::_preorder(pNode Current, pFun_BinaryTree visit,
//         utPointer utp) {
//     if (Current != nullptr) {
//         (*visit)(Current->value, utp);
//         _preorder(Current->lchild, visit, utp);
//         _preorder(Current->rchild, visit, utp);
//     }
// }

// template<class TYPE>
// void BinaryTree<TYPE>::pre_order(pFun_BinaryTree visit, utPointer utp) {
//     _preorder(_root->lchild, visit, utp);
// }

// template<typename TYPE>
// void BinaryTree<TYPE>::_postorder(pNode Current, pFun_BinaryTree visit,
//         utPointer utp) {
//     if (Current != nullptr) {
//         _postorder(Current->lchild, visit, utp);
//         _postorder(Current->rchild, visit, utp);
//         (*visit)(Current->value, utp);
//     }
// }

// template<class TYPE>
// void BinaryTree<TYPE>::post_order(pFun_BinaryTree visit, utPointer utp) {
//     _postorder(_root->lchild, visit, utp);
// }

// template<typename TYPE>
// void BinaryTree<TYPE>::_inorder(pNode Current, pFun_BinaryTree visit,
//         utPointer utp) {
//     if (Current != nullptr) {
//         _inorder(Current->lchild, visit, utp);
//         (*visit)(Current->value, utp);
//         _inorder(Current->rchild, visit, utp);
//     }
// }

// template<class TYPE>
// void BinaryTree<TYPE>::in_order(pFun_BinaryTree visit, utPointer utp) {
//     _inorder(_root->lchild, visit, utp);
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
// bool BinaryTree<TYPE>::empty() const {
//     return _root->lchild == nullptr;
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