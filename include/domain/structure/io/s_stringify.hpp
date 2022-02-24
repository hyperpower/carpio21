#ifndef _S_STRINGIGY_HPP
#define _S_STRINGIGY_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include "domain/domain_define.hpp"

#include "domain/structure/grid/sgrid.hpp"

namespace carpio{

typedef std::list<std::string> Strings;

template<class ANY>
Strings _Stringify(const ANY& a,  StructureTag tag){
    typedef typename ANY::Tag Tag;
    auto res = _StringifyHead(a, Tag());
    res.splice(res.end(), _StringifyData(a, Tag()));
    return res;
};

template<class ANY>
Strings _StringifyHead(const ANY& a, StructureTag){
    Strings res;
    res.push_back("## TYPE_NAME : " + a.type_name());
    res.push_back("## DIMENSION : " + ANY::Dim);
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
    return res;      
};

template<class ANY>
Strings _StringifyData(const ANY& a, SGridUniformTag){
    return Strings();
};




}

#endif