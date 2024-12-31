#ifndef _DATA_FILE_HPP_
#define _DATA_FILE_HPP_

#include "utility/tinyformat.hpp"
#include "io_define.hpp"
#include "data_block.hpp"

namespace carpio{

class DataFile {
public:
    typedef DataBlock Block;
    typedef std::shared_ptr<Block> spBlock;
    typedef std::string str;
    typedef std::fstream fst;
    typedef std::list<spBlock> ListBlock;
protected:
    /// a text file is consisted from three parts
    ///  1. filename
    ///  2. multi blocks 
    /// block seperation condition
    ///  1. three consecutive empty lines
    ///  2. change none configure line to configure line

    str        _filename;
    ListBlock  _listb;

    static const short EMPLTY_LINE = 0;
    static const short CONFIG_LINE = 1;
    static const short NORMAL_LINE = 2;
public:
    DataFile() :
            _filename("") {
        _init_list_blocks();
    }
    DataFile(const str& filename) :
            _filename(filename){
        _init_list_blocks();
    }
    DataFile(const str& filename, const ListBlock& list_b) {
        _filename = filename;
        _listb  = list_b;
    }

    int pre_parse_a_line(std::string& line) const{
        auto nl = Trim(line);
        if(nl.empty()){
            return EMPLTY_LINE; //EMPTYLINE
        }

        if(StartsWith(nl, "##")){ // "##" is the first charactor
            std::cout<< "first of ## " << nl << std::endl;
            return CONFIG_LINE; // config line
        }else{
            if(StartsWith(nl, "#")){ // "#" is the first charactor
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
        this->_listb.emplace_back(std::make_shared<Block>());
    }

    auto block(const ListBlock::size_type& idx){
        if(idx >= this->_listb.size()){
            auto emsg = tfm::format("index %d out of %d", idx, this->_listb.size());
            throw std::out_of_range(emsg);
        }
        auto iter = std::next(this->_listb.begin(), idx);
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
                    auto b = this->_listb.back();
                    b->add_line(sline); 
                    ec +=1;
                    break;
                }
                case CONFIG_LINE:{
                    if(ec >= 3 || prevf == EMPLTY_LINE || prevf == NORMAL_LINE){
                        this->add_new_block();
                        ec = 0;
                    }
                    auto b = this->_listb.back();
                    b->parse_as_config(sline); 
                    break;
                }
                case NORMAL_LINE:{
                    if(ec >= 3){
                        this->add_new_block();
                        ec = 0;
                    }
                    if(prevf == EMPLTY_LINE){
                        ec = 0;
                    }
                    auto b = this->_listb.back();
                    b->add_line(sline); 
                    break;
                }
            }
        }
    }

    auto size_block() const{
        return this->_listb.size();
    }

    ListBlock& list_b(){
        return this->_listb;
    }
    const ListBlock& list_b() const{
        return this->_listb;
    }

// Write ===========================
// =================================
    void write() {
        fst outs;
        this->_open_write(outs);
        outs.seekg(0, std::ios::beg);
        for (const auto& spb : this->_listb) {
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

    void _init_list_blocks(){
        // initial the first block
        this->_listb.emplace_back(std::make_shared<Block>());
    }

    
};

}




#endif