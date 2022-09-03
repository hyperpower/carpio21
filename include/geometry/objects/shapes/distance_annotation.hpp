#ifndef _DISTANCE_ANNOTATION_HPP_
#define _DISTANCE_ANNOTATION_HPP_

#include "geometry/objects/basic/point.hpp"
#include "geometry/operation/affine.hpp"
#include <array>
#include <sstream>
#include <iostream>
#include <cmath>

namespace carpio {

class DistanceAnnotation{
public:
    static const std::size_t Dim = 2;
    typedef DistanceAnnotation Self;
    typedef Point_<double, Dim> Point;
protected:
    Point_<double, Dim> _p1; 
    Point_<double, Dim> _p2; 
    double _height;
    double _normal_line_length;
    double _space;
    double _offset;
    Trinary _text_loc;
    
    std::string _text;
    Point _normal_vector;
public:
    DistanceAnnotation():
        _height(1.0), _space(0.1), _offset(0), 
        _text_loc(_ZERO_), _text(0), _normal_line_length(2.0){
    }
    DistanceAnnotation(const double& x1, const double& y1,
                       const double& x2, const double& y2):
            _p1(x1, y1), _p2(x2, y2), _space(0.1),
            _normal_line_length(2.0){
        this->_cal_height();
    }

    auto normal_line_left(const std::size_t& num){
        
    }
    Point arrow_center(){

    }
    Point arrow_left(){

    }
    Point arrow_right(){

    }
    void normal_vector(){
        this->_normal_vector = this->_p2;
        Rotate(this->_normal_vector, _PI_ * 0.5, _p1);
    }
protected:
    void _cal_height(){
        double dis = Distance(_p1, _p2);
        _height = std::min(dis, 1.0);
    }
};



}

#endif