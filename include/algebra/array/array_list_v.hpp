#ifndef _ARRAYLIST_V_HPP_
#define _ARRAYLIST_V_HPP_

#include <initializer_list>

#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <functional>

#include "type_define.hpp"
#include "array_list.hpp"


#undef max
#undef min

namespace carpio {

template<typename V>
class ArrayListV_: public ArrayListT_<V> {
public:
    // type definitions
	typedef ArrayListT_<V> Base;
    typedef V value_type;
    typedef V* pointer;
    typedef const V* const_pointer;
    typedef V& reference;
    typedef const V& const_reference;
    typedef St size_type;
    typedef St difference_type;
    typedef ArrayListV_<V>        Self;
    typedef ArrayListV_<V>&       ref_Self;
    typedef const ArrayListV_<V>& const_ref_Self;
    //constructor==================================
    ArrayListV_();
    ArrayListV_(size_type Len);
    ArrayListV_(size_type Len, const V& nd);
    ArrayListV_(const std::initializer_list<V>& l):Base(l){};
    void reconstruct(size_type Len);
    //arrayListV(V *nd, size_type Len);
    //opertator====================================
    ref_Self operator+=(const value_type& a){
        AddEqual(this->size(), this->m_p, a);
        return *this;
    }
    ref_Self operator-=(const value_type& a);
    ref_Self operator*=(const V&);
    ref_Self operator/=(const value_type& a);

    template<class VT2>
    ref_Self operator+=(const ArrayListV_<VT2>& a){
        // std::cout << "AL operator+= 2T" << std::endl;
        ASSERT(this->size() == a.size());
	    AddEqual(this->size(), this->m_p, a.data());
        return *this;
    }
    template<class VT2>
    ref_Self operator-=(const ArrayListV_<VT2>& a){
        ASSERT(this->size() == a.size());
	    MinusEqual(this->size(), this->m_p, a.data());
        return *this;
    }
    template<class VT2>
    ref_Self operator*=(const ArrayListV_<VT2>& a){
        ASSERT(this->size() == a.size());
	    MultiplyEqual(this->size(), this->m_p, a.data());
        return *this;
    }
    template<class VT2>
    ref_Self operator*=(const VT2& a){
	    MultiplyEqual(this->size(), this->m_p, a);
        return *this;
    }
    ref_Self operator-=(const Self&);
    // ref_Self operator*=(const ArrayListV_<V>&);
    ref_Self operator/=(const Self&);

        Self operator-() const;
        Self operator*(const Self &a);
        Self operator*(const V &a);
        Self operator/(const Self &a);
        Self operator/(const V &a);

    // other functions==============================
    void abs();
    V sum() const;
    V min() const;
    V max() const;
    V norm1() const;
    V norm2() const;
    V norminf() const;
    size_type min_idx() const;
    size_type max_idx() const;
    //fill ----------------------------------------
    void assign_forward(const V& be, const V& step);
    void assign_backward(const V& be, const V& step);
    void ones();
    void zeros();
    //count ---------------------------------------
    size_type count_equal(const V &a) const;

