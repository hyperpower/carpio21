#ifndef _INTERSECTION_TRITRI_HPP_
#define _INTERSECTION_TRITRI_HPP_

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

/**
 * The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
class SingletonTTHelper
{
    /**
     * The Singleton's constructor/destructor should always be private to
     * prevent direct construction/desctruction calls with the `new`/`delete`
     * operator.
     */
public:
    typedef std::bitset<6> BS;

    std::array<int, 3> _arr012;
    std::array<int, 3> _arr135;
    std::array<int, 3> _arr024;

    std::array<std::array<int,3>, 3> _mapte0e1;
private:
    static SingletonTTHelper * pinstance_;
    static std::mutex mutex_;
protected:
    SingletonTTHelper()
    {
        _init_map_D();
        _init_arridx();
        _init_map_te0e1();
    }
    ~SingletonTTHelper() {}

    // std::string value_;

    std::unordered_map<std::bitset<6>, int> _mapD;

    void _init_map_D(){
        // std::cout<<"init_mapD" << std::endl;
        // 00 = 0, 01 = -, 11 = +
        //                    D2D1D0
        _mapD[BS(std::string("000000"))] = 1; //000
        _mapD[BS(std::string("000001"))] = 2; //00-
        _mapD[BS(std::string("000011"))] = 2; //00+
        _mapD[BS(std::string("000100"))] = 2; //0-0
        _mapD[BS(std::string("000101"))] = 3; //0--
        _mapD[BS(std::string("000111"))] = 4; //0-+
        _mapD[BS(std::string("001100"))] = 3; //0+0
        _mapD[BS(std::string("001101"))] = 4; //0+-
        _mapD[BS(std::string("001111"))] = 3; //0++

        _mapD[BS(std::string("010000"))] = 2; //-00
        _mapD[BS(std::string("010001"))] = 3; //-0-
        _mapD[BS(std::string("010011"))] = 4; //-0+
        _mapD[BS(std::string("010100"))] = 3; //--0
        _mapD[BS(std::string("010101"))] = 0; //---
        _mapD[BS(std::string("010111"))] = 5; //--+
        _mapD[BS(std::string("011100"))] = 4; //-+0
        _mapD[BS(std::string("011101"))] = 5; //-+-
        _mapD[BS(std::string("011111"))] = 5; //-++

        _mapD[BS(std::string("110000"))] = 2; //+00
        _mapD[BS(std::string("110001"))] = 4; //+0-
        _mapD[BS(std::string("110011"))] = 3; //+0+
        _mapD[BS(std::string("110100"))] = 4; //+-0
        _mapD[BS(std::string("110101"))] = 5; //+--
        _mapD[BS(std::string("110111"))] = 5; //+-+
        _mapD[BS(std::string("111100"))] = 3; //++0
        _mapD[BS(std::string("111101"))] = 5; //++-
        _mapD[BS(std::string("111111"))] = 0; //+++

        //0 No intersection 
        //1 all coplane 
        //2 line coplane 
        //3 point coplane 
        //4 one point on plane, other two points on different side 
        //5 points on different side
    }

    void _init_arridx(){
        _arr012[0] = 0;
        _arr012[1] = 1;
        _arr012[2] = 2;

        _arr135[0] = 1;
        _arr135[1] = 3;
        _arr135[2] = 5;

        _arr024[0] = 0;
        _arr024[1] = 2;
        _arr024[2] = 4;
    }

    void _init_map_te0e1(){
        // 0 = 0, + = 1, - = 2
        _mapte0e1[0][0] = 0;
        _mapte0e1[2][0] = 1;
        _mapte0e1[2][2] = 2;
        _mapte0e1[0][2] = 3;
        _mapte0e1[1][2] = 4;
        _mapte0e1[1][0] = 5;
        _mapte0e1[1][1] = 6;
        _mapte0e1[0][1] = 7;
        _mapte0e1[2][1] = 8;
    }

    

