/************************
 //  \file   Matrix.hpp
 //  \brief
 // 
 //  \author zhou
 //  \date   23 janv. 2014 
 ***********************/
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <iostream>
#include <iterator>

#include "algebra/array/array_list.hpp"
#ifdef OPENMP
#include <omp.h>
#endif

namespace carpio {

template<typename T>
class MatrixT_ {
public:
    // type definitions===================
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef St size_type;
    typedef St difference_type;
protected:
    St m_iLen;
    St m_jLen;
    ArrayListT_<T> *m_mp;
public:
    //constructor==========================
    MatrixT_();
    MatrixT_(const MatrixT_<T>& a);
    MatrixT_(size_type iLen, size_type jLen);
    MatrixT_(size_type iLen, size_type jLen, T** value);
    void reconstruct(size_type iLen, size_type jLen);
    //=============================================
    MatrixT_<T>& operator=(const MatrixT_<T> &a);
    //=============================================
    ~MatrixT_();
    //Capacity=====================================
    size_type size() const;
    size_type size_i() const;
    size_type size_j() const;
    bool empty() const;
    //Element access===============================
    ArrayListT_<T>& operator[](size_type index);
    const ArrayListT_<T>& operator[](size_type index) const;
    reference operator()(size_type i, size_type j);
    const_reference operator()(size_type i, size_type j) const;
    reference at(size_type i, size_type j);
    const_reference at(size_type i, size_type j) const;
    T get(size_type i, size_type j);
    //T* getpValue(size_type i, size_type j);
    void set(size_type i, size_type j, const T& value);
    void set_row(size_type i, const T& value);
    void set_col(size_type i, const T& value);
    void assign(const T& value);
    //=============================================

    void swap(size_type i1, size_type j1, size_type i2, size_type j2);

    size_type count_equal(T value); //overload ==

    inline bool check_idx_i(size_type);
    inline bool check_idx_j(size_type);

    //Low efficient function for small matrix
    void append_row(const ArrayListT_<T> &);
    void appendCol(const ArrayListT_<T> &);
    void insert_row_back(size_type, const ArrayListT_<T>&);
    void insert_col_back(size_type, const ArrayListT_<T>&);
    void delete_row(size_type);
    void delete_col(size_type);

protected:
    //class iterator: public std::iterator<std::input_iterator_tag, KeyType>
    //<class Category, class T, class Distance = ptrdiff_t,
    //       class Pointer = T*, class Reference = T&>
    template<class _T, class _Ptr, class _Ref, class _pMatrix>
    class _iterator: public std::iterator<std::bidirectional_iterator_tag, _T,
            ptrdiff_t, _Ptr, _Ref> {
    private:
        typedef _T value_type;
        typedef _Ptr pointer;
        typedef _Ref reference;

        _pMatrix pm;
        _Ptr ptr;
        size_type i;
        size_type j;
    public:
        _iterator() :
                pm(NULL), ptr(nullptr), i(0), j(0) {
            /* empty */
        }

        _iterator(_pMatrix pm, _Ptr p, size_type i, size_type j) {
            this->pm = pm;
            this->ptr = p;
            this->i = i;
            this->j = j;
        }

        _iterator(const _iterator& it) {
            pm = it.pm;
            ptr = it.ptr;
            i = it.i;
            j = it.j;
        }

        _iterator& operator ++() {
            i++;
            if (i >= pm->size_i()) {
                i = 0;
                j++;
                if (j >= pm->size_j()) {
                    ptr = nullptr;
                    return *this;
                }
            }
            ptr = &pm->operator()(i, j);
            return *this;
        }

        _iterator& operator --() {
            i--;
            if (i < 0) {
                i = pm->size_i() - 1;
                j--;
                if (j < 0) {
                    i = 0;
                    j = pm->size_j();
                    ptr = nullptr;
                    return (*this);
                }
            }
            ptr = pm->operator()(i, j);
            return *this;
        }

        _iterator operator --(int) {
            _iterator copy(*this);
            operator--();
            return copy;
        }

        _iterator operator ++(int) {
            _iterator copy(*this);
            operator++();
            return copy;
        }

        bool operator ==(const _iterator& rhs) {
            return pm == rhs.pm && ptr == rhs.ptr && i == rhs.i && j == rhs.j;
        }

        bool operator !=(const _iterator& rhs) {
            return !(*this == rhs);
        }

        reference operator *() const {
            return (*ptr);
        }

        pointer operator ->() const {
            return ptr;
        }
    };

public:
    typedef _iterator<T, T*, T&, MatrixT_<T>*> iterator;
    typedef _iterator<T, const T*, const T&, const MatrixT_<T>* > const_iterator;

