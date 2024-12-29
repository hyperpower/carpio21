#ifndef _DATA_FILE_HPP_
#define _DATA_FILE_HPP_

#include "io_define.hpp"
#include "data_block.hpp"

namespace carpio{

class DataFile {
public:
    typedef DataBlock Block;
    typedef std::shared_ptr<Block> spBlock;
    typedef std::string str;
    typedef std::fstream fst;
    typedef std::list<spBlock> Content;
protected:
    /// a text file is consisted from three parts
    ///  1. filename
    ///  2. multi blocks 
    /// block seperation condition
    ///  1. three consecutive empty lines
    ///  2. change none configure line to configure line

    str      _filename;
    Content  _content;

    static const short EMPLTY_LINE = 0;
    static const short CONFIG_LINE = 1;
    static const short NORMAL_LINE = 2;
public:
    DataFile() :
            _filename("") {
        _init_content();
    }
    DataFile(const str& filename) :
            _filename(filename){
        _init_content();
    }
    DataFile(const str& filename, const Content& content) {
        _filename = filename;
        _content  = content;
    }

    int pre_parse_a_line(std::string& line) const{
        auto nl = Trim(line);
        if(nl.empty()){
            return EMPLTY_LINE; //EMPTYLINE
        }

        if(nl.find_first_of("##") == 0){ // "##" is the first charactor
            return CONFIG_LINE; // config line
        }else{
            if( 0 == nl.find_first_of("#")){ // "#" is the first charactor
                line.clear(); 
                return EMPLTY_LINE;
            }
            std::vector<std::string> tokens;
            Tokenize(nl, tokens, "#");
            line = tokens[0];
            return NORMAL_LINE; // not config line
        }
    }

    void add_new_block(){
        this->_content.emplace_back(std::make_shared<Block>());
    }

    auto block(const Content::size_type& idx){
        auto iter = std::next(this->_content.begin(), idx);
        return *(*iter);
    }

    void read() {
        fst ins;
        this->_open_read(ins);
        ins.seekg(0, std::ios::beg);
        int count = 0, prevf = -1, flag = -1, ec = 0;
        while (!ins.eof()) {
            str sline;
            getline(ins, sline, '\n');
            prevf = flag;
            flag = pre_parse_a_line(sline);
            switch (flag){
                case EMPLTY_LINE:{
                    auto b = this->_content.back();
                    b->add_line(sline); 
                    ec +=1;
                    break;
                }
                case CONFIG_LINE:{
                    if(ec > 3 || prevf == NORMAL_LINE){
                        this->add_new_block();
                        ec = 0;
                    }
                    auto b = this->_content.back();
                    std::cout << sline << std::endl;
                    b->parse_as_config(sline); 
                    break;
                }
                case NORMAL_LINE:{
                    auto b = this->_content.back();
                    b->add_line(sline); 
                    break;
                }
            }
        }
    }

    auto size_block() const{
        return this->_content.size();
    }

    Content& content(){
        return this->_content;
    }
    const Content& content() const{
        return this->_content;
    }

// Write ===========================
// =================================
    void write() {
        fst outs;
        this->_open_write(outs);
        outs.seekg(0, std::ios::beg);
        for (const auto& spb : this->_content) {
            spb->write(outs);
        }
    }

protected:
    void _open_read(fst& ins) {
        ins.open(this->_filename.c_str(), std::ifstream::in);
        if (!ins.is_open()) {
            std::cerr << "!> Open file error! " << this->_filename.c_str() << std::endl;
            throw std::invalid_argument("!> Open file \"" + this->_filename + "\" error");
        }
    }

    void _open_write(fst& outs) {
        outs.open(this->_filename.c_str(), std::fstream::out);
        if (!outs.is_open()) {
            std::cerr << "!> Open file error! " << this->_filename.c_str()
                    << std::endl;
            throw std::invalid_argument("!> Open file \"" + this->_filename + "\" error");
        }
    }

    void _init_content(){
        // initial the first block
        this->_content.emplace_back(std::make_shared<Block>());
    }

    
};

}




#endif