public:
    /**
     * SingletonTTHelpers should not be cloneable.
     */
    SingletonTTHelper(SingletonTTHelper &other) = delete;
    /**
     * SingletonTTHelpers should not be assignable.
     */
    void operator=(const SingletonTTHelper &) = delete;
    /**
     * This is the static method that controls the access to the SingletonTTHelper
     * instance. On the first run, it creates a SingletonTTHelper object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static SingletonTTHelper *GetInstance();
    /**
     * Finally, any SingletonTTHelper should define some business logic, which can be
     * executed on its instance.
     */
    void SomeBusinessLogic()
    {
        // ...
    }
    
    int type_D(const std::bitset<6>& key) const{
        int td = -1;
        try {
            td = _mapD.at(key);
        }catch (std::out_of_range e) {
            std::cerr << e.what() << std::endl;
            return -1;
        }
        return td;
    } 
};

/**
 * Static methods should be defined outside the class.
 */

SingletonTTHelper* SingletonTTHelper::pinstance_{nullptr};
std::mutex SingletonTTHelper::mutex_;

/**
 * The first time we call GetInstance we will lock the storage location
 *      and then we make sure again that the variable is null and then we
 *      set the value. RU:
 */
SingletonTTHelper *SingletonTTHelper::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new SingletonTTHelper();
    }
    return pinstance_;
}

template<typename TYPE, St DIM>
class IntersectionTriTri_:public IntersectionBase_<Triangle_<TYPE, DIM>, Triangle_<TYPE, DIM> >{
public:
    typedef IntersectionBase_<Triangle_<TYPE, DIM>, Triangle_<TYPE, DIM> > Base;
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
    spTri _t1;
    spTri _t2;

    std::array<Vec, 3> _vr; // rebased triangle
    std::array<Vec, 3> _ve; // rebased triangle
    Vec _n0;
    std::array<Vt,  3> _D;

    std::array<Vec, 2> _vt;  // intersection points on plane t2

    //tmp
    std::array<std::array<spVec, 2>, 2> _mte;
    std::array<std::array<spVec, 2>, 2> _met;
    
public:
    IntersectionTriTri_():Base(){};

    IntersectionTriTri_(const Object1& obj1, const Object2& obj2){
        _set_triangles(obj1, obj2);
        this->_inited = true;
    };

    ReturnType is_intersect(){
        if(!this->is_init()){
            std::cerr << "!> Triangles are not initialed" << std::endl;
            return this->no_intersections();
        }
        this->_rebase();
        // _n0
        _n0 = Cross(_ve[0], _ve[1]);
        std::cout << "n0 = " << _n0 << std::endl;

        //calculate D
        _D[0] = Dot(_vr[0],_n0); 
        _D[1] = Dot(_vr[1],_n0); 
        _D[2] = Dot(_vr[2],_n0); 
        
        std::cout << _D[0] << ", " << _D[1] << ", " << _D[2] << std::endl;
        std::bitset<6> Dcode(0);
        _D_code(_D[0], 0, Dcode);
        _D_code(_D[1], 1, Dcode);
        _D_code(_D[2], 2, Dcode);
        std::cout << "Dcode: " << Dcode << '\n';

        //
        SingletonTTHelper* phelper = SingletonTTHelper::GetInstance();
        int td = phelper->type_D(Dcode);
        std::cout << "type d = " << td << std::endl;
        // D type cases
        switch (td) {
            case 0: 
                return this->no_intersections();
            case 1:
                return _all_coplane();
            case 2:
                return _line_coplane(Dcode);
            case 3:
                return _point_coplane(Dcode);
            case 4:
                return _point_coplane_different_side(Dcode);
            case 5:
                return _different_side(Dcode);
        }
        
        return this->no_intersections();

    }
protected:
    void _set_triangles(const Object1& obj1, const Object2& obj2){
        _t1 = std::make_shared<Triangle>(obj1);
        _t2 = std::make_shared<Triangle>(obj2);
    }

