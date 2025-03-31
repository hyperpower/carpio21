/************************
 //  \file   ArrayT.h
 //  \brief
 //
 //  \author zhou
 //  \date   23 janv. 2014
 ***********************/
#ifndef _ARRAYLIST_HPP_
#define _ARRAYLIST_HPP_

#include "type_define.hpp"
#include "algebra/blas/blas_0.hpp"
#include "algebra/blas/blas_1.hpp"
#include "utility/tinyformat.hpp"

#include <initializer_list>

#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <functional>

#undef max
#undef min

namespace carpio {

/**
 * @class Array
 *
 * @brief A class designed for dynamic array.
 *        It's the basic data structure class.
 */
template<typename T>
class ArrayListT_ {
public:
    // type definitions
    typedef T value_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;
    typedef St size_type;
    typedef St difference_type;

    typedef std::function<T(const St&)> FunIndex;

protected:
    St m_Len;
    T *m_p;    //!< The heap for the real data.
public:
    //constructor==================================
    ArrayListT_();
    ArrayListT_(const ArrayListT_<T>&  a);
    ArrayListT_(ArrayListT_<T>&& a);
    ArrayListT_(size_type Len);
    ArrayListT_(const std::initializer_list<T>& l);
    void reconstruct(size_type Len);
    ArrayListT_(size_type Len, const T& nd);
    ArrayListT_(T *nd, size_type Len);
    //=============================================
    virtual ~ArrayListT_();
    //=============================================
    T* data() {
        return m_p;
    }
    const T* data() const {
        return m_p;
    }
    //=============================================
    const_reference operator[](size_type index) const;  //overload []
    reference operator[](size_type index);
    const_reference operator()(size_type index) const;
    reference operator()(size_type index);
    const_reference at(size_type index) const;
    reference at(size_type index);
    //operator=====================================
    ArrayListT_<T>& operator=(const ArrayListT_<T> &a);
    ArrayListT_<T>& operator=(ArrayListT_<T>&&a);
    // iterator support============================
    iterator begin() {
        return m_p;
    }
    const_iterator begin() const {
        return m_p;
    }
    iterator end() {
        return m_p + m_Len;
    }
    const_iterator end() const {
        return m_p + m_Len;
    }

    size_type size() const;  //!< get length of the array
    T get(size_type i) const;
    void set(size_type i, const T& value);
    void assign(const T& nd);
    void assign(FunIndex fun);

    // front() and back()
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    void swap(size_type i1, size_type i2);
    void reverse();
    void push_back(const T& nd);
    void pop_back();
    void erase(size_type i);
    virtual void resize(const size_type& new_size);

    bool empty() const;
    bool non_empty() const;
    inline bool check_idx(size_type) const;

