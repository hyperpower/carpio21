/*
 * BinaryTree.h
 *
 *  Created on: Dec 22, 2014
 *      Author: zhou
 */

#ifndef _AVLTREE_HPP_
#define _AVLTREE_HPP_

#include <algorithm>
#include "utility/binary_tree.hpp"
// #include "Iterator.h"

namespace carpio {

template<class NODE, class COMP = std::less<typename NODE::value_type> >
class AvlTree_ : public SortedBinaryTree_<NODE>{
public:
    typedef typename NODE::value_type  value_type;
    typedef std::size_t size_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    typedef SortedBinaryTree_<NODE> Base;
    typedef AvlTree_<NODE, COMP> Self;

    typedef NODE          Node;
    typedef Node*        pNode;
    typedef const Node* cpNode;

public:
    AvlTree_():Base(){}

    AvlTree_(const Self& o):Base(){
    }

    pNode insert(const_reference value){
        pNode res = nullptr;
        if(this->empty()){
            this->root(new Node(value));
            return this->root();
        }else{
            res = _insert(this->_end->left_child, value);
        }
        return res;
    }

    bool is_balanced(pNode cur, size_type& depth){
        if (cur == nullptr){
            depth = 0;
            return true;
        }

        size_type left, right;
        if(this->is_balanced(cur->left_child, left) &&
           this->is_balanced(cur->right_child, right)){
            size_type diff = std::abs(left-right)
            if(diff <= 1){
                depth = 1 + std::max<size_type>(left, right);
                return true;
            }
        }
        return false;
    }

protected:
    pNode _insert(pNode& cur, const_reference value){
        pNode res = nullptr;
        if (cur == nullptr) {
            cur = new Node(value);
            return cur;
        } else if (COMP{}(value, cur->value)) {  // less
            res = _insert(cur->left_child, value);
            cur->left_child->father = cur;
            size_type hr = Height(cur->right_child);
            size_type hl = Height(cur->left_child);
            if (2 <= int(hl - hr)){
                if (COMP{}(value, cur->left_child->value)){
                    _single_rotate_L(cur);
                }else{
                    _double_rotate_LR(cur);
                }
            }
        } else if (COMP{}(cur->value, value)){   //data >= Current->m_value
            res = _insert(cur->right_child, value);
            cur->right_child->father = cur;
            size_type hr = Height(cur->right_child);
            size_type hl = Height(cur->left_child);
            if (2 <= int(hr - hl)){
                if (COMP{}(cur->right_child->value, value)){
                    _single_rotate_R(cur);
                }else{
                    _double_rotate_RL(cur);
                }
            }
        } else {
            return nullptr;
        }
        return res;
    }
    
    void _single_rotate_R(pNode& cur) {
        pNode temp;
        temp = cur->right_child;
        cur->right_child = temp->left_child;
        temp->left_child = cur;

        temp->father = cur->father;
        cur->father = temp;
        
        if(temp->father->left_child == cur){
            temp->father->left_child = temp;    
        }else{
            temp->father->right_child = temp;
        }
        
        cur = temp;
    }

    void _single_rotate_L(pNode& cur) {
        pNode temp;
        temp = cur->left_child;
        cur->left_child = temp->right_child;
        temp->right_child = cur;

        temp->father = cur->father;
        cur->father = temp;

        if(temp->father->left_child == cur){
            temp->father->left_child = temp;    
        }else{
            temp->father->right_child = temp;
        }
        // if (temp->right_child){
            // temp->right_child->father = cur;
        // }
        cur = temp;
    }

    void _double_rotate_LR(pNode& cur){
        _single_rotate_R(cur->left_child);
        _single_rotate_L(cur);
    }

    void _double_rotate_RL(pNode& cur){
        _single_rotate_L(cur->right_child);
        _single_rotate_R(cur);
    }


};

}

#endif