#ifndef S_STENCIL_3X3_HPP
#define S_STENCIL_3X3_HPP

#include "domain/structure/stencil/sstencil.hpp"
#include "domain/direction.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/regular.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/operator/sapply_bc.hpp"
#include "algebra/array/multi_array.hpp"

namespace carpio{

template<class VT>
class SStencil3x3_ : public SStencil_<2, VT>{
public:
    typedef SStencilTag Tag;
    static const St Dim = 2;

    typedef SStencil_<2, VT> Base;
    typedef SStencil3x3_<VT> Self;

    typedef Indices_<2> Indices;
    typedef typename Base::ValueType ValueType;
    typedef typename Base::_DataInit _DataInit;

    typedef MultiArray_<Indices, 2> StencilIndices; // for 3x3 stencil, there are 9 points
    typedef MultiArray_<ValueType, 2> StencilValue; // corresponding values at the 9 points
protected:
    StencilIndices _relative_idx; // signed offsets of the 9 points in the stencil
    StencilValue _val; // values at the 9 points in the stencil
public:
    SStencil3x3_():
        Base(),
        _relative_idx(3, 3),
        _val(3, 3) {
        for(St j = 0; j < _relative_idx.size_j(); ++j){
            for(St i = 0; i < _relative_idx.size_i(); ++i){
                const Indices rel(static_cast<int>(i) - 1, static_cast<int>(j) - 1);
                _relative_idx(i, j) = rel;
                _val(i, j) = _DataInit::InitZero(rel);
            }
        }
    }

    template<class FIELD>
    void set_value(
        const FIELD& field,
        const typename FIELD::Index& idxc,
        const BoundaryIndex& bi,
        const Vt& time = 0.0)
    {
        for(St j = 0; j < _relative_idx.size_j(); ++j){
            for(St i = 0; i < _relative_idx.size_i(); ++i){
                const auto& rel = _relative_idx(i, j);
                const auto aidx = Base::_absolute_index(idxc, rel);
                _val(i, j) = _value(field, bi, idxc, aidx, rel, time);
            }
        }
        this->_value_setted = true;
    }

    VT average() const {
        ASSERT(this->_value_setted);
        VT sum = 0;
        for(const auto& v : _val){
            sum += v;
        }
        return sum / _val.size();
    }

    VT sum_y(const St i) const {
        ASSERT(this->_value_setted);
        VT sum = 0;
        for(St j = 0; j < _val.size_j(); ++j){
            sum += _val(i, j);
        }
        return sum;
    }

    VT sum_y(const Orientation& ori) const {
        ASSERT(this->_value_setted);
        if(ori == _M_){
            return sum_y(0); 
        }else if(ori == _P_){
            return sum_y(2);
        }else{
            return sum_y(1);
        }
    }

    VT sum_x(const St j) const {
        ASSERT(this->_value_setted);
        VT sum = 0;
        for(St i = 0; i < _val.size_i(); ++i){
            sum += _val(i, j);
        }
        return sum;
    }

    VT sum_x(const Orientation& ori) const {
        ASSERT(this->_value_setted);
        if(ori == _M_){
            return sum_x(0);
        }else if(ori == _P_){
            return sum_x(2);
        }else{
            return sum_x(1);
        }
    }


    template<class FIELD>
    VT grad(const FIELD& field, const Axes& axe) const {
        ASSERT(this->_value_setted);
        EXPAND_FIELD(FIELD);
        EXPAND_FIELD_TAG(FIELD);
        return _grad(field, axe, GridTag());
    }

protected:
    template<class FIELD>
    ValueType _value(
        const FIELD& field,
        const BoundaryIndex& bi,
        const typename FIELD::Index& idxc,
        const typename FIELD::Index& idx,
        const Indices& rel,
        const Vt& time) const
    {
        if(!(field.ghost().is_ghost(idx))){
            return field(idx);
        }

        if(OnSameAxe(rel)){
            const auto axe = GetDeltaAxe(rel);
            const auto ori = GetDeltaOrientOnSameAxe(rel);
            return Value(field, bi, idxc, idx, axe, ori, time);
        }

        ValueType sum_normal = _DataInit::InitZero(idxc);
        St count_normal = 0;
        for(St d = 0; d < Dim; ++d){
            if(rel[d] == 0){
                continue;
            }
            const auto idxn = idx.shift(d, -Sign(rel[d]));
            if(!(field.ghost().is_ghost(idxn))){
                const auto axe = ToAxes(d);
                const auto ori = ToOrientationSign(rel[d]);
                sum_normal += Value(field, bi, idxn, idx, axe, ori, time);
                ++count_normal;
            }
        }
        if(count_normal > 0){
            return sum_normal / count_normal;
        }

        ValueType sum_ghost = _DataInit::InitZero(idxc);
        St count_ghost = 0;
        for(St d = 0; d < Dim; ++d){
            if(rel[d] == 0){
                continue;
            }
            const auto idxn = idx.shift(d, -Sign(rel[d]));
            auto reln = rel;
            reln[d] = 0;
            sum_ghost += _value(field, bi, idxc, idxn, reln, time);
            ++count_ghost;
        }
        ASSERT(count_ghost > 0);
        return sum_ghost / count_ghost;
    }

    template<class FIELD>
    VT _grad(const FIELD& field, const Axes& axe, SGridUniformTag) const {
        EXPAND_FIELD(FIELD);

        const auto& grid = field.grid();
        const auto d = grid.dc();
        if(axe == _X_){
            return ((_val(2, 0) - _val(0, 0))
                  + (_val(2, 1) - _val(0, 1))
                  + (_val(2, 2) - _val(0, 2))) / (6.0 * d);
        }else if(axe == _Y_){
            return ((_val(0, 2) - _val(0, 0))
                  + (_val(1, 2) - _val(1, 0))
                  + (_val(2, 2) - _val(2, 0))) / (6.0 * d);
        }else{
            SHOULD_NOT_REACH;
            return 0.0;
        }
    }
};

template<class VT>
using SStencil_3x3 = SStencil3x3_<VT>;

}


#endif
