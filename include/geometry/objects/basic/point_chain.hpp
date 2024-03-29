/*
 * _point_chain.hpp
 *
 *  Created on: Jul 6, 2017
 *      Author: zhou
 */

#ifndef _POINT_CHAIN_HPP_
#define _POINT_CHAIN_HPP_

#include "geometry/geometry_define.hpp"
#include "point.hpp"
#include "box.hpp"
#include "algebra/array/array_list.hpp"
#include "segment.hpp"
#include <array>
#include <vector>
#include <limits>
#include <list>
#include <fstream>
#include <algorithm>
#include <functional>

namespace carpio {

template<typename TYPE, St DIM>
class IntersectionPairSS_;

struct PointChainTag: public GeometryTag {};

template<class TYPE, St DIM>
class PointChain_ {
public:
    static const St Dim = DIM;
    typedef PointChainTag Tag;
    typedef Point_<TYPE, DIM> Point;
    typedef Point_<TYPE, DIM>& ref_Point;
    typedef const Point_<TYPE, DIM>& const_ref_Point;
    typedef Segment_<TYPE, DIM> Segment;
    typedef Segment& ref_Segment;
    typedef Box_<TYPE, DIM> Box;
    typedef PointChain_<TYPE, DIM>     PointChain;
    typedef PointChain                 Self;
    typedef PointChain_<TYPE, DIM>& refPointChain;
    typedef TYPE vt;
    // std iter like
    typedef typename std::list<Point> ListPoint;
    typedef typename std::list<Point>::iterator iterator;
    typedef typename std::list<Point>::const_iterator const_iterator;
    typedef typename std::list<Point>::reference reference;
    typedef typename std::list<Point>::const_reference const_reference;
    typedef Point value_type;  // < except for this one 

    typedef TYPE CV;

protected:
    /** Linked point chain */
    std::list<Point> _lpoints;
    bool _closed; // is the chain closed, that is, is the first point is linked with the last one?
public:
    PointChain_() :
            _lpoints(), _closed(false) {
    }
    PointChain_(const Point& p1) :
            _closed(false) {
        _lpoints.push_back(p1);
    }
    PointChain_(const Point& p1, const Point& p2) :
            _closed(false) {
        _lpoints.push_back(p1);
        _lpoints.push_back(p2);
    }
    PointChain_(const Point& p1, const Point& p2, const Point& p3, bool close =
            true) :
            _closed(close) {
        _lpoints.push_back(p1);
        _lpoints.push_back(p2);
        _lpoints.push_back(p3);
    }
    PointChain_(const Point& p1, const Point& p2, const Point& p3,
            const Point& p4, bool close = true) :
            _closed(close) {
        _lpoints.push_back(p1);
        _lpoints.push_back(p2);
        _lpoints.push_back(p3);
        _lpoints.push_back(p4);
    }

    template<class Container_Point>
    PointChain_(const Container_Point& ver,
            bool closed = true) : _closed(closed){
        if (ver.size() > 0){
            std::copy(ver.begin(), ver.end(), std::back_inserter(_lpoints));
        }
    }

    PointChain_(const Self& o):_lpoints(o._lpoints), _closed(o._closed){
        // std::cout << "self " << std::endl;
    }
    PointChain_(const Self&& o):_lpoints(std::move(o._lpoints)), _closed(std::move(o._closed)){
        // std::cout << "move" << std::endl;
    }
    Self& operator=(const Self&o){
        if (this == &o) {
            return *this;
        }
        this->_lpoints= o._lpoints;
        this->_closed = o._closed;
        return *this;
    }
    Self& operator=(Self&&o){
        this->_lpoints = std::move(o._lpoints);
        this->_closed  = std::move(o._closed);
        return *this;
    }

    void init(const Segment& s) {
        _lpoints.push_back(s.ps());
        _lpoints.push_back(s.pe());
    }

