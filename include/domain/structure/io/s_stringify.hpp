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
Strings _Stringify(const ANY& a,  StructureTag){
};

template<class ANY>
Strings _StringifyHead(const ANY& a, StructureTag){
    Strings res;
    res.append("## TYPE_NAME : ", a.type_name());
    res.append("## DIMENSION : ", ANY::Dim);
    return res;      
};

template<class ANY>
Strings _StringifyHead(const ANY& a, SGridTag){
    Strings res;
    res = _StringifyHead(a, StructureTag());
    res.append("## TYPE_NAME : ", a.type_name());
    res.append("## DIMENSION : ", ANY::Dim);
    return res;
      
};

template<class ANY>
Strings _StringifyData(const ANY& a, const StructureTag& tag){
};




}

#endif