    // 1 re-origin points
    //   set _t2[0] as the origin point
    void _rebase(){
        _vr[0] = (*_t1)[0] - (*_t2)[0];
        _vr[1] = (*_t1)[1] - (*_t2)[0];
        _vr[2] = (*_t1)[2] - (*_t2)[0];

        _ve[0] = (*_t2)[1] - (*_t2)[0];
        _ve[1] = (*_t2)[2] - (*_t2)[0];
        // _ve[2].assign(0)
    }

    int _D_code(Vt D, int idx, std::bitset<6>& bset){
        if(D > 0.0){
            bset.set(idx * 2,     1);
            bset.set(idx * 2 + 1, 1);
            return 3; // 11
        }else if(D < 0.0){
            bset.set(idx * 2, 1);
            return 1; // 01
        }else{
            return 0; // 00
        } 
    }

    int index_t_e_n(const Vec& t, const Vec& e, const Vec& n){
        auto res = Dot(Cross(t,e), n);
        std::cout << res << std::endl;
        if(res > 0.0){
            return 1;
        }else if(res < 0.0){
            return 2;
        }else{
            return 0;
        }
    }

    
    ReturnType _all_coplane(){
        std::cout << "In coplane "<< std::endl;
        return ReturnType();
    } 
    ReturnType _line_coplane(const std::bitset<6>& D){
        std::cout << "In line coplane "<< std::endl;
        int idx  = 0;
        
        SingletonTTHelper* phelper = SingletonTTHelper::GetInstance();
        for(auto& i : phelper->_arr012){
            if(idx > 1){
                break;
            }
            if(   D[phelper->_arr024[i]] == 0 
               && D[phelper->_arr135[i]] == 0){
                _vt[idx] = _vr[i];
                idx++;
            }
        }
        return ReturnType();
    } 
    ReturnType _point_coplane(const std::bitset<6>& D){
        std::cout << "In point coplane "<< std::endl;
        
        SingletonTTHelper* phelper = SingletonTTHelper::GetInstance();
        for(auto& i : phelper->_arr012){
            if(   D[phelper->_arr024[i]] == 0 
               && D[phelper->_arr135[i]] == 0){
                _vt[0] = _vr[i];
                break;
            }
        }
        return ReturnType();
    }

    ReturnType _point_coplane_different_side(const std::bitset<6>& D){
        std::cout << "In one point coplane different side"<< std::endl;
        
        SingletonTTHelper* phelper = SingletonTTHelper::GetInstance();

        for(auto& i : phelper->_arr012){
            if(   D[phelper->_arr024[i]] == 0 
               && D[phelper->_arr135[i]] == 0){
                _vt[0] = _vr[i];
                break;
            }
        }  // get one point coplane


        cal_vt(_vt[1], 1, 2);
        std::cout << "vt[0] = " << _vt[0] << std::endl;
        std::cout << "vt[1] = " << _vt[1] << std::endl;

        // index (t0 x e0) . n0
        int idxt00 = index_t_e_n(_vt[0], _ve[0], _n0);
        int idxt01 = index_t_e_n(_vt[0], _ve[1], _n0);

        int idxt10 = index_t_e_n(_vt[1], _ve[0], _n0);
        int idxt11 = index_t_e_n(_vt[1], _ve[1], _n0);

        auto t0e = phelper->_mapte0e1[idxt00][idxt01];
        auto t1e = phelper->_mapte0e1[idxt10][idxt11];
        std::cout << "(t0e) = " << t0e << std::endl;
        std::cout << "(t1e) = " << t1e << std::endl;

        return ReturnType();
    }

    ReturnType _different_side(const std::bitset<6>& D){
        std::cout << "In different side"<< std::endl;

        return ReturnType();
    }
    // calculate vt
    // return res
    void cal_vt(Vec& res, int i, int j) const{
        Vt beta = _D[i]/(_D[i] - _D[j]);
        // std::cout << "beta = " << beta << std::endl;
        res = _vr[i] * beta + _vr[j] * (1.0 - beta);
        // std::cout << res << std::endl;
    }
};

}

#endif