    const Point& get(const St& idx) const {
        assert(idx < this->size());
        auto iter = _lpoints.begin();
        for(St i = 0; iter != _lpoints.end(); ++i){
            if( i == idx ){
                return (*iter);
            }
            std::advance(iter, 1);
        }
        SHOULD_NOT_REACH;
        return (*iter);
    }
    Point& get(const St& idx) {
        assert(idx < this->size());
        auto iter = _lpoints.begin();
        for(St i = 0; iter != _lpoints.end(); ++i){
            if( i == idx ){
                return (*iter);
            }
            std::advance(iter, 1);
        }
        SHOULD_NOT_REACH;
        return (*iter);
    }
    void set_close() {
        _closed = true;
    }

    void set_open() {
        _closed = false;
    }

    bool closed() const {
        return _closed;
    }

    bool empty() const {
        return _lpoints.empty();
    }

    iterator begin() {
        return _lpoints.begin();
    }

    iterator end() {
        return _lpoints.end();
    }

    const_iterator begin() const {
        return _lpoints.begin();
    }
    const_iterator end() const {
        return _lpoints.end();
    }

    reference front() {
        return _lpoints.front();
    }
    const_reference front() const {
        return _lpoints.front();
    }

    reference back() {
        return _lpoints.back();
    }
    const_reference back() const {
        return _lpoints.back();
    }

    void push_back(const Point& p) {
        _lpoints.push_back(p);
    }

    void pop_back() {
        _lpoints.pop_back();
    }

    void clear() {
        _lpoints.clear();
    }

    unsigned int size() const {
        return _lpoints.size();
    }

    bool is_simple() const {
        return _is_simple(this->begin(), this->end(), this->closed());
    }

    void reverse() {
        this->_lpoints.reverse();
    }

    Point geometry_center() const{
        Point res;
        int count = 0;
        for(auto& p : _lpoints){
            res += p;
            count++;
        }
        return res / double(count);
    }

    double perimeter() const {
        if (empty()) {
            return 0;
        }
        double res = 0;
        const_iterator iter_end = std::prev(_lpoints.end(), 1);
        for (const_iterator iter = _lpoints.begin();
                iter != iter_end; ++iter) {
            const_iterator iter1 = std::next(iter);
            res += Distance(*iter, *iter1);
        }
        if (closed()) {
            res += Distance(*iter_end, *(_lpoints.begin()));
        }
        return res;
    }

    Box box() const {
        Point min = this->_lpoints.front();
        Point max = this->_lpoints.front();
        for(auto& p: this->_lpoints){
            min = Min(min, p);
            max = Max(max, p);
        }
        return Box(min, max);
    }

    typedef std::function<void(Point&, Point&)> FunPP;
    typedef std::function<void(const Point&, const Point&)> FunConstPConstP;

    void for_each_edge(FunPP fun){
        auto iters = _lpoints.begin();
        auto itere = _lpoints.begin();
        std::advance(itere, 1);
        for(; itere != _lpoints.end();){
            fun((*iters), (*itere));
            iters++;
            itere++;
        }
    }

    void for_each_edge(FunConstPConstP fun) const{
        auto iters = _lpoints.begin();
        auto itere = _lpoints.begin();
        std::advance(itere, 1);
        for (; itere != _lpoints.end();) {
            fun((*iters), (*itere));
            iters++;
            itere++;
        }
        if(this->closed()){
            fun(_lpoints.back(), _lpoints.front());
        }
    }

    void show() const {
        int count = 0;
        for (auto& p : _lpoints) {
            std::cout << "i = " << count << " " << p << std::endl;
            count++;
        }
    }
private:
    bool _is_intersect(const Point& s1, const Point& e1,
                       const Point& s2, const Point& e2) const{
        IntersectionPairSS_<TYPE, DIM> inter(s1, e1, s2, e2);
        auto t = inter.cal_intersection_type();
        if(t != _SS_NO_){
            return true;
        }else{
            return false;
        }
    }

