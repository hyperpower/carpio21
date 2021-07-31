#ifndef _INTERSECTION_SEGTRI_HPP_
#define _INTERSECTION_SEGTRI_HPP_

#include "geometry/geometry_define.hpp"
#include "intersection_base.hpp"
#include "segment_segment.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/basic/triangle.hpp"

#include <memory>
#include <mutex>          // std::mutex
#include <unordered_map>
#include <bitset>
#include <stdexcept>

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

    typedef std::array<std::array<short,2>, 2> MatIC;
    typedef std::array<Vec, 2> ArrayVec;
    typedef std::function<void(
        const ArrayVec&, const ArrayVec&, 
        const int&, const int&, MatIC&)> IFun;
    typedef std::array<std::array<IFun, 9>, 9> MatFun;
protected:
    spTri _tri;
    spSeg _seg;
    std::array<Vec, 2> _atri;  // array trangle rebased
    std::array<Vec, 2> _aseg;  // segment rebased

    std::array<int, 2> _code;

    MatIC _mat_ic;
    //                   code of triangle, code of segment
    // intersect point1[[      0~7       ,       0~3      ], 
    // intersect point2 [      0~7       ,       0~3      ]] 

    MatFun _matfun;

    // rebased results
    std::array<Vec, 2> _res;  // results t0 base
    std::array<int, 2> _rfs;  // valide flag

    Point _vec0; 