    iterator begin() {
        return iterator(this, &(this->operator()(0, 0)), 0, 0);
    }

    const_iterator begin() const {
        return const_iterator(this, &(this->operator()(0, 0)), 0, 0);
    }

    iterator end() {
        return iterator(this, nullptr, 0, this->m_jLen);
    }
    const_iterator end() const {
        return const_iterator(this, nullptr, 0, this->m_jLen);
    }

};

template<typename T>
inline bool MatrixT_<T>::check_idx_i(St i) {
    return (i >= 0 && i < m_iLen) ? true : false;
}

template<typename T>
inline bool MatrixT_<T>::check_idx_j(St j) {
    return (j >= 0 && j < m_jLen) ? true : false;
}

template<typename T>
MatrixT_<T>::MatrixT_() {
    m_iLen = 0;
    m_jLen = 0;
    m_mp = nullptr;
}

template<typename T>
MatrixT_<T>::MatrixT_(const MatrixT_<T>& a) {
    m_iLen = a.size_i();
    m_jLen = a.size_j();
    m_mp = new ArrayListT_<T> [m_iLen];
    for (size_type i = 0; i < m_iLen; i++) {
        m_mp[i].reconstruct(m_jLen);
    }
    for (size_type i = 0; i < m_iLen; i++) {
        for (size_type j = 0; j < m_jLen; j++) {
            m_mp[i][j] = a[i][j];
        }
    }
}

template<typename T>
MatrixT_<T>::MatrixT_(St iLen, St jLen) {
    m_iLen = iLen;
    m_jLen = jLen;
    //m_total = m_iLen * m_jLen;
    m_mp = new ArrayListT_<T> [m_iLen];
    for (size_type i = 0; i < m_iLen; i++) {
        m_mp[i].reconstruct(m_jLen);
    }
}

template<typename T>
MatrixT_<T>::MatrixT_(St iLen, St jLen, T** value) {
    m_iLen = iLen;
    m_jLen = jLen;
    //m_total = m_iLen * m_jLen;
    m_mp = new ArrayListT_<T> [m_iLen];
    for (size_type i = 0; i < m_iLen; i++) {
        m_mp[i].reconstruct(m_jLen);
    }
    for (size_type i = 0; i < m_iLen; i++) {
        for (size_type j = 0; j < m_jLen; j++) {
            m_mp[i][j] = value[i][j];
        }
    }
}
template<typename T>
void MatrixT_<T>::reconstruct(St iLen, St jLen) {
    m_iLen = iLen;
    m_jLen = jLen;
    //m_total = m_iLen * m_jLen;
    if (m_mp != nullptr) {
        delete[] m_mp;
    }
    m_mp = new ArrayListT_<T> [m_iLen];
    for (size_type i = 0; i < m_iLen; i++) {
        m_mp[i].reconstruct(m_jLen);
    }
}
template<typename T>
MatrixT_<T>::~MatrixT_() {
    delete[] m_mp;
}
template<typename T>
const ArrayListT_<T>& MatrixT_<T>::operator[](St index) const {
    ASSERT(index >= 0 && index < m_iLen);
    return m_mp[index];
}
template<typename T>
ArrayListT_<T>& MatrixT_<T>::operator[](St index) {
    ASSERT(index >= 0 && index < m_iLen);
    return m_mp[index];
}
template<typename T>
bool MatrixT_<T>::empty() const {
    return m_mp == nullptr;
}

template<typename T>
MatrixT_<T>& MatrixT_<T>::operator=(const MatrixT_<T> &a) {
    if (this == &a) {
        return *this;
    }
    if (m_iLen == a.size_i() && m_jLen == a.size_j()) {
        for (size_type i = 0; i < m_iLen; i++) {
            for (size_type j = 0; j < m_jLen; j++) {
                this->m_mp[i][j] = a[i][j];
            }
        }
    } else {
        delete[] this->m_mp;
        m_iLen = a.size_i();
        m_jLen = a.size_j();
        //m_total = m_iLen * m_jLen;
        m_mp = new ArrayListT_<T> [m_iLen];
        for (size_type i = 0; i < m_iLen; i++) {
            m_mp[i].reconstruct(m_jLen);
        }
        for (size_type i = 0; i < m_iLen; i++) {
            for (size_type j = 0; j < m_jLen; j++) {
                this->m_mp[i][j] = a[i][j];
            }
        }
    }
    return *this;
}

template<typename T>
St MatrixT_<T>::size() const {
    return m_iLen * m_jLen;
}
template<typename T>
St MatrixT_<T>::size_i() const {
    return m_iLen;
}
template<typename T>
St MatrixT_<T>::size_j() const {
    return m_jLen;
}
template<typename T>
T MatrixT_<T>::get(St i, St j) {
    ASSERT(i >= 0 && i < m_iLen);
    ASSERT(j >= 0 && j < m_jLen);
    return m_mp[i][j];
}
template<typename T>
typename MatrixT_<T>::reference MatrixT_<T>::operator()(size_type i,
        size_type j) {
    ASSERT(i >= 0 && i < m_iLen);
    ASSERT(j >= 0 && j < m_jLen);
    return m_mp[i][j];
}
template<typename T>
typename MatrixT_<T>::const_reference MatrixT_<T>::operator()(size_type i,
        size_type j) const {
    ASSERT(i >= 0 && i < m_iLen);
    ASSERT(j >= 0 && j < m_jLen);
    return m_mp[i][j];
}

template<typename T>
T& MatrixT_<T>::at(St i, St j) {
    ASSERT(i >= 0 && i < m_iLen);
    ASSERT(j >= 0 && j < m_jLen);
    return m_mp[i][j];
}

template<typename T>
const T& MatrixT_<T>::at(St i, St j) const {
    ASSERT(i >= 0 && i < m_iLen);
    ASSERT(j >= 0 && j < m_jLen);
    return m_mp[i][j];
}

//template<typename T>
//T* MatrixT_<T>::getpValue(St i, St j) {
//    ASSERT(i >= 0 && i < m_iLen);
//    ASSERT(j >= 0 && j < m_jLen);
//    return &m_mp[i][j];
//}

template<typename T>
void MatrixT_<T>::set(St i, St j, const T& value) {
    ASSERT(i >= 0 && i < m_iLen);
    ASSERT(j >= 0 && j < m_jLen);
    m_mp[i][j] = value;
}

template<typename T>
void MatrixT_<T>::set_row(St i, const T& value) {
    ASSERT(i >= 0 && i < m_iLen);
    for (size_type j = 0; j < m_jLen; j++) {
        m_mp[i][j] = value;
    }
}

template<typename T>
void MatrixT_<T>::set_col(St j, const T& value) {
    ASSERT(j >= 0 && j < m_jLen);
    for (size_type i = 0; i < m_iLen; i++) {
        m_mp[i][j] = value;
    }
}

template<typename T>
void MatrixT_<T>::assign(const T& value) {
    for (size_type i = 0; i < m_iLen; i++) {
        for (size_type j = 0; j < m_jLen; j++) {
            m_mp[i][j] = value;
        }
    }
}

template<typename T>
void MatrixT_<T>::swap(St i1, St j1, St i2, St j2) {
    ASSERT(i1 >= 0 && i1 < m_iLen && i2 >= 0 && i2 < m_iLen);
    ASSERT(j1 >= 0 && j1 < m_jLen && j2 >= 0 && j2 < m_jLen);
    if (i1 == i2 && j1 == j2) {
        return;
    }
    T tmp;
    tmp = m_mp[i1][j1];
    m_mp[i1][j1] = m_mp[i2][j2];
    m_mp[i2][j2] = tmp;
}

template<typename T>
St MatrixT_<T>::count_equal(T value) {
    size_type num = 0;
    for (size_type i = 0; i < m_iLen; i++) {
        for (size_type j = 0; j < m_jLen; j++) {
            if (m_mp[i][j] == value) {
                num++;
            }
        }
    }
    return num;
}
template<typename T>
void MatrixT_<T>::append_row(const ArrayListT_<T> &a) {
    ASSERT(a.size()==m_jLen||m_mp==NULL);
    if (m_mp == NULL) {
        m_iLen = 1;
        m_jLen = a.size();
        m_mp = new ArrayListT_<T> [m_iLen];
        for (size_type i = 0; i < m_iLen; i++) {
            m_mp[i].reconstruct(m_jLen);
        }
        for (size_type j = 0; j < m_jLen; j++) {
            m_mp[0][j] = a[j];
        }
    } else {
        m_iLen += 1;
        ArrayListT_<T> *tmp = new ArrayListT_<T> [m_iLen];
        for (size_type i = 0; i < m_iLen; i++) {
            tmp[i].reconstruct(m_jLen);
        }
        for (size_type i = 0; i < m_iLen - 1; i++) {
            for (size_type j = 0; j < m_jLen; j++) {
                tmp[i][j] = m_mp[i][j];
            }
        }
        for (size_type j = 0; j < m_jLen; j++) {
            tmp[m_iLen - 1][j] = a[j];
        }
        ArrayListT_<T> *tmp2 = m_mp;
        m_mp = tmp;
        tmp = tmp2;
        delete[] tmp;
    }
}
template<typename T>
void MatrixT_<T>::appendCol(const ArrayListT_<T> &a) {
    ASSERT(a.size()==m_iLen||m_mp==NULL);
    if (m_mp == NULL) {
        m_iLen = a.size();
        m_jLen = 1;
        m_mp = new ArrayListT_<T> [m_iLen];
        for (size_type i = 0; i < m_iLen; i++) {
            m_mp[i].reconstruct(1);
        }
        for (size_type i = 0; i < m_iLen; i++) {
            m_mp[i][0] = a[i];
        }
    } else {
        m_jLen += 1;
        ArrayListT_<T> *tmp = new ArrayListT_<T> [m_iLen];
        for (size_type i = 0; i < m_iLen; i++) {
            tmp[i].reconstruct(m_jLen);
        }
        for (size_type i = 0; i < m_iLen; i++) {
            for (size_type j = 0; j < m_jLen - 1; j++) {
                tmp[i][j] = m_mp[i][j];
            }
        }
        for (size_type i = 0; i < m_iLen; i++) {
            tmp[i][m_jLen - 1] = a[i];
        }
        ArrayListT_<T> *tmp2 = m_mp;
        m_mp = tmp;
        tmp = tmp2;
        delete[] tmp;
    }
}
template<typename T>
void MatrixT_<T>::insert_row_back(MatrixT_<T>::size_type ii,
        const ArrayListT_<T>& a) {
    ASSERT(ii >= 0 && ii < m_iLen);
    ASSERT(a.Len() == m_jLen);
    m_iLen += 1;
    ArrayListT_<T> *tmp = new ArrayListT_<T> [m_iLen];
    for (size_type i = 0; i < m_iLen; i++) {
        tmp[i].reconstruct(m_jLen);
    }
    for (size_type i = 0; i <= ii; i++) {
        for (size_type j = 0; j < m_jLen; j++) {
            tmp[i][j] = m_mp[i][j];
        }
    }
    for (size_type j = 0; j < m_jLen; j++) {
        tmp[ii + 1][j] = a[j];
    }
    for (size_type i = ii + 2; i < m_iLen; i++) {
        for (size_type j = 0; j < m_jLen; j++) {
            tmp[i][j] = m_mp[i - 1][j];
        }
    }
    ArrayListT_<T> *tmp2 = m_mp;
    m_mp = tmp;
    tmp = tmp2;
    delete[] tmp;
}
template<typename T>
void MatrixT_<T>::insert_col_back(MatrixT_<T>::size_type jj,
        const ArrayListT_<T>& a) {
    ASSERT(jj >= 0 && jj < m_jLen);
    ASSERT(a.Len() == m_iLen);
    m_jLen += 1;
    ArrayListT_<T> *tmp = new ArrayListT_<T> [m_iLen];
    for (size_type i = 0; i < m_iLen; i++) {
        tmp[i].reconstruct(m_jLen);
    }
    for (size_type i = 0; i < m_iLen; i++) {
        for (size_type j = 0; j <= jj; j++) {
            tmp[i][j] = m_mp[i][j];
        }
    }
    for (size_type i = 0; i < m_iLen; i++) {
        tmp[i][jj + 1] = a[i];
    }
    for (size_type i = 0; i < m_iLen; i++) {
        for (size_type j = jj + 2; j < m_jLen; j++) {
            tmp[i][j] = m_mp[i][j - 1];
        }
    }
    ArrayListT_<T> *tmp2 = m_mp;
    m_mp = tmp;
    tmp = tmp2;
    delete[] tmp;
}
template<typename T>
void MatrixT_<T>::delete_row(MatrixT_<T>::size_type ii) {
    ASSERT(ii >= 0 && ii < m_iLen);
    m_iLen -= 1;
    if (m_iLen == 0) {
        m_iLen = 0;
        m_jLen = 0;
        delete[] m_mp;
        m_mp = NULL;
    } else {
        ArrayListT_<T> *tmp = new ArrayListT_<T> [m_iLen];
        for (size_type i = 0; i < m_iLen; i++) {
            tmp[i].reconstruct(m_jLen);
        }
        for (size_type i = 0; i < ii; i++) {
            for (size_type j = 0; j < m_jLen; j++) {
                tmp[i][j] = m_mp[i][j];
            }
        }
        for (size_type i = ii + 1; i < m_iLen + 1; i++) {
            for (size_type j = 0; j < m_jLen; j++) {
                tmp[i - 1][j] = m_mp[i][j];
            }
        }
        ArrayListT_<T> *tmp2 = m_mp;
        m_mp = tmp;
        tmp = tmp2;
        delete[] tmp;
    }
}
template<typename T>
void MatrixT_<T>::delete_col(MatrixT_<T>::size_type jj) {
    ASSERT(jj >= 0 && jj < m_iLen);
    m_jLen -= 1;
    if (m_jLen == 0) {
        m_iLen = 0;
        m_jLen = 0;
        delete[] m_mp;
        m_mp = NULL;
    } else {
        ArrayListT_<T> *tmp = new ArrayListT_<T> [m_iLen];
        for (size_type i = 0; i < m_iLen; i++) {
            tmp[i].reconstruct(m_jLen);
        }
        for (size_type i = 0; i < m_iLen; i++) {
            for (size_type j = 0; j < jj; j++) {
                tmp[i][j] = m_mp[i][j];
            }
        }
        for (size_type i = 0; i < m_iLen; i++) {
            for (size_type j = jj + 1; j < m_jLen + 1; j++) {
                tmp[i][j - 1] = m_mp[i][j];
            }
        }
        ArrayListT_<T> *tmp2 = m_mp;
        m_mp = tmp;
        tmp = tmp2;
        delete[] tmp;
    }
}

//end of class MatrixT_===========================
//===============================================
//===============================================

template<typename T>
class MatrixV_: public MatrixT_<T> {
public:
    // type definitions===================
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef St size_type;
    typedef St difference_type;

