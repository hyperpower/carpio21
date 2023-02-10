#include "geometry.hpp"
namespace ref
{
  

rat Point::get_abscissa() const {
  return x;
}

rat Point::get_ordinate() const {
  return y;
}

void Point::assign(const rat& x, const rat& y) {
  this->x = x, this->y = y;
  return ;
}

void Slope::make(const rat& x0, const rat& y0, const rat& x1, const rat&y1) {
  if(x0 == x1)
    type = infty;
  else {
    type = rational;
    value = (y0 - y1) / (x0 - x1) ;
  }

  return;
}

bool Slope::operator<(const Slope& s) const {
  if(type == infty)
    return false;
  else 
    return (s.type == infty || value < s.value);
}

Point Segment::get_left() const {
  return left;
}

Point Segment::get_right() const {
  return right;
}

bool Segment::intersect(const Segment& seg, Point& inter) const {
  // names
  Point s_left = seg.get_left(); Point s_right = seg.get_right();
  
  rat xa = left.get_abscissa(), ya = left.get_ordinate(),
    xb = right.get_abscissa(), yb = right.get_ordinate(),
    xc = s_left.get_abscissa(), yc = s_left.get_ordinate(),
    xd = s_right.get_abscissa(), yd = s_right.get_ordinate();

  static rat zero(I(0)), one(I(1));

  // determinant
  rat delta = (xb - xa)*(yc - yd) - (yb - ya)*(xc - xd);

  // Delta = 0 => no instersection
  if(delta == zero)
    return false;

  // computing r,s : Cramer's solutions
  rat r = (xc - xa)*(yc - yd) - (yc - ya)*(xc - xd); r /= delta;
  rat s = (yc - ya)*(xb - xa) - (xc - xa)*(yb - ya); s /= delta;
  
  // r, s in [0,1] else no intersection
  if(r<zero || r>one || s<zero || s>one)
    return false;

  rat xi = xa + r*(xb - xa), yi = ya + r*(yb - ya);
  inter.assign(xi, yi);
  
  return true;
}

rat Segment::high(const Point& p_sweep) const {
  static rat py, ly, ry;
  if(slope.type == infty) { // find max y if vertical
    py = p_sweep.get_ordinate();
    ly = left.get_ordinate();
    ry = right.get_ordinate();
    if(py < ly)
      return ly;
    else if(py > ry)
      return ry;
    else
      return py;
  }else {
    static rat xa, xb, ya, yb;
    xa = left.get_abscissa(),  ya = left.get_ordinate(),
    xb = right.get_abscissa(), yb = right.get_ordinate();
    
    return (yb - ya)/(xb - xa)*(p_sweep.get_abscissa() - xa) + ya;
  }
}

bool Segment::less(const Segment& s, const Point& p_sweep) const {
   return (
           this->high(p_sweep) < s.high(p_sweep)
           || (
               this->high(p_sweep) == s.high(p_sweep)
               && this->slope < s.slope
               )
           );
}

bool Segment::is_rend(const Point& p) const {
  return (p.get_abscissa() == right.get_abscissa()
          && p.get_ordinate() == right.get_ordinate());
}

bool Segment::is_lend(const Point& p) const {
  return (p.get_abscissa() == left.get_abscissa()
          && p.get_ordinate() == left.get_ordinate());
}

bool Segment::is_in(const Point& p) const {
  //assuming p is on the segment
  return ((! is_rend(p)) && (! is_lend(p)));
}

// /**
//  * \brief Pretty-printing for mpz_class
//  * \param begin Ostream
//  * \param z Integer to print
//  * \return Ostream begin concatenated with the printing of z 
//  */
// std::ostream& operator<<(std::ostream& begin, const mpz_class& z) {
//   begin << z.get_str();
//   return begin;
// }

// /**
//  * \brief Pretty-printing for mpq_class
//  * \param begin Ostream
//  * \param f Rational to print
//  * \return Ostream begin concatenated with the printing of f 
//  */
// std::ostream& operator<<(std::ostream& begin, const mpq_class& f) {
//   begin << f.get_num() << "/" << f.get_den();
//   return begin;
// }

std::ostream& operator<<(std::ostream& begin, const Segment*& s) {
  begin << *s;
  return begin;
}

std::ostream& operator<<(std::ostream& begin, const Point& p) {
  begin << "(" << p.get_abscissa()
        << "," << p.get_ordinate()
        << ")";
  return begin;
}

std::ostream& operator<<(std::ostream& begin, const Event& e) {
  begin << e.get_point() ;
  return begin;
}

std::ostream& operator<<(std::ostream& begin, const Segment& s) {
  begin << "[(" << s.get_left().get_abscissa()
        << "," << s.get_left().get_ordinate()
        << "),(" << s.get_right().get_abscissa()
        << "," << s.get_right().get_ordinate()
        << ")]";
  return begin;
}

bool Event::operator<(const Event& e) const {
  return (
          (point.get_abscissa() < e.point.get_abscissa())
          || (
              (point.get_abscissa() == e.point.get_abscissa())
              && (point.get_ordinate() < e.point.get_ordinate())
              )
          );
}

Point Event::get_point() const {
  return point;
}

} // namespace ref