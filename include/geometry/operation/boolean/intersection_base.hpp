#ifndef _BOOLEAN_INTERSECTION_BASE_HPP_
#define _BOOLEAN_INTERSECTION_BASE_HPP_

#include <mutex>          // std::mutex

#include "geometry/geometry_define.hpp"

namespace carpio{


template<class VEC>
inline VEC Cross(const VEC& a, const VEC& b){
    return VEC(a[1]*b[2]-b[1]*a[2], 
               b[0]*a[2]-a[0]*b[2],
               a[0]*b[1]-b[0]*a[1]);
}
template<class VEC>
inline Vt Dot(const VEC& a, const VEC& b){
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]; 
}

template<class VEC, class NUM>
inline VEC Times(VEC& vec, const NUM& v){
    return VEC(vec[0] * v, 
               vec[1] * v, 
               vec[2] * v); 
}
template<class VEC>
inline Vt SquareSum(VEC& vec){
    return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]; 
}

template<class VEC>
inline int WhichSide32D(const VEC& p, const VEC& vec){
    // vec must be 0 base
    /// d1    = v1 - v3
    /// d2    = v2 - v3
    /// cross = d1x * d2y - d1y * d2x
    double res =  (p[0] * vec[1]) - (vec[0] * p[1]);
    if (res < 0){
        return 2;
    }else if (res > 0){
        return 1;
    }else{
        return 0;
    }
}

template<typename TYPE, St DIM>
struct IntersectionReturn_{
    typedef Point_<TYPE, DIM> Point;
    typedef std::list<int>  ListSize;
    typedef std::list<std::shared_ptr<Point> > ListspPoint;

    int         return_code;  //
    // std::string return_sting; // intersection name
    ListSize    indexs;       // size of each return objects
    ListspPoint points;

    // return code 1 intersect
    //             0 No intersect
    //             2 point touch
    //             3 line touch
    //             4 ploy touch

    IntersectionReturn_():
        return_code(0){
    }

    IntersectionReturn_(const int& c):
        return_code(c){
    }
};


template<class OBJ1, class OBJ2>
class IntersectionBase_{
public:
    typedef OBJ1 Object1;
    typedef OBJ2 Object2;
    typedef typename Object1::ValueType ValueType ;
    typedef IntersectionReturn_<ValueType, Object1::Dim> ReturnType;
protected:
    bool _inited;
    bool _called_intersect;
    bool _called_return;
public:
    IntersectionBase_(){
       _inited           = false;
       _called_intersect = false;
       _called_return    = false;
    }
    bool is_init() const{
        return _inited;
    }

    virtual ReturnType no_intersections() const { 
        return ReturnType();
    }

    virtual ~IntersectionBase_(){}

    virtual void set_method(const std::string& method){}
    // virtual ReturnType cal_intersection() const{}
    // virtual ReturnType cal_intersection(const Object1&, const Object2) const{}

    // virtual bool is_intersect() const{}
    // virtual bool is_intersect(const Object1&, const Object2) const{}


};


/**
 * The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
class SingletonIntersectionHelper
{
    /**
     * The Singleton's constructor/destructor should always be private to
     * prevent direct construction/desctruction calls with the `new`/`delete`
     * operator.
     */
public:
    // typedef std::bitset<6> BS;

    std::array<std::array<int,3>, 3> _mapte0e1;
private:
    static SingletonIntersectionHelper * pinstance_;
    static std::mutex mutex_;
protected:
    SingletonIntersectionHelper()
    {
        _init_map_te0e1();
    }
    ~SingletonIntersectionHelper() {}

    // std::string value_;
//  point location code -------------------------------------------------------
//           ^e1
//           |
//      4    3   2
//           |
//  --5------0-----1---->e0
//           |
//      6    7   8
//           |

    void _init_map_te0e1(){
        // 0 = 0, + = 1, - = 2
        _mapte0e1[0][0] = 0;
        _mapte0e1[0][1] = 1;
        _mapte0e1[1][2] = 2;
        _mapte0e1[1][0] = 3;
        _mapte0e1[1][1] = 4;
        _mapte0e1[0][1] = 5;
        _mapte0e1[2][1] = 6;
        _mapte0e1[2][0] = 7;
        _mapte0e1[2][1] = 8;
    }

    

public:
    /**
     * SingletonIntersectionHelpers should not be cloneable.
     */
    SingletonIntersectionHelper(SingletonIntersectionHelper &other) = delete;
    /**
     * SingletonIntersectionHelpers should not be assignable.
     */
    void operator=(const SingletonIntersectionHelper &) = delete;
    /**
     * This is the static method that controls the access to the SingletonIntersectionHelper
     * instance. On the first run, it creates a SingletonIntersectionHelper object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static SingletonIntersectionHelper *GetInstance();
    /**
     * Finally, any SingletonIntersectionHelper should define some business logic, which can be
     * executed on its instance.
     */
    int point_location_in_coordinate(int side0, int side1)
    {
        // side0 relative to coordinate x
        // side1 relative to coordinate y
        return this->_mapte0e1[side0][side1]; 
    }
    
};

/**
 * Static methods should be defined outside the class.
 */

SingletonIntersectionHelper* SingletonIntersectionHelper::pinstance_{nullptr};
std::mutex SingletonIntersectionHelper::mutex_;

/**
 * The first time we call GetInstance we will lock the storage location
 *      and then we make sure again that the variable is null and then we
 *      set the value. RU:
 */
SingletonIntersectionHelper *SingletonIntersectionHelper::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new SingletonIntersectionHelper();
    }
    return pinstance_;
}


}


#endif