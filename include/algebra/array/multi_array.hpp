/************************
 //  \file   multi_array.hpp
 //  \brief
 //
 //  \author czhou
 //  \date   20 janv. 2018
 ***********************/
#ifndef MULTI_ARRAY_H_
#define MULTI_ARRAY_H_

#include <iostream>
#include <assert.h>

#include "algebra/algebra_define.hpp"
#include <array>
#include "array_list.hpp"

namespace carpio {

template<typename T, St DIM>
class MultiArray_ {
  public:
    static const St Dim = DIM;
    // type definitions===================
    typedef T value_t;
    typedef MultiArray_<value_t, Dim> Self;
    typedef MultiArray_<value_t, Dim>* pSelf;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef St size_type;
    typedef St difference_type;

    typedef typename ArrayListT_<value_t>::iterator iterator;
    typedef typename ArrayListT_<value_t>::const_iterator const_iterator;
  private:
    std::array<St, Dim> m_len;
    ArrayListT_<value_t> m_array;
  public:
    //constructor==========================
    MultiArray_(){
    	m_len.fill(0);
    }
    MultiArray_(const Self& a){
    	this->m_len = a.m_len;
    	this->m_array = a.m_array;
    }
    MultiArray_(St iLen, St jLen= 0, St kLen= 0){
    	St len         = iLen;
		this->m_len[0] = iLen;
		if (Dim >= 2) {
			ASSERT(iLen > 0 && jLen > 0);
			this->m_len[1] = jLen;
			len *= jLen;
		}
		if (Dim == 3) {
			ASSERT(iLen > 0 && jLen > 0 && kLen > 0);
			this->m_len[2] = kLen;
			len *= kLen;
		}
		this->m_array.reconstruct(len);
    }

    void reconstruct(St iLen, St jLen = 0, St kLen= 0){
		St len         = iLen;
		this->m_len[0] = iLen;
		if (Dim >= 2) {
			ASSERT(iLen > 0 && jLen > 0);
			this->m_len[1] = jLen;
			len *= jLen;
		}
		if (Dim == 3) {
			ASSERT(iLen > 0 && jLen > 0 && kLen > 0);
			this->m_len[2] = kLen;
			len *= kLen;
		}
		this->m_array.reconstruct(len);
    }
    //=============================================
    MultiArray_<T, DIM>& operator=(const MultiArray_<T, DIM>& a){
		if (this == &a) {
			return *this;
		}
		this->m_len = a.m_len;
		this->m_array  = a.m_array;
		return *this;
    }
    //=============================================
    ~MultiArray_() {
    }

    //Capacity=====================================
    St size() const {
        return m_array.size();
    }
    St size_i() const {
        return m_len[0];
    }
    St size_j() const {
        return Dim >= 2 ? m_len[1] : 0;
    }
    St size_k() const {
        return Dim >= 3 ? m_len[2] : 0;
    }
    bool empty() const {
        return m_array.empty();
    }
    /*
     *  iterator
     */
    iterator       begin() {
        return m_array.begin();
    }
    const_iterator begin() const {
        return m_array.begin();
    }
    iterator       end() {
        return m_array.end();
    }
    const_iterator end() const {
        return m_array.end();
    }
    //Element access===============================
    St to_1d_idx(St i, St j = 0, St k = 0) const{
		ASSERT(i < this->m_len[0]);
		if (Dim >= 2)
			ASSERT(j < this->m_len[1]);
		if (Dim >= 3)
			ASSERT(k < this->m_len[2]);
		std::array<St, Dim> inp;
		inp[0] = i;
		if (Dim >= 2) {
			inp[1] = j;
		}
		if (Dim >= 3) {
			inp[2] = k;
		}
		St idx = 0;
		for (St ii = 0; ii < Dim; ii++) {
			St b = 1;
			for (St jj = ii + 1; jj < Dim; jj++) {
				b *= m_len[jj];
			}
			idx += b * inp[ii];
		}
		return idx;
    }



    reference       at(St i, St j= 0, St k= 0){
    	 St idx = to_1d_idx(i, j, k);
    	 return m_array[idx];
    }
    const_reference at(St i, St j = 0, St k = 0) const{
    	St idx = to_1d_idx(i, j, k);
    	return m_array[idx];
    }

    reference       operator()(St i, St j = 0, St k = 0){
       	return at(i, j, k);
    }
    const_reference operator()(St i, St j = 0, St k = 0) const{
      	return at(i, j, k);
    }

    reference       at_1d(St i){        return m_array[i];}
    const_reference at_1d(St i) const{  return m_array[i];}

