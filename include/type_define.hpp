#ifndef TYPEDEFINE_HPP
#define TYPEDEFINE_HPP

#include "stddef.h"
#include <assert.h>
#include <iostream>
#include <math.h>
#include <string>
#include <functional>  // for std::forward()
#include <type_traits> // for std::is_same<> and invoke_result<>
#include <utility>     // for std::invoke()
#ifdef OPENMP
#include <omp.h>
#endif

#define ASSERT(expr) assert(expr)
// #define ASSERT_MSG(expr, msg) assert((expr)&&(msg))

#define SHOULD_NOT_REACH assert((false)&&(" >! Should not reach"))
#define CAST(type, p)           ((type)p)
#define CAST_REF(type, p)       (*((type)p))
#define _IF_TRUE_RETRUN(expr)   if(expr){return;};
#define _IF_FALSE_RETRUN(expr)  if(false==(expr)){return;};
#define _SMALL_ (1e-12)
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
#define ENABLE_IF(__T, __CONDITION)  typename std::enable_if< __CONDITION<__T>::value , bool>::type = true
                 
             
#define ENABLE_IF_CONTAINS(__T, __CONDITION)  typename std::enable_if< \
                  std::integral_constant< bool, \
                   IsContainer<__T>::value  \
                && __CONDITION<typename __T::value_type>::value  \
            >::value, bool>::type = true 

// #define ENABLE_IF(__T1, __CONDITION1, __T2, __CONDITION2)  typename std::enable_if< \
//                 std::integral_constant< bool, \
//                    __CONDITION1<__T1>::value  \
//                 && __CONDITION2<__T2>::value  \
//             >::value, bool>::type = true 

#define ENABLE_IF_1D_ARITHMATIC(__T) typename std::enable_if< \
                std::integral_constant< bool,                 \
                   IsContainer<__T>::value              \
                && std::is_arithmetic<typename __T::value_type>::value \
            >::value, bool>::type = true

#define ENABLE_IF_2D_ARITHMATIC(__T) typename std::enable_if<     \
                std::integral_constant< bool,                     \
                   IsContainer<__T>::value                        \
                && IsContainer<typename __T::value_type>::value   \
                && std::is_arithmetic<typename __T::value_type::value_type>::value \
            >::value, bool>::type = true

#   define ASSERT_MSG(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)

