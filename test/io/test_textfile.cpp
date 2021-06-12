#include "gtest/gtest.h"
#include "io/io_define.hpp"
#include <math.h>

using namespace carpio; 

TEST(config, textfile) {
    std::cout << " parse a line " << std::endl; 
    TextFile f;
    std::string line = " \t## test:a ,b ,   de, ## other command ";
    f.parse_config_a_line(line);

}

TEST(config1, textfile) {
//     std::cout << " parse a line " << std::endl; 
    TextFile f("./test/input_files/textfile.txt");
    f.read();
    f.parse_config();
    f.show_config();
}