#ifndef _DATA_BLOCK_HPP_
#define _DATA_BLOCK_HPP_

#include "io_define.hpp"

namespace carpio{

class DataBlock {
public:
    typedef std::string str;
    typedef std::list<str> lines;
    typedef std::fstream fst;
    typedef std::map<std::string, std::string> dict;
protected:
    /// a data block is consisted from three parts
    ///  2. a dictionary: illustrate how to read this text file
    ///  3. contents

    dict  _config;
    lines _content;
public:
    DataBlock() :
            _config(), _content() {
    }
    DataBlock(const lines& content) : _config(){
        _content  = content;
    }

    void add_line(const str& line) {
        this->_content.push_back(line);
    }

    void remove_last_line(const unsigned int& n = 1){
        for(unsigned int i = 0; i < n && this->_content.size()> 0; i++){
            this->_content.pop_back();
        }
    }
    
    void add_config_item(const str& key, const str& value){
        this->_config[key] = value;
    }

    lines& content(){
        return this->_content;
    }
    const lines& content() const{
        return this->_content;
    }

    void parse_config() {
        for (auto it = _content.begin(); it != _content.end(); ) {
            if (parse_config_a_line(*it)) {
                it = _content.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    void parse_as_config(const std::string& line){
        // input line was trimed and start with "##"
        std::vector<std::string> tokens;
        Tokenize(line, tokens, "##");
        // the First token is useful
        if(tokens.size() >= 1){
            auto t = Trim(tokens[0]);
            std::vector<std::string> kvtokens;
            Tokenize(t, kvtokens, ":");
            this->_config[Trim(kvtokens[0])] = Trim(kvtokens[1]);
        }
    }

    bool parse_config_a_line(const std::string& line){
        auto nl = Trim(line);
        if(nl.find_first_of("##") != 0){
            return false; // not config line
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
        return true;
    }

    void show_config() const{
        for (auto& str : _config) {
            std::cout << str.first << " : " << str.second << "\n";
        }
    }
    
    void show_content(const bool show_all = false) const{
        if (show_all || this->_content.size() < 6){
            for (auto& str : _content) {
                std::cout << str << "\n";
            }
        }else{
            for(auto iter = _content.begin(); 
                     iter != std::next(_content.begin(), 3); 
                     ++iter){
                std::cout << *iter << std::endl;
            }
            std::cout << ". . ." << std::endl;
            for(auto iter = std::prev(_content.end(), 3); 
                     iter != _content.end(); 
                     ++iter){
                std::cout << *iter << std::endl;
            }
        }
    }

    void show(const bool show_all = false) const{
        this->show_config();
        this->show_content(show_all);
    }

    bool has_config(const std::string& key) const{
        typename dict::const_iterator iter = this->_config.find(key);
        if (iter == this->_config.end()) {
            return false;
        } else {
            return true;
        }
    }

    std::string get_config(const std::string& key) const {
        typename dict::const_iterator iter = this->_config.find(key);
        if (iter == this->_config.end()) {
            throw std::invalid_argument( "\"" + key + "\" not found");
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
    
    int get_config_as_int(const std::string& key) const{
        std::string value(this->get_config(key));
        auto tvalue = Trim(value);
        double v = std::stoi(tvalue);
        return v;
    }

    std::vector<std::string> parse_as_vector_string(const std::string& line) const{
        auto tvalue = Trim(line);
        std::vector<std::string> tokens;
        Tokenize(tvalue, tokens, ",");
        for(auto& v : tokens){
            v = Trim(v);
        }
        return tokens;
    }
    
    std::vector<double> parse_as_vector_double(const std::string& line) const{
        auto tvalue = Trim(line);
        std::vector<std::string> tokens;
        Tokenize(tvalue, tokens, ",");
        std::vector<double> vecd(tokens.size());
        auto iterv = vecd.begin();
        for(auto& v : tokens){
            (*iterv) = std::stod(Trim(v));
            iterv++;
        }
        return vecd;
    }

    std::vector<std::string> get_config_as_vector_string(const std::string& key) const{
        std::string value(this->get_config(key));
        return this->parse_as_vector_string(value); 
    }

    std::vector<double> get_config_as_array_double(const std::string& key) const{
        std::vector<std::string> arrs = get_config_as_vector_string(key);
        std::vector<double> arrd(arrs.size());
        for(auto i = 0; i < arrs.size(); ++i){
            arrd[i] = std::stod(arrs[i]);
        }
        return arrd;
    }

    std::vector<double> get_content_row_as_vector_double(const lines::size_type& index) const{
        if(index >= this->_content.size()){
            throw std::out_of_range("");
        }
        auto iter_row = std::next(this->_content.begin(), index);
        return this->parse_as_vector_double(*iter_row);
    }
    // Write ===========================
    // =================================
    void write(fst& outs) {
        outs.seekg(0, std::ios::end);
        for (const auto& n : this->_config) {
            outs << "## " << n.first << " : " << n.second << "\n";
        }
        for (auto& line : this->_content) {
            outs << line << "\n";
        }
    }
};
}

#endif