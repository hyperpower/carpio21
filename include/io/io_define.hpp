#ifndef _IO_DEFINE_HPP_
#define _IO_DEFINE_HPP_

#include <type_define.hpp>
#include <iostream>
#include <fstream>
#ifdef __linux__ 
    //linux code goes here
    #include <unistd.h>
    #include <sys/stat.h> 
#else // _WIN32
    // windows code goes here
    #include <io.h>
    #include <process.h>
    #include <windows.h>
    #include <direct.h>
#endif

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <map>

namespace carpio {
template<class V1>
std::string ToString(V1 a) {
    std::ostringstream sst;
    sst << a;
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

inline std::string ToString(const Axes& a) {
    if (a == _X_) {
        return "_X_";
    }
    if (a == _Y_) {
        return "_Y_";
    }
    return "_Z_";
}
inline std::string ToString(const Orientation& a) {
    if (a == _M_) {
        return "_M_";
    }
    if (a == _P_) {
        return "_P_";
    }
    return "_C_";
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
        int mode //
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

inline bool CreateDir(std::string dir) {
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
#elif defined __GNUC__
    mkdir(dir.data(), 0777);
#endif
}

class TextFile {
public:
    typedef std::string str;
    typedef std::list<str> lines;
    typedef std::fstream fst;
    typedef std::map<std::string, std::string> dict;
protected:
    /// a text file is consisted from three parts
    ///  1. filename
    ///  2. a dictionary: illustrate how to read this text file
    ///  3. contents

    str   _filename;
    dict  _config;
    lines _content;
public:
    TextFile() :
            _filename(""), _content() {
    }
    TextFile(const str& filename) :
            _filename(filename), _content() {
    }
    TextFile(const str& filename, const lines& content) {
        _filename = filename;
        _content  = content;
    }

    void add_line(const str& line) {
        return _content.push_back(line);
    }

    void _open_read(fst& ins) {
        ins.open(this->_filename.c_str(), std::ifstream::in);
        if (!ins.is_open()) {
            std::cerr << "!> Open file error! " << this->_filename.c_str()
                    << " \n";
            exit(-1);
        }
    }

    void _open_write(fst& outs) {
        outs.open(this->_filename.c_str(), std::fstream::out);
        if (!outs.is_open()) {
            std::cerr << "!> Open file error! " << this->_filename.c_str()
                    << " \n";
            exit(-1);
        }
    }

    void read() {
        fst ins;
        this->_open_read(ins);
        ins.seekg(0, std::ios::beg);
        while (!ins.eof()) {
            str sline;
            getline(ins, sline, '\n');
            this->_content.push_back(sline);
        }
    }

    lines& content(){
        return this->_content;
    }

    void parse_config() {
        for (auto& line : _content) {
            parse_config_a_line(line);
        }
    }

    void parse_config_a_line(const std::string& line){
        // std::cout << line << std::endl;
        auto nl = Trim(line);
        // std::cout << nl<< std::endl;
        if(nl.find_first_of("##") != 0){
            return; // not config line
        }
        std::vector<std::string> tokens;
        Tokenize(nl, tokens, "##");
        // the First token is useful
        if(tokens.size() >= 1){
            auto t = Trim(tokens[0]);
            std::vector<std::string> kvtokens;
            Tokenize(t, kvtokens, ":");
            this->_config[Trim(kvtokens[0])] = Trim(kvtokens[1]);
        }
    }

    void show_config() {
        for (auto& str : _config) {
            std::cout << str.first << " : " << str.second << "\n";
        }
    }

    std::string get_config(const std::string& key) const {
        typename dict::const_iterator iter = this->_config.find(key);
        if (iter == this->_config.end()) {
            ASSERT_MSG(false, "Not found key");
            return "";
        } else {
            return iter->second;
        }
    }

    void write() {
        fst outs;
        this->_open_write(outs);
        outs.seekg(0, std::ios::beg);
        for (lines::iterator iter = _content.begin(); iter != _content.end();
                ++iter) {
            outs << (*iter) << "\n";
        }
    }

    template<class ContainerT>
    static void Tokenize(const std::string& str, ContainerT& tokens,
            const std::string& delimiters = " ", bool trimEmpty = true)
            {
        std::string::size_type pos, lastPos = 0, length = str.length();

        using value_type = typename ContainerT::value_type;
        using size_type  = typename ContainerT::size_type;

        while (lastPos < length + 1)
        {
            pos = str.find_first_of(delimiters, lastPos);
            if (pos == std::string::npos) {
                pos = length;
            }

            if (pos != lastPos || !trimEmpty)
                tokens.push_back(value_type(str.data() + lastPos,
                        (size_type) pos - lastPos));

            lastPos = pos + 1;
        }
    }

};

}

#endif /* IO_H_ */
