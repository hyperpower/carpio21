#ifndef _TEXT_FILE_HPP_
#define _TEXT_FILE_HPP_

#include "io_define.hpp"

namespace carpio{

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
        this->_content.push_back(line);
    }
    
    void add_config_item(const str& key, const str& value){
        this->_config[key] = value;
    }

    void _open_read(fst& ins) {
        ins.open(this->_filename.c_str(), std::ifstream::in);
        if (!ins.is_open()) {
            std::cerr << "!> Open file error! " << this->_filename.c_str()
                    << std::endl;
            exit(-1);
        }
    }

    void _open_write(fst& outs) {
        outs.open(this->_filename.c_str(), std::fstream::out);
        if (!outs.is_open()) {
            std::cerr << "!> Open file error! " << this->_filename.c_str()
                    << std::endl;
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

    double get_config_as_double(const std::string& key) const{
        std::string value(this->get_config(key));
        auto tvalue = Trim(value);
        double v = std::stod(tvalue);
        return v;
    }
    std::vector<std::string> get_config_as_array_string(const std::string& key) const{
        std::string value(this->get_config(key));
        auto tvalue = Trim(value);
        std::vector<std::string> tokens;
        Tokenize(tvalue, tokens, ",");
        for(auto& v : tokens){
            v = Trim(v);
        }
        return tokens;
    }

    std::vector<double> get_config_as_array_double(const std::string& key) const{
        std::vector<std::string> arrs = get_config_as_array_string(key);
        std::vector<double> arrd(arrs.size());
        for(auto i = 0; i < arrs.size(); ++i){
            arrd[i] = std::stod(arrs[i]);
        }
        return arrd;
    }

// Write ===========================
// =================================
    void write() {
        fst outs;
        this->_open_write(outs);
        outs.seekg(0, std::ios::beg);
        for (const auto& n : this->_config) {
            outs << "## " << n.first << " : " << n.second << "\n";
        }
        for (auto& line : this->_content) {
            outs << line << "\n";
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




#endif