    T    get(St i, St j = 0,  St k = 0){
    	return at(i, j, k);
    }
    void set(const T& value, St i, St j = 0, St k = 0){
    	this->at(i, j, k) = value;
    }

    void assign(const T& value){
    	 m_array.assign(value);
    }

    //element access===============================

//    T* getpValue(St i, St = 0, St = 0);  //not good

    inline bool check_idx(St dim, St idx) const {
        ASSERT(dim < Dim);
        if (idx >= 0 && idx < m_len[dim]) {
            return true;
        } else {
            return false;
        }
    }
    inline bool check_idx_ijk(St i, St j, St k) const {
        return check_idx(0, i) && ((Dim >= 2) ? check_idx(1, j) : true)
               && ((Dim >= 3) ? check_idx(2, k) : true);
    }

    inline St count_equal(const T& nd) const { //overload ==
        return m_array.count_equal(nd);
    }
};

template<typename T, St DIM>
class MultiArrayV_ {
  public:
    static const St Dim = DIM;
    // type definitions===================
    typedef T value_t;
    typedef MultiArrayV_<value_t, Dim>                  Self;
    typedef MultiArrayV_<value_t, Dim>&             ref_Self;
    typedef const MultiArrayV_<value_t, Dim>& const_ref_Self;
    typedef MultiArrayV_<value_t, Dim>*       pSelf;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef St size_type;
    typedef St difference_type;

    typedef typename ArrayListT_<value_t>::iterator iterator;
    typedef typename ArrayListT_<value_t>::const_iterator const_iterator;
  private:
    std::array<St, Dim> m_len;
    ArrayListV_<value_t> m_array;
  public:
    //constructor==========================
    MultiArrayV_(){
    	m_len.fill(0);
    }
    MultiArrayV_(const Self& a):
    	m_len(a.m_len), m_array(a.m_array){
    }
    MultiArrayV_(St iLen, St jLen= 0, St kLen= 0){
    	St len         = iLen;
		this->m_len[0] = iLen;
		if (Dim >= 2) {
			ASSERT(iLen > 0 && jLen > 0);
			this->m_len[1] = jLen;
			len *= jLen;
		}
		if (Dim >= 3) {
			ASSERT(iLen > 0 && jLen > 0 && kLen > 0);
			this->m_len[2] = kLen;
			len *= kLen;
		}
		this->m_array.reconstruct(len);
    }

    void reconstruct(St iLen, St jLen = 0, St kLen= 0){
		St Len = 0;
		this->m_len[0] = iLen;
		if (Dim == 1) {
			Len = iLen;
		}
		if (Dim >= 2) {
			ASSERT(iLen > 0 && jLen > 0);
			this->m_len[1] = jLen;
			Len = iLen * jLen;
		}
		if (Dim >= 3) {
			ASSERT(iLen > 0 && jLen > 0 && kLen > 0);
			this->m_len[2] = kLen;
			Len = iLen * jLen * kLen;
		}
		this->m_array.reconstruct(Len);
    }
    //=============================================
    ref_Self operator=(const Self& a){
		if (this == &a) {
			return *this;
		}
		this->m_len = a.m_len;
		this->m_array = a.m_array;
		return *this;
    }
    //=============================================
    ~MultiArrayV_() {
    }

    //Capacity=====================================
    St size() const {
        return m_array.size();
    }
    St size_i() const {
        return m_len[0];
    }
    St size_j() const {
        return Dim >= 2 ? m_len[1] : 0;
    }
    St size_k() const {
        return Dim >= 3 ? m_len[2] : 0;
    }
    bool empty() const {
        return m_array.empty();
    }
    bool is_compatible(const Self& other) const{
    	for(St i = 0; i< Dim;++i){
    		if(m_len[i] != other.m_len[i]){
    			return false;
    		}
    	}
    	return true;
    }
    /*
     *  iterator
     */
    iterator       begin() {
        return m_array.begin();
    }
    const_iterator begin() const {
        return m_array.begin();
    }
    iterator       end() {
        return m_array.end();
    }
    const_iterator end() const {
        return m_array.end();
    }
    //Element access===============================
    St to_1d_idx(St i, St j = 0, St k = 0) const{
		ASSERT(i < this->m_len[0]);
		if (Dim >= 2)
			ASSERT(j < this->m_len[1]);
		if (Dim >= 3)
			ASSERT(k < this->m_len[2]);
		std::array<St, Dim> inp;
		inp[0] = i;
		if (Dim >= 2) {
			inp[1] = j;
		}
		if (Dim >= 3) {
			inp[2] = k;
		}
		St idx = 0;
		for (St ii = 0; ii < Dim; ii++) {
			St b = 1;
			for (St jj = ii + 1; jj < Dim; jj++) {
				b *= m_len[jj];
			}
			idx += b * inp[ii];
		}
		return idx;
    }



