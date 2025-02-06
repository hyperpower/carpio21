#ifndef _BASE_IO_HPP
#define _BASE_IO_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include "domain/domain_define.hpp"

namespace carpio{

typedef std::list<std::string> Strings;

template<class ANY>
Strings _Stringify(const ANY& a, const DomainTag& tag){};

template<class ANY>
Strings Stringify(const ANY& a){
    typedef typename ANY::Tag Tag;
    Tag t;
    return _Stringify(a, t); 
}

template<class ANY>
void OutputFile(const std::string& filename, const ANY& a){
    TextFile file(filename, Stringify(a));
    file.write();
}

template<class ANY, class INDEX>
Strings _StringifyCell(const ANY& a, const INDEX& index, const DomainTag& tag){
}

template<class ANY, class INDEX>
Strings StringifyCell(const ANY& a, const INDEX& index){
    typedef typename ANY::Tag Tag;
    Tag t;
    return _StringifyCell(a, index, t);
}



}

#endif