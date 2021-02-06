#ifndef S_GRID_HPP
#define S_GRID_HPP

#include "domain/domain_define.hpp"

#include "algebra/algebra.hpp"
#include "geometry/geometry.hpp"

namespace carpio{

template<St DIM>
class SGrid_ {
public:
    static const St Dim = DIM;
    
    typedef SIndex_<Dim> Index;

    typedef ArrayListT_<Vt> Arr;
    typedef Point_<Vt, Dim> Poi;
    typedef St size_type;
    static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
    static const St NumFace = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);
    typedef std::function<void(const Index&)> FunIndex;
public:
    SGrid_(){}

    virtual ~SGrid_(){}

    virtual std::string type_name() const{
        return "SGrid";
    };
    // ghost layer ============================
    virtual Idx ghost_layer() const{};
    // index ==================================
    virtual const Index& n() const{};
    virtual const Index& N() const{};
    virtual Idx n(St dim) const{};
    virtual Idx N(St dim) const{};

    // size ===================================
    virtual inline Vt s_(St dim, Idx i) const{};
    virtual inline Vt s_(St dim, Index idx) const{};

    // half size
    virtual inline Vt hs_(St dim, Idx i) const {};
    virtual inline Vt hs_(St dim, Index idx) const {};
    // volume ==================================
    virtual inline Vt volume(Index idx) const {};
    // center ==================================
    virtual Poi c(Idx i, Idx j = 0, Idx k = 0) const {}
    virtual Poi c(const Index& index) const {}
    virtual Vt  c_(const St& dim, const Idx& idx) const {}
    virtual Vt  c_(const St& dim, const Index& idx) const {}

    virtual Vt cx(Idx i, Idx j = 0, Idx k = 0) const {}
    virtual Vt cy(Idx i, Idx j = 0, Idx k = 0) const {}
    virtual Vt cz(Idx i, Idx j = 0, Idx k = 0) const {}

    virtual St num_cells() const {}
    virtual St num_vertex() const {}
    virtual St num_face() const {}

    virtual Vt min_size() const {}
    virtual Vt min_size(St dim) const {}

    // face  ===================================
    virtual Poi f(St dim,  int ori, const Index& index) const {}
    virtual Poi f(St dim,  int fb, Idx i, Idx j = 0, Idx k = 0) const {}
    virtual Vt  f_(St dim, int ori, Idx idx) const {}
    // face area
    virtual Vt  fa(St dim,  int ori, const Index& index) const {}

    // distance to face
    virtual Vt df_(St dim, Idx idx) const {}
    // vertex ================================
    virtual Poi v(Idx order, Idx i, Idx j = 0, Idx k = 0) const {}
    virtual Poi v(Idx order, Index index) const{}

    virtual Poi v(Idx i,     short oi,
                  Idx j = 0, short oj = 0,
                  Idx k = 0, short ok = 0) const {
    }


    // Point is in the range
    virtual inline bool is_in_on(Poi p) {

    }
    //  find closest index on the negative direction
    //  for example:
    //
    //      1.2   1.4   1.6   1.8   2.0
    //  -----+-----+-----+-----+-----+
    //       0     1     2     3     4
    //                      ^
    //               cor = 1.5
    //  return 2
    virtual inline Idx find_close_idx_m(St dim, Vt cor) {

    }
    //  find closest index on the positive direction
    //  for example:
    //
    //      1.2   1.4   1.6   1.8   2.0
    //  -----+-----+-----+-----+-----+
    //       0     1     2     3     4
    //                      ^
    //               cor = 1.5
    //  return 3
    virtual inline Idx find_close_idx_p(St dim, Vt cor) {

    }

    virtual inline St _IDX(const Idx& i) const {
    }
    virtual inline Idx _idx(const St& I) const {
    }
    virtual Index to_Index(const Index& INDEX) const{}
    virtual Index to_INDEX(const Index& Index) const{}

    virtual void for_each(FunIndex){}
    virtual void for_each_INDEX(FunIndex){}



};

}



#endif