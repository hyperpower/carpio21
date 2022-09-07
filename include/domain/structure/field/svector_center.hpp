#ifndef S_VECTOR_CENTER_HPP
#define S_VECTOR_CENTER_HPP

#include "sfield_center.hpp"

namespace carpio{

struct SVectorCenterTag: public SFieldTag{};

template<St DIM, 
         class VT,
         class GRID, 
         class GHOST, 
         class ORDER>
class SVectorCenter_{
public:
    static const St Dim = DIM;

    typedef SFieldCenter_<DIM, VT, GRID, GHOST, ORDER> Field;
    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef typename Grid::Index Index;

    typedef std::shared_ptr<Index> spIndex;
    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;

    typedef SVectorCenter_<DIM, VT, GRID, GHOST, ORDER> Self;
    typedef std::shared_ptr<Field> spField;

protected:
    std::array<spField, DIM> _arrs;
public:
    SVectorCenter_() {
        FOR_EACH_DIM{
            _arrs[d] = nullptr;
        }
    }

    SVectorCenter_(
            spField u,
            spField v = nullptr,
            spField w = nullptr){
        spField a[] = {u,v,w};
        FOR_EACH_DIM{
            ASSERT(a[d] != nullptr);
            _arrs[d] = a[d];
        }
    }

    void set(Axes a, spField sps){
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

    Field& operator[](St d){
        return *(_arrs[d]);
    }

    const Field& operator[](St d) const{
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
        if(DIM == 1){
            return max();
        }else if(DIM == 2){
            auto sum = SqareSum(*(_arrs[_X_]), *(_arrs[_Y_]));
            return std::sqrt(sum.max());
        }else{
            auto sum = SqareSum(_arrs[_X_], _arrs[_Y_], _arrs[_Z_]);
            return std::sqrt(sum.max());
        }
    }
};


}

#endif
