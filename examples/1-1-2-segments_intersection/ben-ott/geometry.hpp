#ifndef H_GEO
#define H_GEO

#include <vector>
#include <iostream>
#include "utility/rational.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
// #include "integer_type.hpp"

#if USE_BIGINT

typedef mpz_class I;
typedef mpq_class rat;

#else 

typedef int I;
typedef tcb::Rational_<long> rat;

#endif
namespace ref{
  
class Point {
public:
  typedef carpio::PointTag Tag;
  static const short Dim = 2;
private:
  rat x; 
  rat y; 

public:
 
  Point() : x(), y() {};

  Point(const Point& p) : x(p.x), y(p.y) {};
  
  Point(const rat& xx, const rat& yy) : x(xx), y(yy) {};
  rat get_abscissa() const;
  rat get_ordinate() const;
  void assign(const rat&,const rat&);
  
  const rat& operator[](int index) const{
    if(index == 0){
      return x;
    }else if (index == 1){
      return y;
    }else{
      throw std::out_of_range("index");
    }
  }  //overload []
  rat& operator[](int index){
    if(index == 0){
      return x;
    }else if (index == 1){
      return y;
    }else{
      throw std::out_of_range("index");
    }
  }
};

inline std::string ToString(const Point& p,const std::string& sep){
        std::stringstream sstr;
        sstr.precision(4);
        sstr << std::scientific << p[0];
        sstr << sep << p[1];
        return sstr.str();
    }

enum slope_t {undef, rational, infty};

struct Slope {
  slope_t type; 
  rat value; 
  void make(const rat&,const rat&,const rat&,const rat&);
  bool operator<(const Slope&) const;
};

class Segment {
public:
  typedef carpio::SegmentTag Tag;
  static const short Dim = 2;
private:
  Point left; 
  Point right; 
  Slope slope; 

public:

  Segment() : left(), right() { slope.type = undef; };

  Segment(const Segment& seg) : left(seg.left), right(seg.right) {
    slope.type = seg.slope.type;
    if(slope.type == rational)
      slope.value = seg.slope.value;
  };

  Segment(const Point& a, const Point& b) : left(a), right(b) {
    slope.make(a.get_abscissa(), a.get_ordinate(), b.get_abscissa(), b.get_ordinate());
  };

  Segment(const rat& xa, const rat& xb, const rat& ya, const rat& yb)
    : left(xa, ya), right (xb, yb) {
    slope.make(xa,ya,xb,yb);
  };
  const Point& operator[](int index) const{
    if(index == 0){
      return left;
    }else if (index == 1){
      return right;
    }else{
      throw std::out_of_range("index");
    }
  }  //overload []
  Point& operator[](int index){
    if(index == 0){
      return left;
    }else if (index == 1){
      return right;
    }else{
      throw std::out_of_range("index");
    }
  }
  Point get_left() const;
  Point get_right() const;
  rat high(const Point&) const;
  bool less(const Segment&, const Point&) const;
  bool intersect(const Segment&, Point&) const;
  bool is_rend(const Point&) const;
  bool is_lend(const Point&) const;
  bool is_in(const Point&) const;
  bool empty() const{return false;};
};

// Pretty-printing
std::ostream& operator<<(std::ostream&, const Segment&);
std::ostream& operator<<(std::ostream&, const Segment*&);
std::ostream& operator<<(std::ostream&, const Point&);
//std::ostream& operator<<(std::ostream&, const mpz_class&);
//std::ostream& operator<<(std::ostream&, const mpq_class&);

class Event {
private:
  Point point; 
public:
  Event() : point() {};

  Event(const Event& e) : point(e.point) {};

  Event(const Point& p) : point(p) {};
  Point get_point() const;
  bool operator<(const Event&) const;
};

}

#endif //H_GEO