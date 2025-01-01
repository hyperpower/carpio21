#ifndef _DISTANCE_ANNOTATION_HPP_
#define _DISTANCE_ANNOTATION_HPP_

#include "geometry/objects/basic/point.hpp"
#include "geometry/operation/affine.hpp"
#include <array>
#include <sstream>
#include <iostream>
#include <cmath>

namespace carpio {

class DistanceIndicator{
public:
    static const std::size_t Dim = 2;
    typedef DistanceIndicator Self;
    typedef Point_<double, Dim> Point;
    typedef Segment_<double, Dim> Segment;
protected:
    Point_<double, Dim> _p1; 
    Point_<double, Dim> _p2; 
    double _height;
    double _normal_line_length;
    double _space;
    double _offset;
    
    Point _n_vector;
    Point _t_vector;
public:
    DistanceIndicator():
        _height(1.0), _space(0.1), _offset(0), 
        _normal_line_length(2.0){
    }
    DistanceIndicator(const double& x1, const double& y1,
                       const double& x2, const double& y2):
            _p1(x1, y1), _p2(x2, y2), _space(0.2), _height(0.4), _offset(0), 
            _normal_line_length(0.5){
        // this->_cal_height();
        this->_cal_normal_vector();
        this->_cal_tangent_vector();
    }

    void set_height(const double& h){
        this->_height = h;
    }
    void set_space(const double& h){
        this->_space = std::abs(h);
    }
    void set_normal_line_length(const double& h){
        this->_normal_line_length = std::abs(h);
    }
    void set_offset(const double& o){
        this->_offset = o;
    }

    Point normal_line_left(const std::size_t& num){
        if(num == 0){
            return this->_n_vector * double(Sign(this->_height)) 
                * this->_space + this->_p1;
        }else{
            return this->_n_vector * double(Sign(this->_height)) 
                * (this->_normal_line_length + this->_space) + this->_p1;
        }
    }
    Point normal_line_right(const std::size_t& num){
        if(num == 0){
            return this->_n_vector * double(Sign(this->_height)) 
                * this->_space + this->_p2;
        }else{
            return this->_n_vector * double(Sign(this->_height)) 
                * (this->_normal_line_length + this->_space) + this->_p2;
        }
    }
    Point normal_vector()const{
        return this->_n_vector;
    }
    Point tangent_vector()const{
        return this->_t_vector;
    }
    Point arrow_center(){
        auto midp  = Mid(this->_p1, this->_p2);
        return this->_n_vector * double(Sign(this->_height))
            * (std::abs(this->_height) +this->_space) + midp;
    }
    Point arrow_left() const{
        return  this->_n_vector * double(Sign(this->_height)) 
            * (std::abs(this->_height) + this->_space) + this->_p1;
    }
    Point arrow_right(){
        return this->_n_vector * double(Sign(this->_height)) 
            * (std::abs(this->_height) + this->_space) + this->_p2;
    }
    Point text_center(const double& offset = 0.1){
        auto midp  = Mid(this->_p1, this->_p2);
        return this->_n_vector * double(Sign(this->_height)) 
            * (std::abs(this->_height) + this->_space + offset) + midp;
    }
    bool is_offset() const{
        return this->_offset != 0.0;
    }
    Point offset(){
        if(this->_offset < 0){
            return this->_t_vector * this->_offset + this->arrow_left();
        }else{
            return this->_t_vector * this->_offset + this->arrow_right();
        }
    }
protected:
    void _cal_normal_vector(){
        this->_n_vector = this->_p2;
        Rotate(this->_n_vector, _PI_ * 0.5, this->_p1);
        this->_n_vector = this->_n_vector - this->_p1;
        this->_n_vector.normalize();
    }
    void _cal_tangent_vector(){
        this->_t_vector = this->_p2 - this->_p1;
        this->_t_vector.normalize(); 
    }
    void _cal_height(){
        double dis = Distance(_p1, _p2);
        _height = std::min(dis, 1.0);
    }
};



}

#endif