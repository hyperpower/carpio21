/*
 * BinaryTree.h
 *
 *  Created on: Dec 22, 2014
 *      Author: zhou
 */

#ifndef _SORTED_BINARYTREE_HPP_
#define _SORTED_BINARYTREE_HPP_

#include <algorithm>
#include <functional>
#include "type_define.hpp"
#include "binary_tree.hpp"

namespace carpio {


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

    pNode insert(const_reference value){
        pNode res = nullptr;
        if(this->empty()){
            this->root(new Node(value));
            return this->root();
        }else{
            res = this->_insert(this->_end->left_child, value);
        }
        return res;
    }
    
    pNode find(const_reference value){
        if(this->empty()){
            return nullptr;
        } else {
            return this->_find(this->_end->left_child, value);
        }
    }
protected:
    pNode _insert(pNode& cur, const_reference value){
        pNode res = nullptr;
        if (cur == nullptr) {
            cur = new Node(value);
            return cur;
        } else if (COMP{}(value, cur->value)) {
            res = _insert(cur->left_child, value);
            cur->left_child->father = cur;
        } else if (COMP{}(cur->value, value)){   //data >= Current->m_value
            res = _insert(cur->right_child, value);
            cur->right_child->father = cur;
        } else {
            return nullptr;
        }
        return res;
    }
    pNode _find(pNode& cur, const_reference value){
        pNode res = nullptr;
        if (cur == nullptr) {
            return nullptr;
        }
        if (COMP{}(value, cur->value)) {
            res = _find(cur->left_child, value);
        } else if (COMP{}(cur->value, value)) {
            res = _find(cur->right_child, value);
        } else {
            return cur;
        }
        return res;
    }
};



}

#endif /* UTILITY_SORTED_BINARYTREE_H_ */