inline void __M_Assert(const char* expr_str, bool expr, 
            const char* file, int line, const std::string& msg){
    if (!expr){
        std::cerr << "Assert failed:\t" << msg << "\n"
                  << "Expected:\t" << expr_str << "\n"
                  << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}


namespace carpio {
// value type
typedef std::size_t St;    //size type
typedef double      Vt;    //value type
typedef int           Int;
typedef unsigned int uInt;
typedef double        Float;
typedef void*             utPointer;
typedef const void* const_utPointer;


typedef std::function<Vt(Vt, Vt, Vt, Vt)>   FunXYZT_V;
typedef std::function<void(Vt, Vt, Vt, Vt)> FunXYZT;
typedef std::function<Vt(Vt, Vt, Vt)>       FunXYZ_V;
typedef std::function<void(Vt, Vt, Vt)>     FunXYZ;

struct BaseTag{};

template<typename T>
struct HasTag
{
private:
    typedef char                      yes;
    typedef struct { char array[2]; } no;

    template<typename C> static yes test(typename C::Tag*);
    template<typename C> static no  test(...);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};

// Dim tag
struct DimTag: public BaseTag{};
struct Dim1Tag:public DimTag{};
struct Dim2Tag:public DimTag{};
struct Dim3Tag:public DimTag{};

template<St DIM>
struct DimTagTraits_ {using Type = DimTag;};

template<>
struct DimTagTraits_<1> {using Type = Dim1Tag;};

template<>
struct DimTagTraits_<2> {using Type = Dim2Tag;};

template<>
struct DimTagTraits_<3> {using Type = Dim3Tag;};

enum Axes {
    _X_ = 0, //
    _Y_ = 1, //
    _Z_ = 2, //
};
// Dim tag
struct AxesTag :public BaseTag{};
struct XTag:public AxesTag{};
struct YTag:public AxesTag{};
struct ZTag:public AxesTag{};

template<int AXES>
struct AxesTagTraits_ {using Type = AxesTag;};

template<>
struct AxesTagTraits_<0> {using Type = XTag;};

template<>
struct AxesTagTraits_<1> {using Type = YTag;};

template<>
struct AxesTagTraits_<2> {using Type = ZTag;};

inline Axes Next(const Axes& a){
    switch (a) {
    case _X_:
        return _Y_;
    case _Y_:
        return _Z_;
    case _Z_:
        return _X_;
    }
    return _X_;
}

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
    typedef char yes;
    typedef long no;

    template<class T>
    static yes  is_beg_iterable(
            int i,
            typename T::const_iterator = C().begin());
    template<class T>
    static no is_beg_iterable(...);

    enum { value = sizeof(is_beg_iterable<C>(0)) == sizeof(yes) };
};

template<typename T>
struct HasConstIterator
{
private:
    typedef char                      yes;
    typedef struct { char array[2]; } no;

    template<typename C> static yes test(typename C::const_iterator*);
    template<typename C> static no  test(...);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    typedef T type;
};

template <typename T>
struct HasBeginEnd
{
    template<typename C> 
    static char (&f(typename std::enable_if<
                            std::is_same<
                                decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
                                typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char (&f(...))[2];

    template<typename C> static char (&g(typename std::enable_if<
      std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
      typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char (&g(...))[2];

    static bool const beg_value = sizeof(f<T>(0)) == 1;
    static bool const end_value = sizeof(g<T>(0)) == 1;
};
// template <typename C>
// struct HasData
// {
//     typedef char yes;
//     typedef long no;

//     template<class T>
//     static yes has_data(int i,
//             typename T::value_type* = C().data());
//     template<class T>
//     static no has_data(...);

//     static bool const value = sizeof(has_data<C>(0)) == sizeof(yes);
// };

#define DEFINE_HAS_MEMBER(ooo)  \
template <typename, typename T> \
struct Has_##ooo {                \
    static constexpr bool value = false; \
};\
template <typename C, typename Ret, typename... Args> \
struct Has_##ooo<C, Ret(Args...)> { \
private:                          \
    template <typename T>         \
    static constexpr auto check(T*) \
        -> typename std::is_same<   \
            decltype(std::declval<T>().ooo(std::declval<Args>()...)), \
            Ret    \
        >::type; \
    template <typename> \
    static constexpr std::false_type check(...); \
public: \
    using type = decltype(check<C>(nullptr)); \
    static constexpr bool value = type::value; \
};


template<typename T> 
struct IsContainer : std::integral_constant<bool, 
    HasConstIterator<T>::value && HasBeginEnd<T>::beg_value && HasBeginEnd<T>::end_value> 
{ };

template<typename T>
struct IsNumber : std::integral_constant<bool,
      std::is_arithmetic<T>::value> { 
};

struct ArithmeticTag: public BaseTag{};

template<class ValueType,
         typename std::enable_if<
            (! HasTag<ValueType>::value)   //no tag
            && std::is_arithmetic<ValueType>::value
        , bool>::type = true>
struct ValueTagTraits_ {using Type = ArithmeticTag;};

template<typename T>
inline const char* GetTypeName() {
  return typeid(T).name();
}

#define DEFINE_TYPE_NAME(type, type_name)  \
  template<>                               \
  inline const char* GetTypeName<type>() { \
    return type_name;                      \
}

DEFINE_TYPE_NAME(int, "int")
DEFINE_TYPE_NAME(long, "long")
DEFINE_TYPE_NAME(long long, "long long")
DEFINE_TYPE_NAME(float, "float")
DEFINE_TYPE_NAME(double, "double")
DEFINE_TYPE_NAME(std::string, "string")
DEFINE_TYPE_NAME(bool, "bool")
DEFINE_TYPE_NAME(uint32_t, "uint")
DEFINE_TYPE_NAME(uint64_t, "uint")
// add your custom types' definitions

// template <typename Callable, typename... Args>
// decltype(auto) Invoke(Callable &&op, Args &&... args) {
//   if constexpr (std::is_same<std::invoke_result<Callable, Args...>::type, void>::value) {
//     // return type is void:
//     std::invoke(std::forward<Callable>(op), std::forward<Args>(args)...);
//     // ... 
//     return;
//   } else {
//     // return type is not void:
//     decltype(auto) ret{
//         std::invoke(std::forward<Callable>(op), std::forward<Args>(args)...)};
//     // ... 
//     return ret;
//   }
// }


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
    case 0: {return _M_;}
    case 1: {return _P_;}
    case 2: {return _C_;}
    default:
        ASSERT_MSG(false, "Error input Orientation");
    }
    SHOULD_NOT_REACH;
    return _M_;
}

enum CoorPlane {
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
inline void NormalPlane(const Axes&d, Axes& d1, Axes& d2){
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

inline std::string ToString(const Trinary& t){
    switch(t){
        case _POSITIVE_: {return "POSITIVE";break;}
        case _ZERO_:     {return "ZERO";    break;}
        case _NEGATIVE_: {return "NEGATIVE";break;}
    }
    return "ERROR";
}

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
