#ifndef _IO_DEFINE_HPP_
#define _IO_DEFINE_HPP_

#include <type_define.hpp>
#include <iostream>
#include <fstream>
#ifdef __linux__ 
    //linux code goes here
    #include <unistd.h>
    #include <sys/stat.h> 
#elif _WIN32 || _WIN64
    // windows code goes here
    #include <io.h>
    #include <process.h>
    #include <windows.h>
    #include <direct.h>
#elif __APPLE__
    #include <unistd.h>
    #include <sys/stat.h> 
#endif

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
#include <algorithm>

namespace carpio {
template<class V1>
std::string ToString(V1 a) {
    std::ostringstream sst;
    sst << a;
    return sst.str();
}

template<>
std::string ToString(bool a) {
    std::ostringstream sst;
    if (a == true){
        sst << "True";
    }else{
        sst << "False";
    }
    return sst.str();
}

template<class CONTAINER, 
         typename std::enable_if<IsContainer<CONTAINER>::value, bool>::type = true>
std::string ToString(const CONTAINER& a, const std::string& sep) {
    std::ostringstream sst;
    for(auto iter = a.cbegin(); iter != a.cend(); ++iter){
        sst << ToString(*iter);
        if ( std::next(iter, 1) != a.cend()){
            sst << sep;
        }
    }
    return sst.str();
}
/*
 *  out put a and be to string,
 *  class V1 and v2 must overload operator<<
 *  sep is the separator
 *  For example:
 *  a = 1.3
 *  b = 1.4
 *  sep = " "
 *  return  1.3 1.4
 *
 */
template<class V1, class V2>
std::string ToString(V1 a, V2 b, const std::string& sep) {
    std::ostringstream sst;
    sst << a << sep << b;
    return sst.str();
}

template<class V1, class V2, class V3>
std::string ToString(V1 a, V2 b, V3 c, const std::string& sep) {
    std::ostringstream sst;
    sst << a << sep << b << sep << c;
    return sst.str();
}

template<class V1, class V2, class V3, class V4>
std::string ToString(V1 a, V2 b, V3 c, V4 d,
        const std::string& sep) {
    std::ostringstream sst;
    sst << a << sep; //1
    sst << b << sep; //2
    sst << c << sep; //3
    sst << d ; //4
    return sst.str();
}

template<class V1, class V2, class V3, class V4, class V5>
std::string ToString(V1 a, V2 b, V3 c, V4 d, V5 e,
        const std::string& sep) {
    std::ostringstream sst;
    sst << a << sep; //1
    sst << b << sep; //2
    sst << c << sep; //3
    sst << d << sep; //4
    sst << e ;
    return sst.str();
}

template<class V1, class V2, class V3, class V4, class V5, class V6, class V7>
std::string ToString(V1 a, V2 b, V3 c, V4 d, V5 e, V6 f, V7 g,
        const std::string& sep) {
    std::ostringstream sst;
    sst << a << sep; //1
    sst << b << sep; //2
    sst << c << sep; //3
    sst << d << sep; //4
    sst << e << sep; //5
    sst << f << sep; //6
    sst << g;    //7
    return sst.str();
}

const std::string WHITESPACE = " \n\r\t\f\v";
 
inline std::string LTrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
inline std::string RTrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
inline std::string Trim(const std::string &s) {
    return RTrim(LTrim(s));
}

inline bool StartsWith(const std::string& str, const std::string& prefix) {
    return str.find(prefix) == 0;
}
inline std::string ToStringAsAxes(const St& d){
    ASSERT(d < 3);
    if (d == 0) {
        return "X";
    }
    if (d == 1) {
        return "Y";
    }
    return "Z";
}

inline std::string ToString(const Axes& a) {
    if (a == _X_) {
        return "X";
    }
    if (a == _Y_) {
        return "Y";
    }
    return "Z";
}
inline std::string ToString(const Orientation& o) {
    switch(o){
		case _M_: {return "M"; break;}
		case _P_: {return "P"; break;}
		case _C_: {return "C"; break;}
	}
}

//
//  Lowercases string
//
template <typename T>
std::basic_string<T> ToLowerCase(const std::basic_string<T>& s)
{
    std::basic_string<T> s2 = s;
    std::transform(s2.begin(), s2.end(), s2.begin(), tolower);
    return s2;
}

//
// Uppercases string
//
template <typename T>
std::basic_string<T> ToUpperCase(const std::basic_string<T>& s)
{
    std::basic_string<T> s2 = s;
    std::transform(s2.begin(), s2.end(), s2.begin(), toupper);
    return s2;
}



inline std::string GetWorkingPath()
{
    char temp [ 256 ];

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    if ( _getcwd(temp, 256) != 0)
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    if ( getcwd(temp, 256) != 0)
#endif
    return std::string ( temp );

    int error = errno;

    switch ( error ) {
    // EINVAL can't happen - size argument > 0

    // PATH_MAX includes the terminating nul, 
    // so ERANGE should not be returned

    case EACCES:
        throw std::runtime_error("Access denied");

    case ENOMEM:
        // I'm not sure whether this can happen or not 
        throw std::runtime_error("Insufficient storage");

    default: {
        std::ostringstream str;
        str << "Unrecognised error" << error;
        throw std::runtime_error(str.str());
    }
    }
}

inline bool FileAccessCheck( //
        const std::string &filename, //
        int mode = 0 //
        ) {
    if (mode < 0 || mode > 7) {
        std::cerr << " >! Input mode is wrong  =" << mode
                << ", it should be from 0 to 7" << std::endl;
        return false;
    }

    //  int _access(const char *path, int mode);
    //  returns 0 if the file has the given mode,
    //  it returns -1 if the named file does not exist or is not accessible in
    //  the given mode
    // mode = 0 (F_OK) (default): checks file for existence only
    // mode = 1 (X_OK): execution permission
    // mode = 2 (W_OK): write permission
    // mode = 4 (R_OK): read permission
    // mode = 6       : read and write permission
    // mode = 7       : read, write and execution permission
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    if (_access(filename.c_str(), mode) == 0)
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    if (access(filename.c_str(), mode) == 0)
#endif
    {
    return true;
    } else {
        return false;
    }
}

inline bool MakeDir(std::string dir) {
#if defined _MSC_VER   // windows
    int rc = _mkdir( dir.data() );
    if( rc == 0 ){
        return true;
    }else if(rc == EEXIST){
        // file exist
        return true;
    }else{
        return false;
    }
#else // defined __GNUC__
    return (0 == mkdir(dir.data(), 0777));
#endif
}


template <class ContainerT>
void Tokenize(const std::string &str, 
                    ContainerT  &tokens,
              const std::string &delimiters = " ", 
                    bool         trimEmpty = true){
    std::string::size_type pos, lastPos = 0, length = str.length();

    using value_type = typename ContainerT::value_type;
    using size_type  = typename ContainerT::size_type;

    while (lastPos < length + 1){
        pos = str.find_first_of(delimiters, lastPos);
        if (pos == std::string::npos){
            pos = length;
        }

        if (pos != lastPos || !trimEmpty)
            tokens.push_back(value_type(str.data() + lastPos,
                                        (size_type)pos - lastPos));

        lastPos = pos + 1;
    }
}


}

#endif /* IO_H_ */
