#ifndef _INTERSECTION_SEGTRI_HPP_
#define _INTERSECTION_SEGTRI_HPP_

#include "geometry/geometry_define.hpp"
#include "intersection_base.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/basic/triangle.hpp"

#include <memory>
#include <mutex>          // std::mutex
#include <unordered_map>
#include <bitset>

namespace carpio {

class SingletonIntersectionHelper
{
public:
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
        _mapte0e1[0][2] = 1;
        _mapte0e1[1][2] = 2;
        _mapte0e1[1][0] = 3;
        _mapte0e1[1][1] = 4;
        _mapte0e1[0][1] = 5;
        _mapte0e1[2][1] = 6;
        _mapte0e1[2][0] = 7;
        _mapte0e1[2][2] = 8;
    }

    

public:
    
    SingletonIntersectionHelper(SingletonIntersectionHelper &other) = delete;
    void operator=(const SingletonIntersectionHelper &) = delete;

    static SingletonIntersectionHelper *GetInstance();
    
    int point_location_in_coordinate(int side0, int side1)
    {
        // side0 relative to coordinate x
        // side1 relative to coordinate y
        return this->_mapte0e1[side0][side1]; 
    }
    
};

SingletonIntersectionHelper* SingletonIntersectionHelper::pinstance_{nullptr};
std::mutex SingletonIntersectionHelper::mutex_;

SingletonIntersectionHelper *SingletonIntersectionHelper::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new SingletonIntersectionHelper();
    }
    return pinstance_;
}

 
template<typename TYPE, St DIM>
class IntersectionSegTri_:public IntersectionBase_<Segment_<TYPE, DIM>, Triangle_<TYPE, DIM> >{
public:
    typedef IntersectionBase_<Segment_<TYPE, DIM>, Triangle_<TYPE, DIM> > Base;
    typedef typename Base::Object1 Object1;     
    typedef typename Base::Object2 Object2;     
    typedef typename Base::ReturnType ReturnType;
    typedef Triangle_<TYPE, DIM> Triangle;     
    typedef Point_<TYPE, DIM>                        Point;     
    typedef Point_<TYPE, DIM>                        Vec;     
    typedef std::shared_ptr<Vec>                   spVec;     
    typedef std::shared_ptr<Point_<TYPE, DIM> >    spPoint;     
    typedef std::shared_ptr<Triangle>              spTri;     
protected:


};


//  point location code -------------------------------------------------------
//           ^e1
//           |
//      4    3   2
//           |
//  --5------0-----1---->e0
//           |
//      6    7   8
//           |


//  Intersections location code for triangle ----------------
//      e1(2)
//     /   \
//   (4)(6)(5) (7)   
//   /       \
//  0(0)-(3)->e0(1)
//
//  0 -- base point
//  1 -- vertex e0
//  2 -- vertex e1
//  3 -- edge e0
//  4 -- edge e1
//  5 -- edge e0e1
//  6 -- inside triangle
//  -1 -- outside triangle

//  Intersection location code for segment -------------------
//       (3)
//  (0)--(2)-->(1)
//
//  0 -- vertex 0
//  1 -- vertex 1
//  2 -- on segment
//  -1 -- outside segment
template<typename TYPE>
class IntersectionSegTri_<TYPE, 2>:public IntersectionBase_<Segment_<TYPE, 2>, Triangle_<TYPE, 2> >{
public:
    typedef IntersectionBase_<Segment_<TYPE, 2>, Triangle_<TYPE, 2> > Base;
    typedef IntersectionSegTri_<TYPE, 2> Self; 
    typedef typename Base::Object1 Object1;     
    typedef typename Base::Object2 Object2;     
    typedef typename Base::ReturnType ReturnType;
    typedef Segment_<TYPE,2>                       Segment;
    typedef std::shared_ptr<Segment>             spSeg;
    typedef Triangle_<TYPE, 2>                     Triangle;     
    typedef Point_<TYPE, 2>                        Point;     
    typedef Point_<TYPE, 2>                        Vec;     
    typedef std::shared_ptr<Vec>                   spVec;     
    typedef std::shared_ptr<Point_<TYPE, 2> >    spPoint;     
    typedef std::shared_ptr<Triangle>              spTri;

    typedef std::array<std::array<short,2>, 2> MatLoc;
    typedef std::array<Vec, 2> ArrayVec;
    typedef std::function<void(
        const ArrayVec&, const ArrayVec&, 
        const int&, const int&, MatLoc&)> IFun;
    typedef std::array<std::array<IFun, 9>, 9> MatFun;
protected:
    spTri _tri;
    spSeg _seg;
    std::array<Vec, 2> _atri;  // array trangle rebased
    std::array<Vec, 2> _aseg;  // segment rebased

    std::array<int, 2> _code;

