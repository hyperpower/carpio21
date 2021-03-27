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
private:
    static SingletonTTHelper * pinstance_;
    static std::mutex mutex_;
protected:
    SingletonTTHelper()
    {
        _init_mapD();
    }
    ~SingletonTTHelper() {}

    // std::string value_;

    std::unordered_map<std::bitset<6>, int> _mapD;

    void _init_mapD(){
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
    typedef std::shared_ptr<Point_<TYPE, DIM> >    spPoint;     
    typedef std::shared_ptr<Triangle>              spTri;     
protected:
    spTri _t1;
    spTri _t2;

    std::array<Vec, 3> _vr;
    std::array<Vec, 3> _ve;
    Vec _n0;
    std::array<Vt,  3> _D;
public:
    IntersectionTriTri_():Base(){};

    IntersectionTriTri_(const Object1& obj1, const Object2& obj2){
        _set_triangles(obj1, obj2);
        this->_inited = true;
    };

    bool is_intersect(){
        if(!this->is_init()){
            std::cerr << "!> Triangles are not initialed" << std::endl;
            return false;
        }
        this->_rebase();
        // _n0
        _n0 = Cross(_ve[0], _ve[1]);

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
        
        return false;

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

};



}

#endif