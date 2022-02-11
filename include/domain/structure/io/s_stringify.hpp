#ifndef _S_STRINGIGY_HPP
#define _S_STRINGIGY_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include "domain/domain_define.hpp"

namespace carpio{

typedef std::list<std::string> Strings;

template<class ANY>
Strings _Stringify(const ANY& a, const StructureTag& tag){
};

template<class ANY>
Strings _StringifyHead(const ANY& a, const StructureTag& tag){
};

template<class ANY>
Strings _StringifyData(const ANY& a, const StructureTag& tag){
};




}

#endif