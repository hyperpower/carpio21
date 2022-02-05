#ifndef _O_NODE_H_
#define _O_NODE_H_

//#include "../typedefine.hpp"
#include "domain/octree/octree_define.hpp"
#include "ocell.hpp"
#include "odata.hpp"
#include <functional>

#include <math.h>

namespace carpio {

enum NodeIdx {
    //=========================
    //   y
    //   |
    //   ---------------
    //   |  PM  |  PP  |
    //   |  2   |  3   |
    //   |  WN  |  NE  |
    //   ---------------
    //   |  SW  |  SE  |
    //   |  0   |  1   |
    //   |  MM  |  MP  |
    //   ------------------->x
    //
    //   ---------------  ---------------
    //   |  MPM |  MPP |  |  PPM |  PPP |
    //   |  2   |  3   |  |  6   |  7   |
    //   |  WNB |  NEB |  |  WNF |  NEF |
    //   ---------------  ---------------
    //   |  SWB |  SEB |  |  SWP |  SEP |
    //   |  0   |  1   |  |  4   |  5   |
    //   |  MMM |  MMP |  |  PMM |  PMP |
    //   ---------------  ---------------
    //=========================

    //2D
    _MM_ = 0, //00
    _MP_ = 1, //01
    _PM_ = 2, //10
    _PP_ = 3, //11
    //3D
    _MMM_ = 0,
    _MMP_ = 1,
    _MPM_ = 2,
    _MPP_ = 3,
    _PMM_ = 4,
    _PMP_ = 5,
    _PPM_ = 6,
    _PPP_ = 7,
};

inline bool is_x_p(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 6) == 7;
}

inline bool is_x_m(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 6) == 6;
}

inline bool is_y_p(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 5) == 7;
}

inline bool is_y_m(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 5) == 5;
}

inline bool is_z_p(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 3) == 7;
}

inline bool is_z_m(St i) {
    ASSERT(i >= 0 && i < 8);
    return (i | 3) == 3;
}

inline bool is_on_direction(St i, const Direction& dir) {
    ASSERT(i >= 0 && i < 8);
    unsigned short hi = HI(dir);
    unsigned short lo = LO(dir);
    return (hi & i) == (hi & lo);
}


