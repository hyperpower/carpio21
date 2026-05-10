#ifndef S_STENCIL_2X2_HPP
#define S_STENCIL_2X2_HPP

#include "domain/structure/stencil/sstencil.hpp"
#include "domain/direction.hpp"
#include "domain/structure/field/sfield_center.hpp"
#include "domain/structure/operator/sapply_bc.hpp"

namespace carpio{

template<class VT>
class SStencil2x2_ : public SStencil_<2, VT>{
public:
    typedef SStencilTag Tag;
    static const St Dim = 2;

    typedef SStencil_<2, VT> Base;
    typedef SStencil2x2_<VT> Self;

    typedef Indices_<2> Indices;
    typedef typename Base::ValueType ValueType;
    typedef typename Base::_DataInit _DataInit;

    typedef std::array<Indices, 4> StencilIndices; // for 2x2 stencil, there are 4 points
    typedef std::array<ValueType, 4> StencilValue; // corresponding values at the 4 points    
protected:
    CornerDirection _dir; 
    StencilIndices _relative_idx; // signed offsets of the 4 points in the stencil
    StencilValue _val; // values at the 4 points in the stencil
public:
    SStencil2x2_(const CornerDirection& dir):
        Base(),
        _dir(dir),
        _relative_idx({Indices(0,0), Indices(0,0), Indices(0,0), Indices(0,0)}),
        _val({_DataInit::InitZero(Indices(0,0)),
              _DataInit::InitZero(Indices(0,1)),
              _DataInit::InitZero(Indices(1,0)),
              _DataInit::InitZero(Indices(1,1))}) {
        const auto a1 = _dir.axes_first();
        const auto a2 = _dir.axes_second();

        _relative_idx[1][a1] = ToOffsetStep(_dir.orientation(a1));
        _relative_idx[2][a2] = ToOffsetStep(_dir.orientation(a2));
        _relative_idx[3][a1] = _relative_idx[1][a1];
        _relative_idx[3][a2] = _relative_idx[2][a2];
    }

    template<class FIELD>
    void set_value(
        const FIELD& field,
        const typename FIELD::Index& idxc,
        const BoundaryIndex& bi,
        const Vt& time = 0.0)
    {
        std::array<typename FIELD::Index, 4> _aidx{
            Base::_absolute_index(idxc, _relative_idx[0]),
            Base::_absolute_index(idxc, _relative_idx[1]),
            Base::_absolute_index(idxc, _relative_idx[2]),
            Base::_absolute_index(idxc, _relative_idx[3])};

        _val[0] = field(_aidx[0]); 

        std::array<bool, 2> _is_ghost = {false, false};
        for(St i = 1; i < _aidx.size() - 1; ++i){
            if(field.ghost().is_ghost(_aidx[i])){
                _is_ghost[i - 1] = true;
                const auto axe = _dir.axes[i - 1]; 
                const auto ori = _dir.orientation(axe);
                _val[i] = Value(field, bi, idxc, _aidx[i], axe, ori, time);
            }else{
                _val[i] = field(_aidx[i]);
            }
        }

        if(!(field.ghost().is_ghost(_aidx[3]))){
            _val[3] = field(_aidx[3]);
            this->_value_setted = true;
            return;
        }else{
            if(_is_ghost[0] && _is_ghost[1]){
                // if both idxg1 and idxg2 are ghost
                _val[3] = (_val[1] + _val[2]) * 0.5;
            }else if(!_is_ghost[0] && !_is_ghost[1]){
                // if both idxg1 and idxg2 are normal, then idxg3 must be normal as well
                auto _val1 = Value(field, bi, 
                              _aidx[1], _aidx[3], 
                              _dir.axes_second(), 
                              _dir.orientation(_dir.axes_second()), time);
                auto _val2 = Value(field, bi,
                              _aidx[2], _aidx[3],
                              _dir.axes_first(),
                              _dir.orientation(_dir.axes_first()), time);
                _val[3] = (_val1 + _val2) * 0.5;
            }else if(_is_ghost[0] && !_is_ghost[1]){
                // if idxg1 is ghost and idxg2 is normal, then idxg3 must be ghost as well
                _val[3] = Value(field, bi, 
                              _aidx[2], _aidx[3], 
                              _dir.axes_first(), 
                              _dir.orientation(_dir.axes_first()), time);
            }else if(!_is_ghost[0] && _is_ghost[1]){
                // if idxg1 is normal and idxg2 is ghost, then idxg3 must be ghost as well
                _val[3] = Value(field, bi, 
                              _aidx[1], _aidx[3], 
                              _dir.axes_second(), 
                              _dir.orientation(_dir.axes_second()), time);
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

    template<class FIELD>
    VT grad(const FIELD& field, const Axes& axe) const {
        ASSERT(this->_value_setted);
        EXPAND_FIELD(FIELD);
        EXPAND_FIELD_TAG(FIELD);
        return _grad(field, axe, GridTag());
    }

protected:
    template<class FIELD>
    VT _grad(const FIELD& field, const Axes& axe, SGridUniformTag) const {
        EXPAND_FIELD(FIELD);

        const auto a1 = _dir.axes_first();
        const auto a2 = _dir.axes_second();
        const auto& grid = field.grid();
        const auto d = grid.dc();
        if(axe == a1){
            int sign = (_dir.orientation(a1) == _P_) ? 1 : -1;
            return (sign * (_val[3] - _val[2]) + sign * (_val[1] - _val[0])) / (2.0 * d);
        }else if(axe == a2){
            int sign = (_dir.orientation(a2) == _P_) ? 1 : -1;
            return (sign * (_val[3] - _val[1]) + sign * (_val[2] - _val[0])) / (2.0 * d);
        }else{
            SHOULD_NOT_REACH;
            return 0.0;
        }
    }    
};

}


#endif