    MatLoc _loccode;
    //                   code of triangle, code of segment
    // intersect point1[[      0~7       ,       0~3      ], 
    // intersect point2 [      0~7       ,       0~3      ]] 

    MatFun _matfun;
public:
    // Intitial from shared_ptr
    IntersectionSegTri_(spTri spt, spSeg sps): _tri(spt), _seg(sps){
    }
    // Intitial from object
    IntersectionSegTri_(const Triangle& t, const Segment& s){
        this->_tri = std::make_shared<Triangle>(t);
        this->_seg = std::make_shared<Segment>(s);

        _rebase(*_tri, *_seg);

        _cal_location_code();

        _init_mat_fun();
    }
    // Intitial from rebased objects
    IntersectionSegTri_(const Vec& e0, 
                        const Vec& e1, 
                        const Vec& t0, 
                        const Vec& t1){
        _atri[0] = e0;
        _atri[1] = e1;
        _aseg[0] = t0;
        _aseg[1] = t1;
    }

    int triangle_intersection_code(int idx){
        return _loccode[idx][0];
    }
    int segment_intersection_code(int idx){
        return _loccode[idx][1];
    }

    bool is_intersect(){
        // std::cout << "location code 0 = " << _code[0] << std::endl;
        // std::cout << "location code 1 = " << _code[1] << std::endl;
        MatLoc& mat = this->_loccode;
        mat.fill({-1,-1});
        this->_matfun[this->_code[0]][this->_code[1]](
            this->_atri, this->_aseg, 
            this->_code[0], this->_code[1], mat);
        std::cout << mat[0][0] << ",  " << mat[0][1] << std::endl;
        std::cout << mat[1][0] << ",  " << mat[1][1] << std::endl;
        return false;
    }
protected:
    void _rebase(const Triangle& t, const Segment& s){
        // rebase to tri0
        _atri[0] = t[1] - t[0];
        _atri[1] = t[2] - t[0];

        _aseg[0] = s[0] - t[0];
        _aseg[1] = s[1] - t[0];

    }
    // vectors must be rebased.
    void _cal_location_code(){
        SingletonIntersectionHelper* phelper = SingletonIntersectionHelper::GetInstance();
        int side0 = WhichSide32D(_atri[0], _aseg[0]);
        int side1 = WhichSide32D(_atri[1], _aseg[0]);
        std::cout<< "s0 = " << side0 << std::endl;
        std::cout<< "s1 = " << side1 << std::endl;
        _code[0] = phelper->point_location_in_coordinate(side0, side1);
        std::cout << "location code 0 = " << _code[0] << std::endl;
        side0 = WhichSide32D(_atri[0], _aseg[1]);
        side1 = WhichSide32D(_atri[1], _aseg[1]);
        std::cout<< "s0 = " << side0 << std::endl;
        std::cout<< "s1 = " << side1 << std::endl;
        _code[1] = phelper->point_location_in_coordinate(side0, side1);
        std::cout << "location code 1 = " << _code[1] << std::endl;
    }

    void _init_mat_fun(){
        _matfun[0][1] = Self::_fun01_03;
        _matfun[1][0] = Self::_fun01_03;
        _matfun[0][3] = Self::_fun01_03;
        _matfun[3][0] = Self::_fun01_03;
        _matfun[0][4] = Self::_fun0_45678;
        _matfun[4][0] = Self::_fun0_45678;
        _matfun[0][5] = Self::_fun0_45678;
        _matfun[5][0] = Self::_fun0_45678;
        _matfun[0][6] = Self::_fun0_45678;
        _matfun[6][0] = Self::_fun0_45678;
        _matfun[0][7] = Self::_fun0_45678;
        _matfun[7][0] = Self::_fun0_45678;
        _matfun[0][8] = Self::_fun0_45678;
        _matfun[8][0] = Self::_fun0_45678;
        _matfun[1][2] = Self::_fun12_32;
        _matfun[2][1] = Self::_fun12_32;

        _matfun[0][2] = Self::_fun0_2;
        _matfun[2][0] = Self::_fun0_2;
        _matfun[1][1] = Self::_fun11_33;
        _matfun[3][3] = Self::_fun11_33;
    }

    static const int _N_ = 2;  
    static const int _O_ = 0;  
    static const int _P_ = 1;
    static const short S1 = 0, S2 = 1, SE = 2, SO = 3, TO = 7, TIN = 6;

    static inline short _SideToLoc(const short& side,
                             const short& pl,
                             const short& ol,
                             const short& nl){
        return (side == _P_) ? pl : (side == _O_) ? ol : nl;
    }  

