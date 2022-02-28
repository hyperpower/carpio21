#ifndef _S_STRINGIGY_HPP
#define _S_STRINGIGY_HPP

#include <iostream>
#include <list>
#include <algorithm>
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
    res.push_back(tfm::format("## DIMENSION : %d",
        ANY::Dim));
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
Strings _StringifyData(const ANY& a, SGridUniformTag){
    return Strings();
};

template<class ANY>
Strings _StringifyData(const ANY& a, SGridNonUniformTag){
    return Strings();
};

template<class ANY>
Strings _Stringify(const ANY& a,  StructureTag tag){
    typedef typename ANY::Tag Tag;
    auto res = _StringifyHead(a, Tag());
    res.splice(res.end(), _StringifyData(a, Tag()));
    return res;
}

}

#endif