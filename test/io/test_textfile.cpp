#include "gtest/gtest.h"
#include "io/text_file.hpp"
#include "io/data_file.hpp"
#include <math.h>

using namespace carpio; 

TEST(config, textfile) {
    std::cout << " === parse a line ===  " << std::endl; 
    TextFile f;
    std::string line = "## test:a ,b ,   de, ## other command ";
    f.parse_config_a_line(line);
    f.show_config();
}



TEST(aline_to_double , textfile) {
    std::cout << " === parse to double ===  " << std::endl; 
    TextFile f;
    std::string line = "## test: 39349.0 ## other command ";
    f.parse_config_a_line(line);
    auto v = f.get_config_as_double("test");
    std::cout << "v = " << v << std::endl;
    ASSERT_EQ(v, 39349.0);
}

TEST(aline_to_array , textfile) {
    std::cout << " === parse to array string ===  " << std::endl; 
    TextFile f;
    std::string line = "## test: 39349.0, 45, 67, ## other command ";
    f.parse_config_a_line(line);
    auto arr = f.get_config_as_array_string("test");
    std::cout << "Len array = " << arr.size() << std::endl;
    ASSERT_EQ(arr.size(), 3);
    for(auto& a : arr){
        std::cout << a << std::endl;
    }
}

TEST(aline_to_arrayd , textfile) {
    std::cout << " === parse to array double ===  " << std::endl; 
    TextFile f;
    std::string line = "## test: 39349.0, 45, 67, ## other command ";
    f.parse_config_a_line(line);
    auto arr = f.get_config_as_array_double("test");
    std::cout << "Len array = " << arr.size() << std::endl;
    ASSERT_EQ(arr.size(), 3);
    ASSERT_EQ(arr[2], 67);
}
TEST(write, textfile) {
    MakeDir("./data/");
//     std::cout << " parse a line " << std::endl;
    std::string fn = "out_text.txt";
    TextFile f("./data/out_text.txt");
    f.add_config_item("test", "10");
    f.add_config_item("arr", "10, 34, 567");
    f.add_line("123, 345, 67");
    f.add_line("456, 345, 67");
    f.write();

    EXPECT_TRUE(FileAccessCheck("./data/out_text.txt"));
}
TEST(text_file_read, textfile) {
//     std::cout << " parse a line " << std::endl; 
    TextFile f("./test/input_files/textfile.txt");
    f.read();
    f.parse_config();
    f.show_config();
    EXPECT_NO_THROW(f.get_config("col"));
    std::cout << "config get col" << f.get_config("col")<< std::endl;
}

TEST(token, t1) {
    std::cout << " === parse a line with command ===  " << std::endl; 
    std::vector<std::string> tokens;
    std::string a = " 12, 34, # command";
    Tokenize(a, tokens, "#");
    ASSERT_EQ(tokens.size(), 2);
    for(auto t : tokens){
        std::cout << t << std::endl;
    }
    std::cout<< "------" << std::endl;
    std::string b = " ## aa sfef command";
    std::cout << "first of #  " << b.find_first_of("#") << std::endl;
    std::cout << "first of ## " << b.find_first_of("##") << std::endl;
    std::cout << (std::string::npos == b.find_first_of("z")) << std::endl;
    tokens.clear();
    Tokenize(b, tokens, "#");
    EXPECT_EQ(tokens.size(), 2);\
    int num = 0;
    for(auto t : tokens){
        std::cout << num << " "<< t << std::endl;
        num++;
    }
}


TEST(datafile, read) {
    std::cout << " === read a datafile ===  " << std::endl; 
    DataFile df("./test/input_files/textfile.txt");
    df.read();
    std::cout << "size block = " << df.size_block() << std::endl;
    std::cout << "block 0 =======" << std::endl;
    df.block(0).show();
    std::cout << "block 1 =======" << std::endl;
    df.block(1).show(true);
    std::cout << "block 2 =======" << std::endl;
    df.block(2).show(true);

}