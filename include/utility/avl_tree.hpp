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

    bool is_balanced() const{
        size_type h = 0;
        return this->_is_balanced(this->root(), h);
    }
    // input  value of elements to remove
    // return Number of elements removed (0 or 1).
    size_type erase(const_reference value){
        pNode res = _erase(this->_end->left_child, value);
        if(res == nullptr){
            return 0;
        }else{
            return 1;
        }
    }

protected:
    bool _is_balanced(cpNode cur) const{
        size_type h = 0;
        return this->_is_balanced(cur, h);
    }
    bool _is_balanced(cpNode cur, size_type& height) const{
        if (cur == nullptr){
            height = 0;
            return true;
        }

        size_type left, right;
        if(this->_is_balanced(cur->left_child, left) &&
           this->_is_balanced(cur->right_child, right)){
            size_type diff = std::abs(int(left-right));
            if(diff <= 1){
                height = 1 + std::max<size_type>(left, right);
                return true;
            }
        }
        return false;
    }

    pNode _insert(pNode& cur, const_reference value){
        pNode res = nullptr;
        if (cur == nullptr) {
            cur = new Node(value);
            return cur;
        } else if (COMP{}(value, cur->value)) {  // value < cur-value 
            res = _insert(cur->left_child, value);
            cur->left_child->father = cur;
            if (!_is_balanced(cur)){
                if (COMP{}(value, cur->left_child->value)){
                    _right_rotate(cur);
                }else{
                    _LR_rotate(cur);
                }
            }
        } else if (COMP{}(cur->value, value)){   // cur->value < value
            res = _insert(cur->right_child, value);
            cur->right_child->father = cur;
            if (!_is_balanced(cur)){
                if (COMP{}(cur->right_child->value, value)){
                    _left_rotate(cur);
                }else{
                    _RL_rotate(cur);
                }
            }
        } else {
            return nullptr;
        }
        return res;
    }

    pNode _erase(pNode& cur, const_reference value){
        pNode res = nullptr;
        if (cur == nullptr){  // not find
            return nullptr;
        }
        if (COMP{}(value, cur->value)) {  // value < cur-value 
            res = _erase(cur->left_child, value);
            if (!_is_balanced(cur)){
                if (cur->right_child->left_child != nullptr &&
                    (Height(cur->right_child->left_child) > Height(cur->right_child->right_child))){
                    _RL_rotate(cur);
                }else{
                    _left_rotate(cur);
                }
            }
        } else if (COMP{}(cur->value, value)){   // cur->value < value
            res = _erase(cur->right_child, value);
            if (!_is_balanced(cur)){
                if (cur->left_child->right_child != nullptr &&
                    (Height(cur->left_child->right_child) > Height(cur->left_child->left_child))){
                    _LR_rotate(cur);
                }else{
                    _right_rotate(cur);
                }
            }
        } else {
            if(cur->has_children()){
                pNode temp = cur->right_child;
                // find min in left sub tree
                temp = temp->left_most();
                // min in left sub tree assign to current
                cur->value = temp->value;
                _erase(temp, temp->value);
                if(!_is_balanced(cur)){
                    if (cur->left_child->right_child != nullptr &&
                        (Height(cur->left_child->right_child) > Height(cur->left_child->left_child))){
                        _LR_rotate(cur);
                    }else{
                        _right_rotate(cur);
                    }
                }
            } else {  // 1 or 0 child
                pNode temp = cur;
                if(cur->has_child(Node::_LEFT_)){
                    pNode& l = cur->left_child;
                    l->father = cur->father;
                    if(cur->father->left_child == cur){
                        cur->father->left_child  = l;    
                    }else{
                        cur->father->right_child = l;
                    }
                    cur = l;
                }else if(cur->has_child(Node::_RIGHT_)){
                    pNode& r = cur->right_child;
                    r->father = cur->father;
                    if(cur->father->left_child == cur){
                        cur->father->left_child  = r;    
                    }else{
                        cur->father->right_child = r;
                    }
                    cur = r;
                }else{
                    if(cur->father->left_child == cur){
                        cur = cur->father;
                        cur->left_child = nullptr;    
                    }else{
                        cur = cur->father;
                        cur->right_child = nullptr;
                    }
                }
                delete(temp);
                res = cur;
            }
        }
        return res;
    }
// RR case --> Left Rotate
//    z                                y
//  /  \                            /   \ 
// T1   y     Left Rotate(z)       z      x
//     /  \   - - - - - - - ->    / \    / \
//    T2   x                     T1  T2 T3  T4
//        / \
//      T3  T4
    void _left_rotate(pNode& cur) {
        pNode temp;
        temp = cur->right_child;
        if(temp->left_child)
            temp->left_child->father = cur;
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
//  LL case --> Right Rotate
//  T1, T2, T3 and T4 are subtrees.
//          z (cur)                                y 
//         / \                                   /   \
//        y   T4      Right Rotate (z)          x      z
//       / \          - - - - - - - - ->      /  \    /  \ 
//      x   T3                               T1  T2  T3  T4
//     / \
//   T1   T2
    void _right_rotate(pNode& cur) {
        pNode temp;
        temp = cur->left_child;
        if(temp->right_child)
            temp->right_child->father = cur;
        cur->left_child = temp->right_child;
        temp->right_child = cur;

        temp->father = cur->father;
        cur->father = temp;

        if(temp->father->left_child == cur){
            temp->father->left_child = temp;    
        }else{
            temp->father->right_child = temp;
        }
        cur = temp;
    }

// Left Right  
//      z                               z                           x
//     / \                            /   \                        /  \ 
//    y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
//   / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
// T1   x                          y    T3                    T1  T2 T3  T4
//     / \                        / \
//   T2   T3                    T1   T2
    void _LR_rotate(pNode& cur){
        _left_rotate(cur->left_child);
        _right_rotate(cur);
    }
//
//    z                            z                            x
//   / \                          / \                          /  \ 
// T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
//     / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
//    x   T4                      T2   y                  T1  T2  T3  T4
//   / \                              /  \
// T2   T3                           T3   T4
    void _RL_rotate(pNode& cur){
        _right_rotate(cur->right_child);
        _left_rotate(cur);
    }


};

}

#endif