    typedef MatrixT_<T> Base;
    typedef MatrixV_<T> Self;
    typedef Self&       ref_Self;
    //constructor==========================
    MatrixV_();
    MatrixV_(St iLen, St jLen);
    MatrixV_(St iLen, St jLen, St n, T *value);
    //void reconstruct(St size_i, St size_j);
    //~MatrixV_();
    //=============================================
    ref_Self operator+=(const T& a) {
        for (St i = 0; i < this->m_iLen; i++) {
            for (St j = 0; j < this->m_jLen; j++) {
                this->m_mp[i][j] += a;
            }
        }
        return *this;
    }
    ref_Self operator-=(const T& a) {
        for (St i = 0; i < this->m_iLen; i++) {
            for (St j = 0; j < this->m_jLen; j++) {
                this->m_mp[i][j] -= a;
            }
        }
        return *this;
    }
    ref_Self operator*=(const T& a){
        for (St i = 0; i < this->m_iLen; i++) {
            for (St j = 0; j < this->m_jLen; j++) {
                this->m_mp[i][j] *= a;
            }
        }
        return *this;
    }
    ref_Self operator/=(const T& a) {
        for (St i = 0; i < this->m_iLen; i++) {
            for (St j = 0; j < this->m_jLen; j++) {
                this->m_mp[i][j] /= a;
            }
        }
        return *this;
    }
    ref_Self operator+=(const Self& a) {
        ASSERT(a.size_i() == this->size_i());
        ASSERT(a.size_j() == this->size_j());
        for (St i = 0; i < this->m_iLen; i++) {
            for (St j = 0; j < this->m_jLen; j++) {
                this->m_mp[i][j] += a[i][j];
            }
        }
        return *this;
    }
    ref_Self operator-=(const Self& a) {
        ASSERT(a.size_i() == this->size_i());
        ASSERT(a.size_j() == this->size_j());
        for (St i = 0; i < this->m_iLen; i++) {
            for (St j = 0; j < this->m_jLen; j++) {
                this->m_mp[i][j] -= a[i][j];
            }
        }
        return *this;
    }
    ref_Self operator*=(const Self& a) {
        ASSERT(a.size_i() == this->size_i());
        ASSERT(a.size_j() == this->size_j());
        for (St i = 0; i < this->m_iLen; i++) {
            for (St j = 0; j < this->m_jLen; j++) {
                this->m_mp[i][j] *= a[i][j];
            }
        }
        return *this;
    }
    ref_Self operator/=(const Self& a) {
        ASSERT(a.size_i() == this->size_i());
        ASSERT(a.size_j() == this->size_j());
        for (St i = 0; i < this->m_iLen; i++) {
            for (St j = 0; j < this->m_jLen; j++) {
                this->m_mp[i][j] /= a[i][j];
            }
        }
        return *this;
    }
    Self operator-() const;
    Self operator+(const Self &a);
    Self operator-(const Self &a);
    Self operator*(const Self &a);
    Self operator*(const T &a);
    Self operator/(const Self &a);
    Self operator/(const T &a);