    static void _fun01_03(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatLoc& ml){
        short T1 = 0, T2 = 1, TE = 3;
        if(c0 == 3 || c1 == 3){
            T1 = 0, T2 = 2, TE = 4;
        }
        int zero = c1>c0?0:1; 
        int one  = c1>c0?1:0; 
        int rc = WhichSide32D(
            tri[one], seg[one], tri[zero]);
        if(rc == _N_){
            ml[zero][zero] = T1;
            ml[zero][one]  = S1;
            ml[one][zero]  = T2;
            ml[one][one]   = TE;
        }else if(rc == _O_){
            ml[zero][zero] = T1;
            ml[zero][one]  = S1;
            ml[one][zero]  = T1;
            ml[one][one]   = S2;
        }else{
            ml[zero][zero] = T1;
            ml[zero][one]  = S1;
            ml[one][zero]  = TE;
            ml[one][one]   = S2;
        }
    }

    static void _fun0_45678(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatLoc& ml){
        int zero = c1>c0?0:1; 
        int one  = c1>c0?1:0; 
        ml[zero][zero] = 0;
        ml[zero][one]  = 0;
        ml[one][zero]  = TO;
        ml[one][one]   = SO;
    }

    static void _fun0_2(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatLoc& ml){
        int zero = c1>c0?0:1; 
        int one  = c1>c0?1:0; 
        int rc = WhichSide32D(
            tri[one], seg[one], tri[zero]);
        if(rc == 2){
            ml[zero][zero] = 0;
            ml[zero][one]  = 0;
            ml[one][zero]  = 5;
            ml[one][one]   = 2;
        }else if(rc == 0){
            ml[zero][zero] = 0;
            ml[zero][one]  = 0;
            ml[one][zero]  = 5;
            ml[one][one]   = 1;
        }else{
            ml[zero][zero] = 0;
            ml[zero][one]  = 0;
            ml[one][zero]  = 6;
            ml[one][one]   = 1;
        }
    }
    
    static void _fun12_32(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatLoc& ml){
        short zero = c1>c0?0:1; 
        short one  = c1>c0?1:0; 
        short T2 = 1, TE = 3, TE2 = 5; 
        if (c0 == 3 || c1 == 3) {
            T2 = 2, TE = 4;
        }
        short rc0 = WhichSide32D(
            tri[one], seg[zero], tri[zero]);
        short rc1 = WhichSide32D(
            tri[one], seg[one], tri[zero]);
        switch(rc0){
            case _P_:{
                ml[0][0] = TE;
                ml[0][1] = S1; 
                ml[1][0] = _SideToLoc(rc1, TIN, TE2, TE2); 
                ml[1][1] = _SideToLoc(rc1, S2, SE, SE); 
            };break;
            case _O_:{
                ml[0][0] = T2;
                ml[0][1] = S1; 
                ml[1][0] = _SideToLoc(rc1, TIN, TE2, TO); 
                ml[1][1] = _SideToLoc(rc1, S2, S2, SO); 
            };break;
            case _N_:{
                ml[0][0] = _SideToLoc(rc1, TE2, TE2, TO);
                ml[0][1] = _SideToLoc(rc1, SE, SE, SO); 
                ml[1][0] = _SideToLoc(rc1, TE, T2, TO); 
                ml[1][1] = _SideToLoc(rc1, S2, S2, SO); 
            };break;
        }
    }
    
    static void _fun11_33(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatLoc& ml){
        short T1 = 0, T2 = 1, TE = 3;
        if(c0 == 3){
            T1 = 0, T2 = 2, TE = 4;
        }
        short rc0 = WhichSide32D(
            tri[1], seg[0], tri[0]);
        short rc1 = WhichSide32D(
            tri[1], seg[1], tri[0]);
        switch(rc0){
            case _P_:{
                ml[0][0] = TE;
                ml[0][1] = S1; 
                ml[1][0] = _SideToLoc(rc1, TE, T2, T2); 
                ml[1][1] = _SideToLoc(rc1, S2, S2, SE); 
            };break;
            case _O_:{
                ml[0][0] = T2;
                ml[0][1] = S1; 
                ml[1][0] = _SideToLoc(rc1, TE, T2, T2); 
                ml[1][1] = _SideToLoc(rc1, S2, S2, SO); 
            };break;
            case _N_:{
                ml[0][0] = _SideToLoc(rc1, T2, T2, TO);
                ml[0][1] = _SideToLoc(rc1, S2, S2, SO); 
                ml[1][0] = _SideToLoc(rc1, TE, T2, TO); 
                ml[1][1] = _SideToLoc(rc1, S2, S2, SO); 
            };break;
        }
    }
};

template<typename TYPE>
class IntersectionSegTri_<TYPE, 3>:public IntersectionBase_<Segment_<TYPE, 3>, Triangle_<TYPE, 2> >{
public:
    IntersectionSegTri_(){
        std::cout << "Intersection 3" << std::endl;
    }
};
}

#endif //