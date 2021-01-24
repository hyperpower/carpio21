#ifndef TYPEDEFINE_HPP
#define TYPEDEFINE_HPP

#include "stddef.h"
#include <assert.h>
#include <math.h>
#ifdef OPENMP
#include <omp.h>
#endif

#define ASSERT(expr) assert(expr)
#define ASSERT_MSG(expr, msg) assert((expr)&&(msg))

#define SHOULD_NOT_REACH assert((false)&&(" >! Should not reach"))
#define CAST(type, p)           ((type)p)
#define CAST_REF(type, p)       (*((type)p))
#define _IF_TRUE_RETRUN(expr)   if(expr){return;};
#define _IF_FALSE_RETRUN(expr)  if(false==(expr)){return;};
#define SMALL (1e-12)
//return code
#define _SUCCESS   0
#define _ERROR     1
#define _WARNING   2

#define _RETURN_VAL_IF_FAIL(expr,val)  {       \
		if (!(expr))                           \
			return (val);                      \
	    }

#define _RETURN_IF_FAIL(expr)  {               \
		if (!(expr))                           \
			return ;                           \
	    }

#define _PI_ (3.1415926)

namespace carpio {
// value type
typedef unsigned int  St;    //size type
typedef double        Vt;          //value type
typedef int           Int;
typedef unsigned int uInt;
typedef double        Float;
typedef void*             utPointer;
typedef const void* const_utPointer;


enum Axes {
	_X_ = 0, //
	_Y_ = 1, //
	_Z_ = 2, //
};
inline Axes ToAxes(const St& i) {
	ASSERT(i >= 0 && i < 3);
	switch (i) {
	case 0:
		return _X_;
	case 1:
		return _Y_;
	case 2:
		return _Z_;
	default:
		ASSERT_MSG(false, "Error input i");
	}
	SHOULD_NOT_REACH;
	return _X_;
}

template<typename C>
struct IsIterable
{
    typedef char true_type;
    typedef long false_type;

    template<class T>
    static true_type  is_beg_iterable(
    		int i,
            typename T::const_iterator = C().begin());
    template<class T>
    static false_type is_beg_iterable(...);

    enum { value = sizeof(is_beg_iterable<C>(0)) == sizeof(true_type) };
};

/*
 * geometry
 */
enum Orientation {
	_M_ = 0, //
	_P_ = 1, //
	_C_ = 2, //
};
inline Orientation ToOrientation(const St& i) {
	ASSERT(i >= 0 && i < 3);
	switch (i) {
	case 0:
		return _M_;
	case 1:
		return _P_;
	case 2:
		return _C_;
	default:
		ASSERT_MSG(false, "Error input Orientation");
	}
	SHOULD_NOT_REACH;
	return _M_;
}


enum Plane {
	_XY_ = 24,
	_YZ_ = 48,
	_ZX_ = 40,
};


inline Orientation Opposite(const Orientation& ori){
	if(ori == _M_){
		return _P_;
	}else if(ori == _P_){
		return _M_;
	}else{
		return _C_;
	}
}

inline void NormalPlane(const St&d, St& d1, St& d2){
	if(d == _X_){
		d1 = _Y_;
		d2 = _Z_;
	}else if(d == _Y_){
		d1 = _Z_;
		d2 = _X_;
	}else if(d == _Z_){
		d1 = _X_;
		d2 = _Y_;
	}else{
		SHOULD_NOT_REACH;
	}
}

inline St Normal(const St&d1, const St& d2){
	ASSERT(0 <= d1 && d1 <=2);
	ASSERT(0 <= d2 && d2 <=2);
	ASSERT(d1 != d2);
	return ToAxes(3 - d1 - d2);
}

enum Trinary {
	_POSITIVE_ = 1,  //
	_ZERO_     = 0,  //
	_NEGATIVE_ = -1, //
};

template <class TYPE>
inline int Heaviside(const TYPE& v){
	return v<=0?0:1;
}
template<class TYPE>
inline int Sign(const TYPE& x) {
	if (x < 0.0) {
		return -1;
	} else if (x > 0.0) {
		return 1;
	} else {
		return 0;
	}
}
template <class TYPE>
inline bool IsCloseTo(const TYPE& v, const TYPE& dst, const Vt tol = 1e-12){
	return std::abs(v - dst) < tol;
}
template <class TYPE>
inline bool IsCloseToZero(const TYPE& v, const Vt tol = 1e-12){
	return std::abs(v - 0.0) < tol;
}


inline int LoopNext(const int& bgn, const int& end, const int& cur){
	return (cur == end) ? bgn : cur + 1;
}
inline int LoopPrev(const int& bgn, const int& end, const int& cur){
	return (cur == bgn) ? end : cur - 1;
}

}




#endif