    //show ========================================
    void show() const;

};

template<typename T>
MatrixV_<T>::MatrixV_() :
        MatrixT_<T>() {
}

template<typename T>
MatrixV_<T>::MatrixV_(St iLen, St jLen) :
        MatrixT_<T>(iLen, jLen) {
    this->assign(0);
}

template<typename T>
MatrixV_<T>::MatrixV_(St iLen, St jLen, St n, T *value) :
        MatrixT_<T>(iLen, jLen) {
    // if n > i * j, the rest number will be dropped.
    // if n < i * j, the rest number will be zero.
    St i1d = 0;
    for (St j = 0; j < this->m_jLen; j++) {
        for (St i = 0; i < this->m_iLen; i++) {
            if (i1d >= n) {
                this->m_mp[i][j] = 0.0;
            } else {
                this->m_mp[i][j] = value[i1d];
            }
            i1d++;
        }
    }
}

template<typename T>
MatrixV_<T> MatrixV_<T>::operator+(const MatrixV_<T> &a) {
    ASSERT(a.size_i() == this->size_i());
    ASSERT(a.size_j() == this->size_j());
    MatrixV_<T> sum(this->m_iLen, this->m_jLen);
#ifdef OPENMP
#pragma omp parallel if (a.size_i() > 100)
    {
#pragma omp for schedule(dynamic)
#endif
    for (St i = 0; i < this->m_iLen; i++) {
        for (St j = 0; j < this->m_jLen; j++) {
            sum[i][j] = this->m_mp[i][j] + a[i][j];
        }
    }
#ifdef OPENMP
    }
#endif
    return sum;
}
template<typename T>
MatrixV_<T> MatrixV_<T>::operator-() const{
    MatrixV_<T> res(this->m_iLen, this->m_jLen);
#ifdef OPENMP
#pragma omp parallel if (this->size_i() > 100)
    {
#pragma omp for schedule(dynamic)
#endif
    for (St i = 0; i < this->m_iLen; i++) {
        for (St j = 0; j < this->m_jLen; j++) {
            res[i][j] = -(this->m_mp[i][j]);
        }
    }
#ifdef OPENMP
    }
#endif
    return res;
}
template<typename T>
MatrixV_<T> MatrixV_<T>::operator-(const MatrixV_<T> &a) {
    ASSERT(a.size_i() == this->size_i());
    ASSERT(a.size_j() == this->size_j());
    MatrixV_<T> sum(this->m_iLen, this->m_jLen);
#ifdef OPENMP
#pragma omp parallel if (a.size_i() > 100)
    {
#pragma omp for schedule(dynamic)
#endif

    for (St i = 0; i < this->m_iLen; i++) {
        for (St j = 0; j < this->m_jLen; j++) {
            sum[i][j] = this->m_mp[i][j] - a[i][j];
        }
    }
#ifdef OPENMP
    }
#endif
    return sum;
}

template<typename T>
MatrixV_<T> MatrixV_<T>::operator*(const MatrixV_<T> &a) {
    ASSERT(a.size_i() == this->size_j());
    St nrow = this->m_iLen;
    St ncol = a.size_j();
    MatrixV_<T> res(nrow, ncol);
    for (St i = 0; i < nrow; i++) {
        for (St j = 0; j < ncol; j++) {
            res[i][j] = this->m_mp[i][j] * a[i][j];
        }
    }
    return res;
}

//template<typename T>
//MatrixV_<T> MatrixV_<T>::operator*(const MatrixV_<T> &a) {
//    ASSERT(a.size_i() == this->size_j());
//    St nrow = this->m_iLen;
//    St ncol = a.size_j();
//    MatrixV_<T> res(nrow, ncol);
//    for (St i = 0; i < nrow; i++) {
//        for (St j = 0; j < ncol; j++) {
//            res[i][j] = 0;
//            for (St k = 0; k < this->m_jLen; k++) {
//                res[i][j] += this->m_mp[i][k] * a[k][j];
//            }
//        }
//    }
//    return res;
//}
template<typename T>
MatrixV_<T> MatrixV_<T>::operator*(const T& a) {
    St nrow = this->m_iLen;
    St ncol = this->m_jLen;
    MatrixV_<T> res(nrow, ncol);
    for (St i = 0; i < nrow; i++) {
        for (St j = 0; j < ncol; j++) {
            res[i][j] = this->m_mp[i][j] * a;
        }
    }
    return res;
}
template<typename T>
void MatrixV_<T>::show() const {
    std::cout << "> Matrix " << this->m_iLen << " x " << this->m_jLen << "\n";
    std::cout << "> ";
    for (int i = 0; i < this->m_iLen; i++) {
        for (int j = 0; j < this->m_jLen; j++) {
            std::cout << std::scientific << this->m_mp[i][j] << "  ";
        }
        std::cout << std::endl;
        std::cout << "> ";
    }
    std::cout << "< ----------\n";
}

//===============================================
//Function outside of the class==================
//===============================================
template<typename T>
ArrayListV_<T> operator*(const MatrixV_<T> &m, const ArrayListV_<T> &a) {
    ASSERT(m.size_j() == a.size());
    ArrayListV_<T> res(a.size());
    for (int i = 0; i < m.size_i(); i++) {
        for (int j = 0; j < m.size_j(); j++) {
            res[i] += m[i][j] * a[j];
        }
    }
    return res;
}

//typedef MatrixV_<Float> Matrix;



}    //namespace=====================================

#endif /* MATRIXT_H_ */
