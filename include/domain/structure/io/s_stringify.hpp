#ifndef _S_STRINGIGY_HPP
#define _S_STRINGIGY_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include <string>

#include "domain/domain_define.hpp"

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/grid/uniform.hpp"
#include "domain/structure/grid/nonuniform.hpp"

namespace carpio{

typedef std::list<std::string> Strings;

template<class ANY>
Strings _StringifyHead(const ANY& a, StructureTag){
    Strings res;
    res.push_back(tfm::format("## TYPE_NAME : %s",
        a.type_name()));
    auto d = ANY::Dim;
    res.push_back(tfm::format("## DIMENSION : %d", d));
    return res;      
};

template<class ANY>
Strings _StringifyHead(const ANY& a, SGridUniformTag){
    Strings res;
    res = _StringifyHead(a, StructureTag());
    auto po = a.original();
    res.push_back(tfm::format("## ORIGINAL: %.5e, %.5e, %.5e", 
        po.value(0), po.value(1), po.value(2)));
    res.push_back(tfm::format("## SIZE : %d, %d, %d",
        a.n(0), a.n(1), a.n(2)));
    res.push_back(tfm::format("## GHOST_LAYER : %d",
        a.ghost_layer()));
    res.push_back(tfm::format("## CELL_SIZE : %.5e",
        a.s_(0, 0)));
    return res;      
};
template<class ANY>
Strings _StringifyHead(const ANY& a, SGridNonUniformTag){
    Strings res;
    res = _StringifyHead(a, StructureTag());
    auto po = a.original();
    res.push_back(tfm::format("## ORIGINAL: %.5e, %.5e, %.5e", 
        po.value(0), po.value(1), po.value(2)));
    res.push_back(tfm::format("## SIZE : %d, %d, %d",
        a.n(0), a.n(1), a.n(2)));
    res.push_back(tfm::format("## GHOST_LAYER : %d",
        a.ghost_layer()));
    for(St d = 0; d < a.dim(); ++d){
        std::string s = tfm::format("## CELL_SIZE_%s : ", 
                        ToStringAsAxes(d));
        for(St i = 0; i < a.n(d); ++i){
            s += tfm::format("%.5e", a.s_(d, i));
            if( i < (a.n(d) - 1)){
                s += ", ";
            }
        }
        res.push_back(s);
    }
    return res;      
};

template<class ANY>
Strings _StringifyData(const ANY& grid, SGridTag){
    Strings res;
    auto dim = ANY::Dim;
    Idx nk = (dim>2?grid.n(_Z_):1);
    Idx nj = (dim>1?grid.n(_Y_):1);
    short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
    for (Idx k = 0; k < nk; ++k) {
        for (Idx j = 0; j < nj; ++j) {
            for (Idx i = 0; i < grid.n(_X_); ++i) {
                typename ANY::Index index(i, j, k);
                res.splice(res.end(), 
                           _StringifyCell(grid, index, SGridTag(), typename ANY::DimTag()));
                res.push_back("");
            }
        }
    }
    return res;
};
// template<class ANY, class TAG,
        // typename std::enable_if<
            // std::is_base_of<StructureTag, TAG>::value,
        // bool>::type = true >
template<class ANY>
Strings _Stringify(const ANY& a,  StructureTag t){
    typedef typename ANY::Tag Tag;
    auto res = _StringifyHead(a, Tag());
    res.splice(res.end(), _StringifyData(a, Tag()));
    return res;
}

template<class ANY, class INDEX>
Strings _StringifyCell(const ANY& grid, const INDEX& index, SGridTag, Dim3Tag){
    Strings res;
    short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
    for (short o = 0; o < ANY::NumVertex; ++o){
        auto p = grid.v(order[o], index);
        res.push_back(ToString(p.value(_X_),
                               p.value(_Y_),
                               p.value(_Z_),
                               " "));
    }
    auto p = grid.v(0, index);
    res.push_back(ToString(p.value(_X_),
                           p.value(_Y_),
                           p.value(_Z_),
                           " "));
    return res;
}
template<class ANY, class INDEX>
Strings _StringifyCell(const ANY& grid, const INDEX& index, SGridTag, Dim2Tag){
    Strings res;
    short order[] = { 0, 1, 3, 2 };
    for (short o = 0; o < ANY::NumVertex; ++o){
        auto p = grid.v(order[o], index);
        res.push_back(ToString(p.value(_X_),
                               p.value(_Y_), 0.0,
                               " "));
    }
    auto p = grid.v(0, index);
    res.push_back(ToString(p.value(_X_),
                           p.value(_Y_), 0.0,
                           " "));
    return res;
}
template<class ANY, class INDEX>
Strings _StringifyCell(const ANY& grid, const INDEX& index, SGridTag, Dim1Tag){
    Strings res;
    short order[] = { 0, 1 };
    for (short o = 0; o < ANY::NumVertex; ++o){
        auto p = grid.v(order[o], index);
        res.push_back(ToString(p.value(_X_),
                               0.0,
                               " "));
    }
    return res;
}
template<class ANY, class INDEX>
Strings _StringifyCell(const ANY& a, const INDEX& index, StructureTag tag){
    typedef typename ANY::Tag Tag;
    typedef typename ANY::DimTag DimTag;
    return _StringifyCell(a, index, Tag(), DimTag());    
}

}

#endif