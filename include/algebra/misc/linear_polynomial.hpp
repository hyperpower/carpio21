#ifndef _LINEAR_POLYNOMIAL_H_
#define _LINEAR_POLYNOMIAL_H_

#include <type_define.hpp>
#include <utility>
#include <map>


namespace carpio {

template<class T>
struct IsZero_: std::unary_function<const T&, bool> {
	bool operator()(const T& number) const {
		return IsCloseToZero(number);
	}
};

template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >                                 //
class LinearPolynomial_ : public std::map<TERM, COE, COMPARE_TERM>{
public:
	typedef std::map<TERM, COE, COMPARE_TERM> Base;
	typedef LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> Self;
	typedef Self& ref_Self;

	typedef typename Base::iterator iterator;

	typedef COE     Coe;
	typedef TERM    Term;
	typedef IS_ZERO IsZero;
protected:
	COE    _num;
	IsZero _is_zero;
public:
	LinearPolynomial_() : Base(), _num(0) {
	}

	LinearPolynomial_(const COE& c): Base(), _num(c){
	}

	LinearPolynomial_(const TERM& t): Base(), _num(0.0){
		this->operator [](t) = 1.0;
	}

	LinearPolynomial_(const Self& other) : Base(other), _num(other._num) {
	}

	Coe value() const{
		return _num;
	}

	Self& operator=(const Self& other){
		this->Base::operator =(other);
		_num = other._num;
		return *this;
	}

	Self& operator=(const Coe& coe){
		this->clear();
		_num = coe;
		return *this;
	}

	Self operator-() const{
		Self res;
		for (auto& i : *this) {
			res[i.first] = -(i.second);
		}
		res._num = -(this->_num);
		return res;
	}

	ref_Self operator+=(const Coe& a){
		this->_num += a;
		return *this;
	}
	ref_Self operator+=(const Term& t) {
		if (this->find(t) != this->end()) {
			// has the item;
			this->operator [](t) += 1;
		} else {
			this->operator [](t) = 1;
		}
		return *this;
	}
	ref_Self operator+=(const Self& a){
		for (auto& ia : a){
			if(this->find(ia.first)!= this->end()){
				// has the item;
				this->operator [](ia.first) += ia.second;
			}else{
				this->operator [](ia.first) = ia.second;
			}
		}
		this->_num += a._num;
		this->_trim_zero();
		return *this;
	}
	ref_Self operator-=(const Coe& a){
		this->_num -= a;
		return *this;
	}
	ref_Self operator-=(const Term& t) {
		if (this->find(t) != this->end()) {
			// has the item;
			this->operator [](t) -= 1;
		} else {
			this->operator [](t) = -1;
		}
		return *this;
	}
	ref_Self operator*=(const Coe& a){
		for(auto iter = this->begin(); iter!=this->end(); ++iter){
			iter->second *=a;
		}
		this->_num *=a;
		this->_trim_zero();
		return *this;
	}
	ref_Self operator/=(const Coe& a){
		// a is not zero;
		for (auto iter = this->begin(); iter != this->end(); ++iter) {
			iter->second /= a;
		}
		this->_num /= a;
		return *this;
	}

	ref_Self operator-=(const Self& a) {
		for (auto& ia : a) {
			if (this->find(ia.first) != this->end()) {
				// has the item;
				this->operator [](ia.first) -= ia.second;
			} else {
				this->operator [](ia.first) = -(ia.second);
			}
		}
		this->_num -= a._num;
		this->_trim_zero();
		return *this;
	}


protected:
	void _trim_zero(){
		auto it_b = this->begin();
		auto it_e = this->end();
		iterator it_t;

		while (it_b != it_e) {
			if (_is_zero(it_b->second)) {  // Criteria checking here
				it_t = it_b;            // Keep a reference to the iter
				++it_b;                 // Advance in the map
				this->erase(it_t);       // Erase it !!!
			} else {
				++it_b;                 // Just move on ...
			}
		}
	}
};

template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator+(		LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> x,
		  const LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> &y){
	x += y;
	return x;
}
template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator+(      LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> x,
		  const COE& a){
	x +=a;
	return x;
}
template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator+(const COE& a,
		        LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> x
		  ){
	x +=a;
	return x;
}

template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator+(      LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> x,
		  const TERM& a){
	x +=a;
	return x;
}
template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator+(const TERM& a,
		        LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> x
		  ){
	x +=a;
	return x;
}


template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator-(		LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> x,
		  const LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> &y){
	x -= y;
	return x;
}
template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator-(      LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> x,
		  const COE& a){
	x -=a;
	return x;
}
template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator-(const COE& a,
		  const LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>& x
		  ){
	auto mx = -x;
	mx +=a;
	return mx;
}
template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator-(      LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> x,
		  const TERM& a){
	x -=a;
	return x;
}
template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator-(const TERM& a,
		  const LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>& x
		  ){
	auto mx = -x;
	mx +=a;
	return mx;
}

template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator*(      LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> x,
		  const COE& a){
	x *=a;
	return x;
}
template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator*(const COE& a,
		        LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> x
		  ){
	x *=a;
	return x;
}

template<class COE, class TERM,
		 class IS_ZERO = IsZero_<COE>,
         class COMPARE_TERM = std::less<TERM> >
LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM>
operator/(LinearPolynomial_<COE, TERM, IS_ZERO, COMPARE_TERM> x,
		  const COE& a){
	IS_ZERO iz;
	ASSERT((!(iz(a))));
	x /= a;
	return x;
}



template<class COE, class TERM,
         class COMPARE_TERM = std::less<TERM> >
std::ostream& operator<<(
		std::ostream& stream,
		const LinearPolynomial_<COE, TERM, COMPARE_TERM>& p) {
	for (auto& t : p) {
		stream << t.second << " : ";
		stream << t.first  << "\n";
	}
	stream << p.value();
	stream << " <<--";
	return stream;
}



}

#endif