public:
    // Intitial from shared_ptr
    IntersectionSegTri_(spTri spt, spSeg sps): _tri(spt), _seg(sps) {
        _vec0[0] = 0.0;
        _vec0[1] = 0.0;
    }
    // Intitial from object
    IntersectionSegTri_(const Triangle& t, const Segment& s){
        _vec0[0] = 0.0;
        _vec0[1] = 0.0;
        this->_tri = std::make_shared<Triangle>(t);
        this->_seg = std::make_shared<Segment>(s);
        _rebase(*_tri, *_seg);
        _init_mat_fun();
        this->_inited = true;
    }
    // Intitial from rebased objects
    IntersectionSegTri_(const Vec& e0, 
                        const Vec& e1, 
                        const Vec& t0, 
                        const Vec& t1){
        _vec0[0] = 0.0;
        _vec0[1] = 0.0;

        _atri[0] = e0;
        _atri[1] = e1;
        _aseg[0] = t0;
        _aseg[1] = t1;
        _init_mat_fun();
        this->_inited = true;
    }

    int triangle_intersection_code(int idx){
        return _mat_ic[idx][0];
    }
    int segment_intersection_code(int idx){
        return _mat_ic[idx][1];
    }

    int location_code(int idx){
        return _code[idx];
    }

    void cal_code(){
        _cal_location_code();
        _cal_intersection_code();
        this->_called_intersect = true;
    }

    void cal_intersection(){
        _cal_intersection();
        this->_called_return = true;
    }

    // void cal_intersect(){
    //     // std::cout << "location code 0 = " << _code[0] << std::endl;
    //     // std::cout << "location code 1 = " << _code[1] << std::endl;
    //     MatIC& mat = this->_mat_ic;
    //     mat.fill({-1,-1});
    //     this->_matfun[this->_code[0]][this->_code[1]](
    //         this->_atri, this->_aseg, 
    //         this->_code[0], this->_code[1], mat);
    //     // std::cout << mat[0][0] << ",  " << mat[0][1] << std::endl;
    //     // std::cout << mat[1][0] << ",  " << mat[1][1] << std::endl;
    // }
    
    Point get_intersect_point(const St& idx){
        return _res[idx]+(*_tri)[0];
    }

    bool has_intersect_point(const St& idx){
        return _rfs[idx] >= 0 ;
    }

    bool is_same_intersect_point(){
        if((_mat_ic[0][0] == _mat_ic[1][0]) && _mat_ic[0][0] < 3){
            return true;
        }
        if((_mat_ic[0][1] == _mat_ic[1][1]) && _mat_ic[0][1] < 2){
            return true;
        }
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
        int side0 = _on_which_side(_atri[0], _aseg[0]);
        int side1 = _on_which_side(_atri[1], _aseg[0]);
        // std::cout<< "s0 = " << side0 << std::endl;
        // std::cout<< "s1 = " << side1 << std::endl;
        _code[0] = phelper->point_location_in_coordinate(side0, side1);
        // std::cout << "location code 0 = " << _code[0] << std::endl;
        side0 = _on_which_side(_atri[0], _aseg[1]);
        side1 = _on_which_side(_atri[1], _aseg[1]);
        // std::cout<< "s0 = " << side0 << std::endl;
        // std::cout<< "s1 = " << side1 << std::endl;
        _code[1] = phelper->point_location_in_coordinate(side0, side1);
        // std::cout << "location code 1 = " << _code[1] << std::endl;
    }
    void _cal_intersection_code(){
        // std::cout << "location code 0 = " << _code[0] << std::endl;
        // std::cout << "location code 1 = " << _code[1] << std::endl;
        MatIC& mat = this->_mat_ic;
        mat.fill({-1,-1});
        this->_matfun[this->_code[0]][this->_code[1]](
            this->_atri, this->_aseg, 
            this->_code[0], this->_code[1], mat);
        // std::cout << mat[0][0] << ",  " << mat[0][1] << std::endl;
        // std::cout << mat[1][0] << ",  " << mat[1][1] << std::endl;
    }

    bool _has_point(int idx){
        if(_mat_ic[idx][0] == 7 && _mat_ic[idx][1] == 3){
            return false;
        }else{
            return true;
        }
    }

    bool _find_exist_point(int idx){
        if(_mat_ic[idx][0] == 0){
            _res[idx] = Vec(0.0, 0.0);
            return true;
        }else if(_mat_ic[idx][0] < 3){ // 1,2
            _res[idx] = _atri[_mat_ic[idx][0] - 1];
            return true;
        }else if(_mat_ic[idx][1] < 2){ // 0,1
            _res[idx] = _aseg[_mat_ic[idx][1]];
            return true;
        }
        return false;
    }

    int _cal_one_point(int idx){
        if(_has_point(idx)){
            if(!_find_exist_point(idx)){
                switch(_mat_ic[idx][0]){
                    case 3:{
                        _res[idx] = CalSegmentsIntersection(
                            _atri[0], _aseg[0], _aseg[1]);
                    }break;
                    case 4:{
                        _res[idx] = CalSegmentsIntersection(
                            _atri[1], _aseg[0], _aseg[1]);
                    }break;
                    case 5:{
                        _res[idx] = CalSegmentsIntersection(
                            _atri[0], _atri[1], _aseg[0], _aseg[1]);
                    }break;
                    default: {
                        throw std::invalid_argument("Not a Triangle Edge Code");
                    }
                }
                return 1; // cal a new one
            }else{
                return 0; //exist point
            }
        }
        return -1; // no point
    }
    
    void _cal_intersection(){
        // find intersetion point from mat_ic
        _rfs[0] = _cal_one_point(0);
        _rfs[1] = _cal_one_point(1);
    }

    void _init_mat_fun(){
        _matfun[0][0] = Self::_fun00;
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
        _matfun[1][3] = Self::_fun13;
        _matfun[3][1] = Self::_fun13;
        _matfun[1][4] = Self::_fun14;
        _matfun[4][1] = Self::_fun14;
        _matfun[2][4] = Self::_fun14;
        _matfun[4][2] = Self::_fun14;
        _matfun[3][8] = Self::_fun14;
        _matfun[8][3] = Self::_fun14;
        _matfun[2][8] = Self::_fun14;
        _matfun[8][2] = Self::_fun14;
        _matfun[3][4] = Self::_fun34;
        _matfun[4][3] = Self::_fun34;
        _matfun[3][5] = Self::_fun34;
        _matfun[5][3] = Self::_fun34;
        _matfun[3][6] = Self::_fun34;
        _matfun[6][3] = Self::_fun34;
        _matfun[1][8] = Self::_fun34;
        _matfun[8][1] = Self::_fun34;
        _matfun[1][7] = Self::_fun34;
        _matfun[7][1] = Self::_fun34;
        _matfun[1][6] = Self::_fun34;
        _matfun[6][1] = Self::_fun34;
        _matfun[2][5] = Self::_fun25;
        _matfun[5][2] = Self::_fun25;
        _matfun[2][7] = Self::_fun25;
        _matfun[7][2] = Self::_fun25;
        _matfun[1][5] = Self::_fun15;
        _matfun[5][1] = Self::_fun15;
        _matfun[3][7] = Self::_fun15;
        _matfun[7][3] = Self::_fun15;
        _matfun[2][6] = Self::_fun26;
        _matfun[6][2] = Self::_fun26;
        _matfun[4][8] = Self::_fun48;
        _matfun[8][4] = Self::_fun48;

        _matfun[3][2] = Self::_fun12_32;
        _matfun[2][3] = Self::_fun12_32;

        _matfun[0][2] = Self::_fun02;
        _matfun[2][0] = Self::_fun02;
        _matfun[1][1] = Self::_fun11_33;
        _matfun[2][2] = Self::_fun22;
        _matfun[3][3] = Self::_fun11_33;

        _matfun[4][4] = Self::_fun_no;
        _matfun[4][5] = Self::_fun_no;
        _matfun[4][6] = Self::_fun_no;
        _matfun[4][7] = Self::_fun_no;
        _matfun[5][4] = Self::_fun_no;
        _matfun[6][4] = Self::_fun_no;
        _matfun[7][4] = Self::_fun_no;
        for(int i = 5; i < 9; i++){
            for(int j = 5; j < 9; j++){
                _matfun[i][j] = Self::_fun_no;
            }
        }
    }

    static const int _N_ = 2;  
    static const int _O_ = 0;  
    static const int _P_ = 1;
    static const short S1 = 0;
    static const short S2 = 1;
    static const short SE = 2;
    static const short SO = 3;
    static const short TO = 7;
    static const short TIN = 6;
    
    static int _on_which_side(const Vec&p1, const Vec&p2){
        Vec p3(0,0);
        return _on_which_side(p1, p2, p3);
    }
    static int _on_which_side(const Vec&p1, const Vec&p2, const Vec& p3){
        auto tri = OnWhichSide3(p1, p2, p3);
        switch(tri){
            case _POSITIVE_:{return 1; break;}
            case _ZERO_:    {return 0; break;}
            case _NEGATIVE_:{return 2; break;}
        } 
        SHOULD_NOT_REACH;
        return 0;
    }

    static inline short _SideToLoc(
        short side, short pl, short ol, short nl){
        return (side == _P_) ? pl : (side == _O_) ? ol : nl;
    }  
    static void _fun_no(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatIC& ml){
        ml[0][0] = 7;
        ml[0][1] = 3;
        ml[1][0] = 7;
        ml[1][1] = 3;
    }
    static void _fun00(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatIC& ml){
        ml[0][0] = 0;
        ml[0][1] = 0;
        ml[1][0] = 0;
        ml[1][1] = 1;
    }
    static void _fun01_03(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatIC& ml){
        short T1   = 0, T2 = 1, TE = 3;
        short _S1  = c1>c0?S1:S2; 
        short _S2  = c1>c0?S2:S1; 
        if(c0 == 3 || c1 == 3){
            T1 = 0, T2 = 2, TE = 4;
        }
        int zero = c1>c0?0:1; 
        int one  = c1>c0?1:0; 
        int rc = _on_which_side(
            tri[1], seg[one], tri[0]);
        if(rc == _N_){
            ml[zero][0] = T1;
            ml[zero][1] = _S1;
            ml[one][0]  = T2;
            ml[one][1]  = SE;
        }else if(rc == _O_){
            ml[zero][0] = T1;
            ml[zero][1] = _S1;
            ml[one][0]  = T2;
            ml[one][1]  = _S2;
        }else{
            ml[zero][0] = T1;
            ml[zero][1] = _S1;
            ml[one][0]  = TE;
            ml[one][1]  = _S2;
        }
    }

    static void _fun0_45678(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatIC& ml){
        int zero   = c1>c0?0:1; 
        int one    = c1>c0?1:0; 
        short _S1  = c1>c0?S1:S2; 
        ml[zero][0] = 0;
        ml[zero][1] = _S1;
        ml[one][0]  = TO;
        ml[one][1]  = SO;
    }

    static void _fun02(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatIC& ml){
        short zero = c1>c0?0:1; 
        short one  = c1>c0?1:0; 
        short _S1  = c1>c0?S1:S2; 
        short _S2  = c1>c0?S2:S1; 
        int rc = _on_which_side(
            tri[1], seg[one], tri[0]);
        if(rc == _N_){
            ml[zero][0] = 0;
            ml[zero][1] = _S1;
            ml[one][0]  = 5;
            ml[one][1]  = 2;
        }else if(rc == _O_){
            ml[zero][0] = 0;
            ml[zero][1] = _S1;
            ml[one][0]  = 5;
            ml[one][1]  = _S2;
        }else{ // _P_
            ml[zero][0] = 0;
            ml[zero][1] = _S1;
            ml[one][0]  = TIN;
            ml[one][1]  = _S2;
        }
    }

    static void _fun13(
                    const ArrayVec& tri,
                    const ArrayVec& seg,
                    const int& c0, const int& c1,
                    MatIC& ml){
        short zero, one, _S1, _S2;
        zero = c1>c0?0:1; 
        one  = c1>c0?1:0;
        _S1  = c1>c0?S1:S2; 
        _S2  = c1>c0?S2:S1;
        short _TE1 = 3, _TE2 = 4; 
        short _TV1 = 1, _TV2 = 2; 
        short rc0 = _on_which_side(
            tri[1], seg[zero], tri[0]);
        short rc1 = _on_which_side(
            tri[1], seg[one], tri[0]);
        switch(rc0){
            case _P_:{
                ml[zero][0] = _TE1;
                ml[zero][1] = _S1; 
                ml[one][0]  = _SideToLoc(rc1, _TE2, _TV2, 5); 
                ml[one][1]  = _SideToLoc(rc1, _S2, _S2, SE); 
            };break;
            case _O_:{
                ml[zero][0] = _TV1;
                ml[zero][1] = _S1; 
                ml[one][0]  = _SideToLoc(rc1, _TE2, _TV2, TO); 
                ml[one][1]  = _SideToLoc(rc1, _S2,  _S2,  SO); 
            };break;
            case _N_:{
                ml[zero][0] = _SideToLoc(rc1, 5, TO, TO);
                ml[zero][1] = _SideToLoc(rc1, SE,  _S2, SO); 
                ml[one][0]  = _SideToLoc(rc1, _TE2, _TV2, TO); 
                ml[one][1]  = _SideToLoc(rc1, _S2, _S2, SO); 
            };break;
        }
    }
    static void _fun14(
                    const ArrayVec& tri,
                    const ArrayVec& seg,
                    const int& c0, const int& c1,
                    MatIC& ml){
        short zero, one, _S1, _S2;
        int c[2] = {c0, c1};
        bool upper = c1>c0;
        zero = upper?0:1; 
        one  = upper?1:0;
        _S1  = upper?S1:S2; 
        _S2  = upper?S2:S1;
        short BP, BO, BN, EN, EV, EP, IdxV;

        if(c[zero] == 1){
            BP = 3, BO = 1, BN = TO; 
        }else if (c[zero] == 2){ // == 2
            BP = 6, BO = 5, BN = TO; 
        }else{ // == 3
            BP = 4, BO = 2, BN = TO; 
        }
        if(c[one] == 4){
            EN = 4, EV = 2, EP = 5;
            IdxV  = 1; 
        }else{ // == 8
            EN = 3, EV = 1, EP = 5;
            IdxV  = 0;
        }
        short rc0 = _on_which_side(
            tri[1], seg[zero], tri[0]);
        short rc1;
        if(c[one] == 4){
            rc1= _on_which_side(
                seg[one], tri[IdxV], seg[zero]);
        }else{
            rc1 = _on_which_side(
                seg[zero], tri[IdxV], seg[one]);
        }
        switch(rc0){
            case _P_:{
                ml[zero][0] = BP;
                ml[zero][1] = _S1; 
                ml[one][0]  = _SideToLoc(rc1, EP, EV, EN); 
                ml[one][1]  = SE; 
            };break;
            case _O_:{
                ml[zero][0] = BO;
                ml[zero][1] = _S1; 
                ml[one][0]  = _SideToLoc(rc1, TO, EV, EN); 
                ml[one][1]  = _SideToLoc(rc1, SO, SE, SE); 
            };break;
            case _N_:{
                ml[zero][0] = _SideToLoc(rc1, TO, EV, EP); 
                ml[zero][1] = _SideToLoc(rc1, SO, SE, SE); 
                ml[one][0]  = _SideToLoc(rc1, TO, EV, EN); 
                ml[one][1]  = _SideToLoc(rc1, SO, SE, SE); 
            };break;
        }
    }
    static void _fun26(
                    const ArrayVec& tri,
                    const ArrayVec& seg,
                    const int& c0, const int& c1,
                    MatIC& ml){
        short zero, one, _S1;
        int c[2] = {c0, c1};
        bool upper = c1>c0;
        zero = upper?0:1; 
        one  = upper?1:0;
        _S1  = upper?S1:S2; 
        Vec p0(0.0,0.0);
        short rc0 = _on_which_side(
            tri[1], seg[zero], tri[0]);
        short tc0 = _on_which_side(
            seg[one], p0, seg[zero]);
        switch(rc0){
            case _P_:{
                ml[zero][0] = TIN;
                ml[zero][1] = _S1; 
                ml[one][0]  = _SideToLoc(tc0, 4, 0, 3); 
                ml[one][1]  = SE; 
            };break;
            case _O_:{
                ml[zero][0] = 5;
                ml[zero][1] = _S1; 
                ml[one][0]  = _SideToLoc(tc0, 4, 0, 3); 
                ml[one][1]  = SE; 
            };break;
            case _N_:{
                if (tc0 == _P_){
                    short tcv = _on_which_side(
                        seg[one], tri[1], seg[zero]);
                    ml[zero][0] = _SideToLoc(tcv, TO, 2, 5); 
                    ml[zero][1] = _SideToLoc(tcv, SO, SE, SE); 
                    ml[one][0]  = _SideToLoc(tcv, TO, 2, 4); 
                    ml[one][1]  = _SideToLoc(tcv, SO, SE, SE); 
                }else if (tc0 == _O_){
                    ml[zero][0] = 5; 
                    ml[zero][1] = SE; 
                    ml[one][0]  = 0; 
                    ml[one][1]  = SE; 
                }else{
                    short tcv = _on_which_side(
                        seg[one], tri[0], seg[zero]);
                    ml[zero][0] = _SideToLoc(tcv, 5, 1, TO); 
                    ml[zero][1] = _SideToLoc(tcv, SE, SE, SO); 
                    ml[one][0]  = _SideToLoc(tcv, 3, 1, TO); 
                    ml[one][1]  = _SideToLoc(tcv, SE, SE, SO); 
                }
            };break;
        }

    }
    static void _fun48(
                    const ArrayVec& tri,
                    const ArrayVec& seg,
                    const int& c0, const int& c1,
                    MatIC& ml){
        short zero, one, _S1;
        int c[2] = {c0, c1};
        bool upper = c1>c0;
        zero = upper?0:1; 
        one  = upper?1:0;
        _S1  = upper?S1:S2; 
        Vec p0(0.0,0.0);
        // short rc0 = _on_which_side(
            // tri[1], seg[zero], tri[0]);
        short tc0 = _on_which_side(
            seg[one], p0, seg[zero]);            
        switch(tc0){
            case _P_:{
                ml[zero][0] = TO;
                ml[zero][1] = SO; 
                ml[one][0]  = TO; 
                ml[one][1]  = SO; 
            };break;
            case _O_:{
                ml[zero][0] = 0;
                ml[zero][1] = SE; 
                ml[one][0]  = 0; 
                ml[one][1]  = SE; 
            };break;
            case _N_:{
                short tc1 = _on_which_side(
                    seg[one], tri[0], seg[zero]);
                short tc2 = _on_which_side(
                    seg[one], tri[1], seg[zero]);
                if(tc2 == _P_){
                    ml[zero][0] = 4; 
                    ml[zero][1] = SE; 
                    ml[one][0]  = _SideToLoc(tc1, 3, 1, 5); 
                    ml[one][1]  = SE;
                } else if(tc2 == _O_){
                    ml[zero][0] = 2; 
                    ml[zero][1] = SE; 
                    ml[one][0]  = _SideToLoc(tc1, 3, 1, TO); 
                    ml[one][1]  = _SideToLoc(tc1, SE,SE,SO);
                } else {
                    ml[zero][0] = _SideToLoc(tc1, 5, 1, TO); 
                    ml[zero][1] = _SideToLoc(tc1, SE,SE,SO); 
                    ml[one][0]  = _SideToLoc(tc1, 3, 1, TO); 
                    ml[one][1]  = _SideToLoc(tc1, SE,SE,SO);
                }
            };break;     
        }      
    }
    static void _fun25(
                    const ArrayVec& tri,
                    const ArrayVec& seg,
                    const int& c0, const int& c1,
                    MatIC& ml){
        short zero, one, _S1, _S2;
        int c[2] = {c0, c1};
        bool upper = c1>c0;
        zero = upper?0:1; 
        one  = upper?1:0;
        _S1  = upper?S1:S2; 
        _S2  = upper?S2:S1;
        short BP, BO, BN, EN, EV, EP, IdxV;

        BP = 6, BO = 5, BN = TO; 
        if(c[one] == 5){
            EN = 4, EV = 2, EP = 5;
            IdxV  = 1; 
        }else{ // == 7
            EN = 3, EV = 1, EP = 5;
            IdxV  = 0;
        }
        short rc0 = _on_which_side(
            tri[1], seg[zero], tri[0]);
        short rc1;
        if(c[one] == 5){
            rc1= _on_which_side(
                seg[one], tri[IdxV], seg[zero]);
        }else{
            rc1 = _on_which_side(
                seg[zero], tri[IdxV], seg[one]);
        }
        switch(rc0){
            case _P_:{
                ml[zero][0] = BP;
                ml[zero][1] = _S1; 
                ml[one][0]  = EN; 
                ml[one][1]  = SE; 
            };break;
            case _O_:{
                ml[zero][0] = BO;
                ml[zero][1] = _S1; 
                ml[one][0]  = EN; 
                ml[one][1]  = SE; 
            };break;
            case _N_:{
                ml[zero][0] = _SideToLoc(rc1, TO, EV, EP); 
                ml[zero][1] = _SideToLoc(rc1, SO, SE, SE); 
                ml[one][0]  = _SideToLoc(rc1, TO, EV, EN); 
                ml[one][1]  = _SideToLoc(rc1, SO, SE, SE); 
            };break;
        }
    }
    static void _fun34(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatIC& ml){
        short zero, one, _S1;
        int c[2] = {c0, c1};
        bool upper = c1>c0;
        zero = upper?0:1; 
        one  = upper?1:0;
        _S1  = upper?S1:S2; 
        // _S2  = upper?S2:S1;
        short BP, BO;

        if(c[zero] == 3){
            BP = 4, BO = 2;
        }else{ // 1 
            BP = 3, BO = 1;
        }
        short rc0 = _on_which_side(
            tri[1], seg[zero], tri[0]);
        switch(rc0){
            case _P_:{
                ml[zero][0] = BP;
                ml[zero][1] = _S1; 
                ml[one][0]  = TO; 
                ml[one][1]  = SO; 
            };break;
            case _O_:{
                ml[zero][0] = BO;
                ml[zero][1] = _S1; 
                ml[one][0]  = TO; 
                ml[one][1]  = SO; 
            };break;
            case _N_:{
                ml[zero][0] = TO; 
                ml[zero][1] = SO; 
                ml[one][0]  = TO; 
                ml[one][1]  = SO; 
            };break;
        }
    
    }
    static void _fun15(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatIC& ml){
        short zero, one, _S1;
        int c[2] = {c0, c1};
        bool upper = c1>c0;
        zero = upper?0:1; 
        one  = upper?1:0;
        _S1  = upper?S1:S2; 
        // _S2  = upper?S2:S1;
        short BP, BO;

        if(c[zero] == 1){
            BP = 3, BO = 1;
        }else{ // 5 
            BP = 4, BO = 2;
        }
        short rc0 = _on_which_side(
            tri[1], seg[zero], tri[0]);
        switch(rc0){
            case _P_:{
                ml[zero][0] = BP;
                ml[zero][1] = _S1; 
                ml[one][0]  = 0; 
                ml[one][1]  = SE; 
            };break;
            case _O_:{
                ml[zero][0] = BO;
                ml[zero][1] = _S1; 
                ml[one][0]  = 0; 
                ml[one][1]  = SE; 
            };break;
            case _N_:{
                ml[zero][0] = BO; 
                ml[zero][1] = SE; 
                ml[one][0]  = 0; 
                ml[one][1]  = SE; 
            };break;
        }
    
    }
    static void _fun12_32(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatIC& ml){
        short zero, one, _S1, _S2;
        if(c0 == 1 || c1 == 1){
            zero = c1>c0?0:1; 
            one  = c1>c0?1:0;
            _S1  = c1>c0?S1:S2; 
            _S2  = c1>c0?S2:S1; 
        }else{
            zero = c1<c0?0:1; 
            one  = c1<c0?1:0;
            _S1  = c1<c0?S1:S2; 
            _S2  = c1<c0?S2:S1; 
        }
        short T2 = 1, TE = 3, TE2 = 5; 
        if (c0 == 3 || c1 == 3) {
            T2 = 2, TE = 4;
        }
        short rc0 = _on_which_side(
            tri[1], seg[zero], tri[0]);
        short rc1 = _on_which_side(
            tri[1], seg[one], tri[0]);
        switch(rc0){
            case _P_:{
                ml[zero][0] = TE;
                ml[zero][1] = _S1; 
                ml[one][0]  = _SideToLoc(rc1, TIN, TE2, TE2); 
                ml[one][1]  = _SideToLoc(rc1, _S2, _S2, SE); 
            };break;
            case _O_:{
                ml[zero][0] = T2;
                ml[zero][1] = _S1; 
                ml[one][0]  = _SideToLoc(rc1, TIN, TE2, TO); 
                ml[one][1]  = _SideToLoc(rc1, _S2, _S2, SO); 
            };break;
            case _N_:{
                ml[zero][0] = _SideToLoc(rc1, TE2, TO, TO);
                ml[zero][1] = _SideToLoc(rc1, SE,  SO, SO); 
                ml[one][0]  = _SideToLoc(rc1, TIN, TE2, TO); 
                ml[one][1]  = _SideToLoc(rc1, _S2, _S2, SO); 
            };break;
        }
    }
    static void _fun22(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatIC& ml){
        short TE = 5;
        short rc0 = _on_which_side(
            tri[1], seg[0], tri[0]);
        short rc1 = _on_which_side(
            tri[1], seg[1], tri[0]);
        std::cout << "tc0 = " << rc0 << std::endl;
        std::cout << "tc1 = " << rc1 << std::endl;
        if(rc0 == _N_){
            ml[0][0] = _SideToLoc(rc1, TE, TE, TO);
            ml[0][1] = _SideToLoc(rc1, SE, SO, SO);
            ml[1][0] = _SideToLoc(rc1, TIN,TE, TO);
            ml[1][1] = _SideToLoc(rc1, S2, S2, SO);
        }else if(rc0 == _O_){
            ml[0][0] = TE;
            ml[0][1] = S1;
            ml[1][0] = _SideToLoc(rc1, TIN,TE, TO);
            ml[1][1] = _SideToLoc(rc1, S2, S2, SO);
        }else{ // _P_
            ml[0][0] = TIN;
            ml[0][1] = S1;
            ml[1][0] = _SideToLoc(rc1, TIN,TE, TE);
            ml[1][1] = _SideToLoc(rc1, S2, S2, SE);
        }
    }
    static void _fun11_33(const ArrayVec& tri,
                        const ArrayVec& seg,
                        const int& c0, const int& c1,
                        MatIC& ml){
        short T1 = 0, T2 = 1, TE = 3;
        if(c0 == 3){
            T1 = 0, T2 = 2, TE = 4;
        }
        short rc0 = _on_which_side(
            tri[1], seg[0], tri[0]);
        short rc1 = _on_which_side(
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
                ml[0][1] = _SideToLoc(rc1, SE, S2, SO); 
                ml[1][0] = _SideToLoc(rc1, TE, T2, TO); 
                ml[1][1] = _SideToLoc(rc1, S2, S2, SO); 
            };break;
        }
    }
    // normal intersection
	Point _cal_intersection_point(const Vec& v0, 
                                  const Vec& v1,
                                  const Vec& v2, 
                                  const Vec& v3){
		// The intersection type must be _SS_INTERSECT_
		Vt x0 = v0[0];
		Vt x1 = v1[0];
		Vt x2 = v2[0];
		Vt x3 = v3[0];

		Vt y0 = v0[1];
		Vt y1 = v1[1];
		Vt y2 = v2[1];
		Vt y3 = v3[1];

		double denom = (x0-x1) * (y2- y3) - (y0 - y1) * (x2- x3) + 1e-20;

		double x = ((x0 * y1 - y0 * x1) * (x2 - x3)
				- (x0 - x1) * (x2 * y3 - y2 * x3)) / denom ;
		double y = ((x0 * y1 - y0 * x1) * (y2 - y3)
				- (y0 - y1) * (x2 * y3 - y2 * x3)) / denom;

		return Point(x, y);
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