    bool has(const T& nd) const;              //overload ==
    size_type find(const T& nd) const;        //overload ==
    size_type count_equal(const T& nd) const; //overload ==
};

template<typename T>
ArrayListT_<T>::ArrayListT_() {
    m_Len = 0;
    m_p = nullptr;
}

template<typename T>
ArrayListT_<T>::ArrayListT_(const ArrayListT_<T>& a) {
    // std::cout << "ArrayListT assgin &" << std::endl;
    m_Len = a.size();
    m_p   = new T[m_Len];
    //unrolled loop
    Copy(m_Len, a.m_p, this->m_p);
}
template<typename T>
ArrayListT_<T>::ArrayListT_(ArrayListT_<T>&& a) {
    // std::cout << "ArrayListT move" << std::endl;
    m_Len = a.m_Len;
    m_p   = a.m_p;
    a.m_Len = 0;
    a.m_p   = nullptr;
}

template<typename T>
ArrayListT_<T>::ArrayListT_(size_type Len) {
    m_Len = Len;
    m_p   = new T[m_Len];
}

template<typename T>
ArrayListT_<T>::ArrayListT_(const std::initializer_list<T>& l) {
	St n      = l.size();
	m_Len     = n;
	this->m_p = new T[m_Len];
	St i  = 0;
	for(auto& v : l){
		this->m_p[i] = v;
		i++;
	}
}

template<typename T>
void ArrayListT_<T>::reconstruct(size_type Len) {
    if (Len == 0 && m_Len == 0) {
        m_p = nullptr;
        return;
    }
    if (Len == 0 && nullptr != m_p) {
        delete[] m_p;
        m_p = nullptr;
        return;
    }
    ASSERT(Len > 0);
    if (nullptr != m_p)
        delete[] m_p;
    m_Len = Len;
    m_p = new T[m_Len];
}

template<typename T>
ArrayListT_<T>::ArrayListT_(size_type Len,
                          const T& nd) {
    m_Len = Len;
    m_p = new T[Len];
    for (size_type i = 0; i < m_Len; i++) {
        m_p[i] = nd;
    }
}

template<typename T>
ArrayListT_<T>::ArrayListT_(T *nd,
                          size_type Len) {
    m_Len = Len;
    m_p = new T[Len];
    for (size_type i = 0; i < m_Len; i++) {
        m_p[i] = nd[i];
    }
}

template<typename T>
ArrayListT_<T>::~ArrayListT_() {
    delete[] m_p;
}

template<typename T>
const T& ArrayListT_<T>::operator[](size_type index) const {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}

template<typename T>
T& ArrayListT_<T>::operator[](size_type index) {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}

template<typename T>
const T& ArrayListT_<T>::operator()(size_type index) const {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}

template<typename T>
T& ArrayListT_<T>::operator()(size_type index) {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}
template<typename T>
const T& ArrayListT_<T>::at(size_type index) const {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}
template<typename T>
T& ArrayListT_<T>::at(size_type index) {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}

template<typename T>
ArrayListT_<T>& ArrayListT_<T>::operator=(const ArrayListT_<T> &a) {
    // std::cout << "ArrayListT operator= &" << std::endl;
    if (this == &a) {
        return *this;
    }
    if (a.m_Len <= 0) {
        this->resize(0);
        return *this;
    }
    if (m_Len == a.size()) {
        //unrolled loop
        Copy(m_Len, a.m_p, m_p);
    } else {
        delete[] this->m_p;
        m_Len = a.size();
        m_p = new T[m_Len];
        Copy(m_Len, a.m_p, m_p);
    }
    return *this;
}
template<typename T>
ArrayListT_<T>& ArrayListT_<T>::operator=(ArrayListT_<T> &&a) {
    // std::cout << "ArrayListT operator= &&" << std::endl;
    if (this == &a) {
        return *this;
    } else {
        m_Len   = a.m_Len;
        m_p     = a.m_p;
        a.m_Len = 0;
        a.m_p   = nullptr;
    }
    return *this;
}
template<typename T>
void ArrayListT_<T>::set(size_type i, const T& value) {
    ASSERT(i >= 0 && i < m_Len);
    m_p[i] = value;
}

template<typename T>
void ArrayListT_<T>::assign(const T& nd) {
    Assign(this->m_Len, this->m_p, nd);
}
template<typename T>
void ArrayListT_<T>::assign(ArrayListT_<T>::FunIndex fun) {
    for (size_type i = 0; i < m_Len; i++) {
        m_p[i] = fun(i);
    }
}
template<typename T>
St ArrayListT_<T>::size() const {
    return m_Len;
}
template<typename T>
T& ArrayListT_<T>::front() {
    ASSERT(m_Len > 0);
    return m_p[0];
}
template<typename T>
const T& ArrayListT_<T>::front() const {
    ASSERT(m_Len > 0);
    return m_p[0];
}
template<typename T>
T& ArrayListT_<T>::back() {
    ASSERT(m_Len > 0);
    return m_p[m_Len - 1];
}
template<typename T>
const T& ArrayListT_<T>::back() const {
    ASSERT(m_Len > 0);
    return m_p[m_Len - 1];
}

template<typename T>
T ArrayListT_<T>::get(size_type i) const {
    ASSERT(i >= 0 && i < m_Len);
    return m_p[i];
}

template<typename T>
void ArrayListT_<T>::swap(size_type i1, size_type i2) {
    ASSERT(i1 >= 0 && i1 < m_Len && i2 >= 0 && i2 < m_Len);
    if (i1 == i2) {
        return;
    }
    T tmp;
    tmp = m_p[i1];
    m_p[i1] = m_p[i2];
    m_p[i2] = tmp;
}

template<typename T>
void ArrayListT_<T>::reverse() {
    if (empty()) {
        return;
    }
    T tmp;
    for (int i1 = 0,i2 = size() - 1; i1 < i2; i1++, i2--) {
        tmp = m_p[i1];
        m_p[i1] = m_p[i2];
        m_p[i2] = tmp;
    }
}

template<typename T>
void ArrayListT_<T>::push_back(const T& nd) {
    if (m_p == nullptr) {
        m_Len = 1;
        m_p = new T[1];
        m_p[0] = nd;
    } else {
        T *tmp = new T[m_Len];
        for (size_type i = 0; i < m_Len; i++) {
            tmp[i] = m_p[i];
        }
        delete[] m_p;
        m_Len += 1;
        m_p = new T[m_Len];
        for (size_type i = 0; i < m_Len - 1; i++) {
            m_p[i] = tmp[i];
        }
        m_p[m_Len - 1] = nd;
        delete[] tmp;
    }
}

template<typename T>
void ArrayListT_<T>::pop_back() {
    if (m_p == NULL) {
        return;
    } else if (m_Len == 1) {
        m_Len = 0;
        delete[] m_p;
        m_p = NULL;
    } else {
        T *tmp = new T[m_Len];
        for (size_type i = 0; i < m_Len; i++) {
            tmp[i] = m_p[i];
        }
        delete[] m_p;
        m_Len -= 1;
        m_p = new T[m_Len];
        for (size_type i = 0; i < m_Len; i++) {
            m_p[i] = tmp[i];
        }
        delete[] tmp;
    }
}

template<typename T>
void ArrayListT_<T>::erase(size_type idx) {
    ASSERT(idx >= 0 && idx < m_Len);
    if (m_Len == 1) {
        m_Len = 0;
        delete[] m_p;
        m_p = NULL;
    } else {
        T *tmp = new T[m_Len];
        for (size_type i = 0; i < m_Len; i++) {
            if (i != idx)
                tmp[i] = m_p[i];
        }
        delete[] m_p;
        m_Len--;
        m_p = new T[m_Len];
        for (size_type i = 0; i < idx; i++) {
            m_p[i] = tmp[i];
        }
        for (size_type i = idx; i < m_Len; i++) {
            m_p[i] = tmp[i + 1];
        }
        delete[] tmp;
    }
}

template<typename T>
void ArrayListT_<T>::resize(const size_type& new_size) {
    ASSERT(new_size >= 0);
    if (new_size == m_Len) {
        return;
    } else if (empty()) {
        reconstruct(new_size);
    } else {
        T *tmp = new T[new_size];
        for (size_type i = 0; i < m_Len && i < new_size; i++) {
            tmp[i] = m_p[i];
        }
        delete[] m_p;
        m_Len = new_size;
        m_p = tmp;
    }
}

template<typename T>
bool ArrayListT_<T>::has(const T& nd) const {
    if (m_p == NULL) {
        return false;
    } else {
        for (size_type i = 0; i < m_Len; i++) {
            if (nd == m_p[i]) {
                return true;
            }
        }
        return false;
    }
}

template<typename T>
St ArrayListT_<T>::find(const T& nd) const {
    if (m_p == NULL) {
        return -1;
    } else {
        for (size_type i = 0; i < m_Len; i++) {
            if (nd == m_p[i]) {
                return i;
            }
        }
        return -1;
    }
}
template<typename T>
St ArrayListT_<T>::count_equal(const T& nd) const { //overload ==
    size_type res = 0;
    if (m_p == NULL) {
        return 0;
    } else {
        for (size_type i = 0; i < m_Len; i++) {
            if (nd == m_p[i]) {
                res++;
            }
        }
    }
    return res;
}
template<typename T>
bool ArrayListT_<T>::empty() const {
    if (m_p == NULL && m_Len == 0) {
        return true;
    } else {
        return false;
    }
}

template<typename T>
bool ArrayListT_<T>::non_empty() const {
    return !empty();
}
template<typename T>
inline bool ArrayListT_<T>::check_idx(ArrayListT_<T>::size_type idx) const {
    if (idx >= 0 && idx < m_Len) {
        return true;
    } else {
        return false;
    }

}
//end of Class arrayListT

}

#endif /* ARRAY_LIST_HPP_ */