    template<typename _ForwardIterator>
    bool _is_simple(_ForwardIterator begin,
            _ForwardIterator end,
            bool isclose = false) const{
        bool sametype = std::is_same<typename _ForwardIterator::value_type,
                Point>::value;
        ASSERT(sametype == true);

        typedef _ForwardIterator iterator;

        long count = 0;
        for (iterator iter = begin; iter != end; ++iter) {
            count++;
            if (count > 3) {
                break;
            }
        }
        if (count <= 3) {
            return true;
        }
        //
        iterator iter_end = end;
        std::advance(iter_end, -1);
        iterator iter_end2 = end;
        std::advance(iter_end2, -1);
        for (iterator iter0 = begin; iter0 != iter_end; ++iter0) {
            iterator iter1 = std::next(iter0);

            iterator iterm0 = std::prev(iter0);
            for (iterator iter2 = begin; iter2 != iter_end; ++iter2) {
                iterator iter3 = std::next(iter2);
                if (iter2 == iterm0 || iter2 == iter0 || iter2 == iter1) {
                    continue;
                }
                bool res = _is_intersect(*iter0, *iter1, *iter2, *iter3);
//				std::cout << " inter = " << res << "\n";
                if (res == true) {
                    return false;
                }
            }
        }
        if (isclose) {
            iter_end = end;
            std::advance(iter_end, -2);
            iterator iter0 = std::prev(end, 1);
            iterator iter1 = begin;
            for (iterator iter2 = std::next(iter1); iter2 != iter_end;
                    ++iter2) {
                iterator iter3 = std::next(iter2);

                bool res = _is_intersect(*iter0, *iter1, *iter2, *iter3);
//				std::cout << " inter = " << res << "\n";
                if (res == true) {
                    return false;
                }
            }
        }
        return true;
    }
};
// Only for 2D
template<class TYPE>
bool IsInOn(const Point_<TYPE, 2>& p, const PointChain_<TYPE, 2>& pc){
    // winding number method
    int    wn = 0;    // the  winding number counter

    typedef Point_<TYPE, 2> Point;
    pc.for_each_edge([&p, &wn](const Point& ps, const Point& pe){
        if(ps.y() <= p.y()){
            if(pe.y() >= p.y()){
                auto pos = OnWhichSide7(ps, pe, p);
                if(pos == _PS_LEFT_){
                    ++wn;
                }else if(  pos == _PS_ON_END_
                        || pos == _PS_ON_START_
                        || pos == _PS_IN_){
                    wn = 1; // force winding number be a positive number
                    return;
                }
            }
        }else{
            if(pe.y() <= p.y()){
                auto pos = OnWhichSide7(ps, pe, p);
                if(pos == _PS_RIGHT_){
                    --wn;
                }else if(  pos == _PS_ON_END_
                        || pos == _PS_ON_START_
                        || pos == _PS_IN_){
                    wn = 1; // force winding number be a positive number
                    return;
                }
            }
        }
    });
    return wn > 0;
}

template<class TYPE>
PointChain_<TYPE, 2> ProjectToXY(const PointChain_<TYPE, 3>& pc){
    PointChain_<TYPE, 2> res;
    for(auto& p : pc){
        res.push_back({p.x(), p.y()});
    }
    
    return res;
}
template<class CONTAINER, 
        typename std::enable_if<   
                   IsContainer<CONTAINER>::value 
                && IsPoint<typename CONTAINER::value_type>::value
        , bool>::type = true>
auto ProjectAlong(const CONTAINER& pc, Axes a){
    Axes d1, d2;
    NormalPlane(a, d1, d2);
    PointChain_<typename CONTAINER::value_type::value_type, 2> res;
    for(auto& p : pc){
        res.push_back({p(d1), p(d2)});
    }
    return res;
}
template<class TYPE>
auto NormalFirstThree(const PointChain_<TYPE, 3>& pc){
    typedef const typename PointChain_<TYPE, 3>::value_type Point;
    typedef const Point* cpPoint;
    if (pc.size() < 3){
        throw std::invalid_argument("Length of PointChain is less than 3");
    }
    std::array<cpPoint, 3> arr;
    int i = 0;
    for(auto& p : pc){
        if (i > 2){
            break;
        }
        arr[i] = &p;
        i++;
    }
    auto v1 = *(arr[1]) - *(arr[0]);
    auto v2 = *(arr[2]) - *(arr[0]);
    return Cross(v1, v2);
}

}

#endif
