#ifndef _TEST_CLOCK_HPP_
#define _TEST_CLOCK_HPP_

#include "gtest/gtest.h"
#include "utility/clock.hpp"
#include "utility/profile.hpp"

#include <math.h>
#include <string>
#include <memory>
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
// #include "mpi.h"

namespace carpio {

void a_function(){
     ProfileStart("a fun");
     sleep_ms(10);
     ProfileEnd();
}

TEST(profile, clock){
    ProfileStart("main");
    for (int i = 0 ; i< 10 ;i++){
        ProfileStart("inner_fun");
        a_function();
        ProfileEnd();
    }
    ProfileEnd();

    ProfileListShow();
}
TEST(profile, clock2){
    ProfileClean();
    ProfileStart("main");
    for (int i = 0 ; i< 10 ;i++){
        ProfileStart("inner_fun");
        a_function();
        ProfileEnd();
    }
    for (int i = 0 ; i< 10 ;i++){
        ProfileStart("inner_fun2");
        a_function();
        ProfileEnd();
    }
    ProfileEnd();

    ProfileListShow();
}
}

#endif
