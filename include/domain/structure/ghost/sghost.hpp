#ifndef S_GHOST_HPP
#define S_GHOST_HPP

#include "domain/domain_define.hpp"

namespace carpio{

template<St DIM, class GRID>
class SGhost_{
public:
    static const St Dim = DIM;
    
    typedef SIndex_<Dim> Index;
    typedef GRID         Grid;

    typedef ArrayListT_<Vt> Arr;
    typedef Point_<Vt, Dim> Poi;
    
    typedef St size_type;
    static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
    static const St NumFace   = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);
    typedef std::function<void(const Index&)> FunIndex;

    SGhost_(){}

    virtual ~SGhost_(){}
    virtual std::string type() const{
        return "SGhost";
    }
    virtual St type(const Index& idx) const{}

    virtual Grid& grid() {
        SHOULD_NOT_REACH;
        return Grid();
    }

    virtual const Grid& grid() const {
        SHOULD_NOT_REACH;
        return Grid();
    }

    virtual bool is_ghost(const Index& index) const{};
    virtual bool is_not_ghost(const Index& index) const{};
    virtual bool is_boundary(
            const Index& index,
            const St&    a,
            const St&    o) const{};
    virtual bool is_cut(
            const Index& index) const{};
    virtual bool is_normal(const Index& index) const{};


    virtual int boundary_id(const Index& indexc,
                            const Index& indexg,
                            const St&  axe,
                            const St& ori) const{};
    virtual Index boundary_index(const Index& indexc,
                                 const Index& indexg,
                                 const St&    axe,
                                 const St&    ori) const{};
    virtual St size_normal() const{}
    virtual St size_not_ghost() const{}
};



}

#endif