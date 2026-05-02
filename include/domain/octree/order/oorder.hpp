#ifndef _O_ORDER_HPP_
#define _O_ORDER_HPP_

#include "domain/octree/octree_define.hpp"

#include <type_traits>
#include <utility>
#include <vector>

namespace carpio {

struct OOrderTag : public OctreeTag, public OrderBaseTag {};

template<class T, class = void>
struct HasIdx : std::false_type {};

template<class T>
struct HasIdx<T, std::void_t<decltype(std::declval<const T&>().idx())>>
    : std::bool_constant<
          std::is_convertible_v<decltype(std::declval<const T&>().idx()), St>
      > {};

template<class GRID, class GHOST, class LOCTAG>
class OOrder_ : public OrderBase_<GRID::Dim, LOCTAG> {
public:
    typedef GRID Grid;
    typedef GHOST Ghost;
    static const St Dim = Grid::Dim;

    typedef OOrderTag Tag;
    typedef LOCTAG LocationTag;
    typedef typename Grid::DimTag DimTag;
    typedef std::shared_ptr<Grid> spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef typename Grid::Node Node;
    typedef typename Grid::pNode pNode;
    typedef typename Grid::const_pNode const_pNode;
    typedef typename Grid::Indices Indices;

    typedef std::vector<const_pNode> OrderVector;
    typedef typename OrderVector::iterator iterator;
    typedef typename OrderVector::const_iterator const_iterator;

protected:
    spGrid _grid;
    spGhost _ghost;

public:
    OOrder_() {
    }

    OOrder_(spGrid spgrid, spGhost spghost) :
        _grid(spgrid),
        _ghost(spghost) {
    }

    virtual ~OOrder_() = default;

    virtual St size() const {
        SHOULD_NOT_REACH;
        return 0;
    }

    virtual iterator begin() {
        return iterator();
    }

    virtual iterator end() {
        return iterator();
    }

    virtual const_iterator begin() const {
        return const_iterator();
    }

    virtual const_iterator end() const {
        return const_iterator();
    }

    virtual St get_order(const_pNode) const {
        return 0;
    }

    const Grid& grid() const {
        return *_grid;
    }

    const Ghost& ghost() const {
        return *_ghost;
    }

    spGrid spgrid() const {
        return _grid;
    }

    spGhost spghost() const {
        return _ghost;
    }

};

}

#endif
