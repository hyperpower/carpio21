#ifndef S_GRID_HPP
#define S_GRID_HPP

#include "domain/domain_define.hpp"

#include "algebra/algebra.hpp"
#include "geometry/geometry.hpp"

namespace carpio{
    
struct SGridTag:  public StructureTag{};

template<St DIM>
class SGrid_ {
public:
    static const St Dim = DIM;
    typedef SGridTag Tag; 
    typedef SIndex_<Dim> Index;

    typedef ArrayListT_<Vt> Arr;
    typedef Point_<Vt, Dim> Point;
    typedef St size_type;
    static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
    static const St NumFace   = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);
    typedef std::function<void(const Index&)> FunIndex;
public:
    SGrid_(){}

    virtual ~SGrid_(){}

    virtual std::string type_name() const{
        return "SGrid";
    };
    virtual Point original() const{return Point();};
    virtual St dim() const{return Dim;};
    // ghost layer ============================
    virtual Idx ghost_layer() const{return 0;};
    // index ==================================
    virtual Index n() const{return Index();};
    virtual Index N() const{return Index();};
    virtual Idx n(St) const{return 0;};
    virtual Idx N(St) const{return 0;};

    // size ===================================
    virtual inline Vt s_(St,   Idx) const{return 0;};
    virtual inline Vt s_(St, Index) const{return 0;};
    virtual inline Vt s_(Index, St) const{return 0;};

    // half size
    virtual inline Vt hs_(St, Idx) const {return 0;};
    virtual inline Vt hs_(St, Index) const {return 0;};
    // volume ==================================
    virtual inline Vt volume(Index) const {return 0;};
    // center ==================================
    virtual Point c (Idx i, Idx j = 0, Idx k = 0)     const {return Point();}
    virtual Point c (const Index&)              const {return Point();}
    virtual Vt  c_(const St& dim, const Idx& idx)   const {return 0;}
    virtual Vt  c_(const St& dim, const Index& idx) const {return 0;}

    virtual Vt cx(Idx i, Idx j = 0, Idx k = 0) const {return 0;}
    virtual Vt cy(Idx i, Idx j = 0, Idx k = 0) const {return 0;}
    virtual Vt cz(Idx i, Idx j = 0, Idx k = 0) const {return 0;}

    virtual St num_cells()  const  {return 0;}
    virtual St num_vertex() const  {return 0;}
    virtual St num_face()   const  {return 0;}

    virtual Vt min_size()   const {return 0;}
    virtual Vt min_size(St) const {return 0;}

    // face  ===================================
    virtual Point f (St, int, const Index&) const {return Point();}
    virtual Point f (St, int, Idx, Idx j = 0, Idx k = 0) const {return Point();}
    virtual Vt  f_(St, int, Idx) const {return 0;}
    // face area
    virtual Vt  fa(St, int, const Index&) const {return 0;}

    // distance to face
    virtual Vt df_(St, Idx) const {return 0;}
    // vertex ================================
    virtual Point v(Idx order, Idx i, Idx j = 0, Idx k = 0) const {return Point();}
    virtual Point v(Idx order, Index index) const{return Point();}

    virtual Point v(Idx i,     short oi,
                  Idx j = 0, short oj = 0,
                  Idx k = 0, short ok = 0) const {return Point();}

    // Point is in the range
    virtual inline bool is_in_on(Point p) {return false;}
    //  find closest index on the negative direction
    //  for example:
    //
    //      1.2   1.4   1.6   1.8   2.0
    //  -----+-----+-----+-----+-----+
    //       0     1     2     3     4
    //                      ^
    //               cor = 1.5
    //  return 2
    virtual inline Idx find_close_idx_m(St dim, Vt cor) {return 0;}
    //  find closest index on the positive direction
    //  for example:
    //
    //      1.2   1.4   1.6   1.8   2.0
    //  -----+-----+-----+-----+-----+
    //       0     1     2     3     4
    //                      ^
    //               cor = 1.5
    //  return 3
    virtual inline Idx find_close_idx_p(St dim, Vt cor) {return 0;}

    virtual inline St _IDX(const Idx&)   const {return 0;}
    virtual inline Idx _idx(const St&)   const {return 0;}
    virtual Index to_Index(const Index&) const{return Index();}
    virtual Index to_INDEX(const Index&) const{return Index();}

    virtual void for_each(FunIndex){}
    virtual void for_each(FunIndex) const{}
    virtual void for_each_INDEX(FunIndex){}



};

}



#endif