    void show() const;
};
// template<typename V>
// ArrayListV_<V> operator+(const ArrayListV_<V>& x, const ArrayListV_<V> &y);

template<typename V, typename V2>
ArrayListV_<V> operator+(ArrayListV_<V> x, const ArrayListV_<V2> &y);

template<typename V>
ArrayListV_<V> operator+(ArrayListV_<V> x, const V &a);
template<typename V>
ArrayListV_<V> operator+(const V &a, ArrayListV_<V> x);

template<typename V>
ArrayListV_<V> operator-(const ArrayListV_<V>& x, const ArrayListV_<V> &y);
template<typename V>
ArrayListV_<V> operator-(ArrayListV_<V> x, const V &a);
template<typename V>
ArrayListV_<V> operator-(const V &a, const ArrayListV_<V>& x);

template<typename V>
ArrayListV_<V> operator*(const V &a, const ArrayListV_<V> &x);
template<typename V>
ArrayListV_<V> operator*(const ArrayListV_<V> &x, const V &a);
//=================================================
/*
 * Return evenly spaced numbers over a specified interval.
 * Returns num evenly spaced samples, calculated over the interval [start, stop].
 */
template<typename V>
ArrayListV_<V> Linspace(V start, V stop, St n) {
    if (n == 0) {
        return ArrayListV_<V>();
    }
    ArrayListV_<V> arr(n + 1);
    V d = (stop - start) / n;
    for (St i = 0; i < n + 1; ++i) {
        arr[i] = start + d * i;
    }
    return arr;
}

template<typename V>
ArrayListV_<V>::ArrayListV_() :
    ArrayListT_<V>() {
}
template<typename V>
ArrayListV_<V>::ArrayListV_(size_type Len) :
    ArrayListT_<V>(Len) {
    this->assign(V(0));
}
template<typename V>
ArrayListV_<V>::ArrayListV_(size_type Len, const V& nd) :
    ArrayListT_<V>(Len, nd) {
}


template<typename V>
void ArrayListV_<V>::reconstruct(size_type Len) {
    if (Len == 0 && this->m_Len == 0) {
        return;
    }
    if (Len == 0 && nullptr != this->m_p) {
        delete[] this->m_p;
        return;
    }
    ASSERT(Len > 0);
    if (nullptr != this->m_p)
        delete[] this->m_p;
    this->m_Len = Len;
    this->m_p = new V[this->m_Len];
    this->assign(V(0));
}

template<typename V>
ArrayListV_<V> operator+(const ArrayListV_<V>& x, const ArrayListV_<V> &y){
    auto n = x.size();
    #ifndef NDEBUG
	    ASSERT(n == y.size());
    #endif
    ArrayListV_<V> res(n, false); // no assign
    // auto nc = (n <= 256 ? n : 256);
    // AddUnroll256(nc, x.data(), y.data(), res.data());
    // if(n > 256){
	//     AddInterval((decltype(n))257, n, x.data(), y.data(), res.data());
    // }
    Add(n, x.data(), y.data(), res.data());
	return res;
}

template<typename V, typename V2>
ArrayListV_<V> operator+(ArrayListV_<V> x, const ArrayListV_<V2> &y){
	ASSERT(x.size() == y.size());
    x += y;
	return x;
}

template<typename V>
ArrayListV_<V> operator+(ArrayListV_<V> x, const V &a){
	x += a;
	return x;
}
template<typename V>
ArrayListV_<V> operator+(const V &a, ArrayListV_<V> x){
	x += a;
	return x;
}

template<typename V>
ArrayListV_<V> operator-(const ArrayListV_<V>& x, const ArrayListV_<V> &y){
	#ifndef NDEBUG
	    ASSERT(x.size() == y.size());
    #endif
    ArrayListV_<V> res(x.size(), false); // no assign
	Minus(x.size(), x.data(), y.data(), res.data());
	return res;
}
template<typename V>
ArrayListV_<V> operator-(ArrayListV_<V> x, const V &a){
	x -= a;
	return x;
}
template<typename V>
ArrayListV_<V> operator-(const V &a, const ArrayListV_<V>& x){
	ArrayListV_<V> res(x.size());
	res.assign(a);
	res -= x;
	return res;
}
template<typename V>
ArrayListV_<V> ArrayListV_<V>::operator-() const{
	ArrayListV_<V> tmp(this->m_Len);
    Nagative(this->m_Len, this->m_p, tmp.m_p);
	return tmp;
}

template<typename V>
ArrayListV_<V> ArrayListV_<V>::operator*(const ArrayListV_<V> &a) {
    ASSERT(a.size() == this->size());
    ArrayListV_<V> tmp(this->m_Len);
    for (size_type i = 0; i < this->m_Len; i++) {
        tmp[i] = this->m_p[i] * a[i];
    }
    return tmp;
}
template<typename V>
ArrayListV_<V> ArrayListV_<V>::operator*(const V &a) {
    ArrayListV_<V> sum(this->m_Len);
    for (size_type i = 0; i < this->m_Len; i++) {
        sum[i] = this->m_p[i] * a;
    }
    return sum;
}
template<typename V>
ArrayListV_<V> ArrayListV_<V>::operator/(const ArrayListV_<V> &a) {
    ASSERT(a.size() == this->size());
    ArrayListV_<V> tmp(this->m_Len);
    for (size_type i = 0; i < this->m_Len; i++) {
        tmp[i] = this->m_p[i] / a[i];
    }
    return tmp;
}

template<typename V>
ArrayListV_<V> ArrayListV_<V>::operator/(const V &a) {
    ArrayListV_<V> sum(this->m_Len);
    for (size_type i = 0; i < this->m_Len; i++) {
        sum[i] = this->m_p[i] / a;
    }
    return sum;
}
template<typename V>
void ArrayListV_<V>::abs(){
    for (size_type i = 0; i < this->m_Len; i++) {
        this->m_p[i] = std::abs(this->m_p[i]);
    }
}

template<typename V>
V ArrayListV_<V>::sum() const {
    V sum = 0;
    for (size_type i = 0; i < this->m_Len; i++) {
        sum += this->m_p[i];
    }
    return sum;
}
template<typename V>
V ArrayListV_<V>::min() const {
    ASSERT(this->m_p!=NULL);
    V min = this->m_p[0];
    for (size_type i = 0; i < this->m_Len; i++) {
        if (this->m_p[i] < min) {
            min = this->m_p[i];
        }
    }
    return min;
}
template<typename V>
V ArrayListV_<V>::max() const {
    ASSERT(this->m_p!=NULL);
    V max = this->m_p[0];
    for (size_type i = 0; i < this->m_Len; i++) {
        if (this->m_p[i] > max) {
            max = this->m_p[i];
        }
    }
    return max;
}
template<typename V>
V ArrayListV_<V>::norm1() const {
    ASSERT(this->m_p != nullptr);
    return Nrm1(*this);
}

template<typename V>
V ArrayListV_<V>::norm2() const {
    ASSERT(this->m_p != nullptr);
    return Nrm2(*this);
}

template<typename V>
V ArrayListV_<V>::norminf() const {
    ASSERT(this->m_p != nullptr);
    return Amax(*this);
}

template<typename V>
St ArrayListV_<V>::min_idx() const {
    ASSERT(this->m_p!=NULL);
    V min = this->m_p[0];
    size_type idx = 0;
    for (size_type i = 0; i < this->m_Len; i++) {
        if (this->m_p[i] < min) {
            min = this->m_p[i];
            idx = i;
        }
    }
    return idx;
}
template<typename V>
St ArrayListV_<V>::max_idx() const {
    ASSERT(this->m_p!=NULL);
    V max = this->m_p[0];
    size_type idx = 0;
    for (size_type i = 0; i < this->m_Len; i++) {
        if (this->m_p[i] > max) {
            max = this->m_p[i];
            idx = i;
        }
    }
    return idx;
}
template<typename V>
void ArrayListV_<V>::assign_forward(const V& be, const V& step) {
    for (size_type i = 0; i < this->m_Len; i++) {
        this->m_p[i] = be + i * step;
    }
}
template<typename V>
void ArrayListV_<V>::assign_backward(const V& be, const V& step) {
    for (size_type i = this->m_Len - 1; i != 0; --i) {
        this->m_p[i] = be + (this->m_Len - 1 - i) * step;
    }
    this->m_p[0] = be + (this->m_Len - 1) * step;
}

template<typename V>
void ArrayListV_<V>::ones() {
    this->assign_forward(1.0, 0.0);
}

template<typename V>
void ArrayListV_<V>::zeros() {
    this->assign_forward(V(0.0), V(0.0));
}

template<typename V>
St ArrayListV_<V>::count_equal(const V &a) const {
    if (this->m_p != NULL) {
        return 0;
    }
    size_type count = 0;
    for (size_type i = 0; i < this->m_Len; i++) {
        if (this->m_p[i] == a) {
            count++;
        }
    }
    return count;
}
template<typename V>
void ArrayListV_<V>::show() const {
    std::cout << "size = " << this->m_Len << "\n";
    for (int i = 0; i < this->m_Len; i++) {
    	tfm::format(std::cout, "%d %12.5f\n", i, this->m_p[i]);
    }
}
//=========================================================
template<typename V>
ArrayListV_<V>& ArrayListV_<V>::operator-=(const V &a) {
	MinusEqual(this->size(), this->m_p, a);
    return *this;
}
template<typename V>
ArrayListV_<V>& ArrayListV_<V>::operator*=(const V &a) {
	MultiplyEqual(this->size(), this->m_p, a);
    return *this;
}
template<typename V>
ArrayListV_<V>& ArrayListV_<V>::operator/=(const V &a) {
	DivideEqual(this->size(), this->m_p, a);
    return *this;
}
template<typename V>
ArrayListV_<V>& ArrayListV_<V>::operator-=(const ArrayListV_<V> &a) {
	ASSERT(this->size() == a.size());
	MinusEqual(this->size(), this->m_p, a.m_p);
    return *this;
}
template<typename V>
ArrayListV_<V>& ArrayListV_<V>::operator/=(const ArrayListV_<V> &a) {
	ASSERT(this->size() == a.size());
	DivideEqual(this->size(), this->m_p, a.m_p);
    return *this;
}
template<typename V>
ArrayListV_<V> operator*(const V& a, const ArrayListV_<V>& x) {
    ArrayListV_<V> res(x);
    res *= a;
    return res;
}
template<typename V>
ArrayListV_<V> operator*(const ArrayListV_<V>& x, const V&a) {
    ArrayListV_<V> res(x);
    res *= a;
    return res;
}

}

#endif /* ARRAYT_H_ */