    reference       at(const St& i,
    		           const St& j= 0,
					   const St& k= 0){
    	 St idx = to_1d_idx(i, j, k);
    	 return m_array[idx];
    }
    const_reference at(const St& i,
    		           const St& j = 0,
					   const St& k = 0) const{
    	St idx = to_1d_idx(i, j, k);
    	return m_array[idx];
    }

    reference       operator()(const St& i,
    		                   const St& j = 0,
							   const St& k = 0){
       	return at(i, j, k);
    }
    const_reference operator()(const St i,
    		                   const St j = 0,
							   const St k = 0) const{
      	return at(i, j, k);
    }

    reference       at_1d(St i){        return m_array[i];}
    const_reference at_1d(St i) const{  return m_array[i];}

    T    get(St i, St j = 0,  St k = 0){
    	return at(i, j, k);
    }
    void set(const T& value, St i, St j = 0, St k = 0){
    	this->at(i, j, k) = value;
    }

    void assign(const T& value){
    	 m_array.assign(value);
    }


	T max() const {
		return this->m_array.max();
	}

	T min() const {
		return this->m_array.min();
	}

	void abs(){
		this->m_array.abs();
	}

	T norm1() const{
		return this->m_array.norm1();
	}
	T norm2() const{
		return this->m_array.norm2();
	}
	T norminf() const{
		return this->m_array.norminf();
	}

    //element access===============================

    inline bool check_idx(St dim, St idx) const {
        ASSERT(dim < Dim);
        if (idx >= 0 && idx < m_len[dim]) {
            return true;
        } else {
            return false;
        }
    }
    inline bool check_idx_ijk(St i, St j, St k) const {
        return check_idx(0, i) && ((Dim >= 2) ? check_idx(1, j) : true)
               && ((Dim >= 3) ? check_idx(2, k) : true);
    }

    inline St count_equal(const T& nd) const { //overload ==
        return m_array.count_equal(nd);
    }
    // operator ====================================
    Self operator-(){
    	Self res(*this);
    	res.m_array = -res.m_array;
    	return res;
    }
	ref_Self operator+=(const Self& a) {
		ASSERT(this->is_compatible(a));
		m_array += a.m_array;
		return *this;
	}
	ref_Self operator-=(const Self& a) {
		ASSERT(this->is_compatible(a));
		m_array -= a.m_array;
		return *this;
	}
	ref_Self operator*=(const Self& a) {
		ASSERT(this->is_compatible(a));
		m_array *= a.m_array;
		return *this;
	}
	ref_Self operator/=(const Self& a) {
		ASSERT(this->is_compatible(a));
		m_array /= a.m_array;
		return *this;
	}
	ref_Self operator+=(const Vt& a) {
		m_array += a;
		return *this;
	}
	ref_Self operator-=(const Vt& a) {
		m_array -= a;
		return *this;
	}
	ref_Self operator*=(const Vt& a) {
		m_array *= a;
		return *this;
	}
	ref_Self operator/=(const Vt& a) {
		m_array /= a;
		return *this;
	}
};
template<typename T, St DIM>
MultiArrayV_<T, DIM> Abs(const MultiArrayV_<T, DIM> a){
	MultiArrayV_<T, DIM> res(a);

}
template<typename V, St DIM>
MultiArrayV_<V, DIM> operator+(      MultiArrayV_<V, DIM> x, 
                               const MultiArrayV_<V, DIM> &y){
	ASSERT(x.size() == y.size());
	x += y;
	return x;
}
template<typename V, St DIM>
MultiArrayV_<V, DIM> operator+(MultiArrayV_<V, DIM> x, const V &a){
	x += a;
	return x;
}
template<typename V, St DIM>
MultiArrayV_<V, DIM> operator+(const V &a, MultiArrayV_<V, DIM> x){
	x += a;
	return x;
}
template<typename V, St DIM>
MultiArrayV_<V, DIM> operator-(      MultiArrayV_<V, DIM> x, 
                               const MultiArrayV_<V, DIM> &y){
	ASSERT(x.size() == y.size());
	x -= y;
	return x;
}
template<typename V, St DIM>
MultiArrayV_<V, DIM> operator-(MultiArrayV_<V, DIM> x, const V &a){
	x -= a;
	return x;
}

}

#endif /* MULTI_ARRAY_HPP */
