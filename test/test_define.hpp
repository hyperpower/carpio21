#pragma once
#ifndef _TEST_DEFINE_HPP_
#define _TEST_DEFINE_HPP_

#include <string>

#include "gtest/gtest.h"
const std::string FIG_PATH = "./fig/";
const std::string DATA_PATH = "./data/";

const int fig_width  = 590 * 1.5;
const int fig_height = 400 * 1.5;

#include "io/io_define.hpp"

TEST(test_define, dir_location){
    std::string cwd = carpio::GetWorkingPath();
    // std::cout << "Dir_loaction :"<< cwd << std::endl;
    bool has_include = carpio::FileAccessCheck(cwd + "/include");
    // std::cout << "Has Include  " << carpio::ToString(has_include) << std::endl;
    bool has_test = carpio::FileAccessCheck(cwd + "/test");
    // std::cout << "Has Test  " << carpio::ToString(has_test) << std::endl;
    bool has_fig = carpio::FileAccessCheck(cwd + "/fig");
    // std::cout << "Has Fig  " << carpio::ToString(has_fig) << std::endl;
    bool has_examples = carpio::FileAccessCheck(cwd + "/examples");
    // std::cout << "Has Example  " << carpio::ToString(has_examples) << std::endl;
    bool has_readme = carpio::FileAccessCheck(cwd + "/README.md");
    // std::cout << "Has Example  " << carpio::ToString(has_readme) << std::endl;
    int a = 0;
    ASSERT_EQ(true, has_include) << "include dir is not found"; 
    ASSERT_EQ(true, has_test) << "test dir is not found"; 
    ASSERT_EQ(true, has_examples) << "examples dir is not found"; 
    ASSERT_EQ(true, has_readme) << "readme dir is not found"; 

    std::cout << "[    CWD   ] " << cwd << std::endl;
}

#endif 