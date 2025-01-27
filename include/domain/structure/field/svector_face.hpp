#ifndef S_VECTOR_FACE_HPP
#define S_VECTOR_FACE_HPP

#include "sfield_face.hpp"

namespace carpio{

struct SVectorFaceTag: public SFieldTag{};

template<St DIM, 
         class VT,
         class GRID, 
         class GHOST, 
         class ORDER>
class SVectorFace_{
public:
    static const St Dim = DIM;

    typedef SFieldFace_<DIM, VT, GRID, GHOST, ORDER> Component;
    typedef SVectorFaceTag Tag;
    typedef typename DimTagTraits_<Dim>::Type DimTag;
    typedef VT ValueType;
    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef typename Grid::Index Index;

    typedef std::shared_ptr<Index> spIndex;
    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;

    typedef SVectorFace_<DIM, VT, GRID, GHOST, ORDER> Self;
    typedef std::shared_ptr<Component> spComponent;

    typedef typename std::array<spComponent, DIM>::iterator iterator;
    typedef typename std::array<spComponent, DIM>::const_iterator const_iterator;

protected:
    std::array<spComponent, DIM> _arrs;
public:
    SVectorFace_() {
        FOR_EACH_DIM{
            _arrs[d] = nullptr;
        }
    }

    SVectorFace_(
            spComponent u,
            spComponent v = nullptr,
            spComponent w = nullptr){
        spComponent a[] = {u,v,w};
        FOR_EACH_DIM{
            ASSERT(a[d] != nullptr);
            _arrs[d] = a[d];
        }
    }

    ValueType& operator()(Axes a, Orientation o,
                   St i, St j = 0, St k = 0) {
        ASSERT(a < DIM);
        return (*_arrs[a])(o, i, j, k);
    }
    const ValueType& operator()(Axes a, Orientation o,
                   St i, St j = 0, St k = 0) const{
        ASSERT(a < DIM);
        return (*_arrs[a])(o, i, j, k);
    }
    ValueType& operator()(Axes a, Orientation o, const Index& index){
        ASSERT(a < DIM);
        return (*_arrs[a])(o, index);
    }

    const ValueType& operator()(Axes a, Orientation o, const Index& index) const{
        ASSERT(a < DIM);
        return (*_arrs[a])(o, index);
    }
    
    void set(Axes a, spComponent sps){
        ASSERT(a < DIM);
        _arrs[a] = sps;
    }

    const Order& order() const {
        return _arrs[_X_]->order();
    }

    const Grid& grid() const {
        return _arrs[_X_]->grid();
    }

    const Ghost& ghost() const {
        return _arrs[_X_]->ghost();
    }

    Component& operator[](St d){
        return *(_arrs[d]);
    }

    const Component& operator[](St d) const{
        return *(_arrs[d]);
    }

    Vt max() const{
        Vt m = _arrs[_X_]->max();
        for(St d = 1; d< DIM; d++){
            Vt md = _arrs[d]->max();
            if(m < md){
                m = md;
            }
        }
        return m;
    }

    Vt max_norm2() const{
        if constexpr (DIM == 1){
            return max();
        }else if constexpr (DIM == 2){
            auto sum = SqareSum(*(_arrs[_X_]), *(_arrs[_Y_]));
            return std::sqrt(sum.max());
        }else{
            auto sum = SqareSum(_arrs[_X_], _arrs[_Y_], _arrs[_Z_]);
            return std::sqrt(sum.max());
        }
    }

    iterator begin(){
        return _arrs.begin();
    }
    const_iterator begin() const{
        return _arrs.begin();
    }
    iterator end(){
        return _arrs.end();
    }
    const_iterator end() const{
        return _arrs.end();
    }
};

}

#endif