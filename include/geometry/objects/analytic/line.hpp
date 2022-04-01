#ifndef _LINE_HPP_
#define _LINE_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include <array>
#include <memory>

namespace carpio {
template<typename NUM>
NUM InterseptX(const NUM& a, const NUM& alpha){
	if(std::abs(a) < (_SMALL_)){
		return  alpha / (_SMALL_);
	}else{
		return alpha / a;
	}
}
template<typename NUM>
NUM InterseptX(const NUM& a, const NUM& alpha, const Vt& s){
	if(std::abs(a) < (s)){
		return  alpha / s;
	}else{
		return alpha / a;
	}
}

template<typename NUM>
NUM InterseptY(const NUM& b, const NUM& alpha, const Vt& s){
	if(std::abs(b) < s){
		return  alpha / (s);
	}else{
		return alpha / b;
	}
}

template<typename NUM>
NUM CalculateX(const NUM& a, const NUM& b, const NUM& alpha,  // The line
		       const NUM& value,                              // Y = value
		       const NUM& s = 1e-10){
	if(std::abs(a) < s){
		return (alpha - value * b) / s;
	} else {
		return (alpha - value * b) / a;
	}
}
template<typename NUM>
NUM CalculateY(const NUM& a, const NUM& b, const NUM& alpha,  // The line
		       const NUM& value,                              // X = value
		       const NUM& s = 1e-10){
	if(std::abs(b) < s){
		return (alpha - value * a) / s;
	} else {
		return (alpha - value * a) / b;
	}
}
template<typename NUM>
NUM Calculate(const NUM& a,  const NUM& b, const NUM& alpha,  // The line
		      const Axes& axe, const NUM& value,              // Axes = value
		      const NUM& s = 1e-10){
	if(axe == _X_){
		return CalculateY(a, b, alpha, value, s);
	}else{
		return CalculateX(a, b, alpha, value, s);
	}
}
template<typename NUM>
std::array<NUM, 3> Construct(const NUM& ax, const NUM& ay,
                             const NUM& bx, const NUM& by) {
        //assert(!isEqual(ax, bx) || !isEqual(ay,by));
        std::array<NUM, 3> res;
        if (ax == bx) {
            res[0] = 1;
            res[1] = 0;
            res[2] = ax;
        } else if (ay == by) {
            res[0] = 0;
            res[1] = 1;
            res[2] = ay;
        } else {
            res[0] = 1.0 / (ax - bx);
            res[1] = -1.0 / (ay - by);
            res[2] = bx / (ax - bx) - by / (ay - by);
        }
        return res;
    }

struct LineTag: public GeometryTag {};

template<typename TYPE>
class Line_: public std::array<TYPE, 3> {
	//The Line function defined as ax+by=alpha
public:
    static const St Dim = 2;
	typedef St size_type;
	typedef TYPE Vt;
	typedef TYPE& reference;
	typedef const TYPE& const_reference;
	typedef Line_<TYPE> Line;
	typedef LineTag Tag;

	typedef Point_<Vt, 2> Point;
	// std like
	typedef TYPE value_type;
public:
	Line_() :
			std::array<Vt, 3>() {
	}
	Line_(const Vt& a, const Vt& b, const Vt& alpha) {
		this->at(0) = a;
		this->at(1) = b;
		this->at(2) = alpha;
	}
	Line_(Vt ax, Vt ay, Vt bx, Vt by) {
		//assert(!isEqual(ax, bx) || !isEqual(ay,by));
		auto arr = Construct(ax, ay, bx, by);
		this->at(0) = arr[0];
		this->at(1) = arr[1];
		this->at(2) = arr[2];
	}
	Line_(const Point &p1, const Point &p2) {
		auto arr = Construct(p1.x(), p1.y(), p2.x(), p2.y());
		this->at(0) = arr[0];
		this->at(1) = arr[1];
		this->at(2) = arr[2];
	}
	Line_(const Line& l){
		this->at(0) = l[0];
		this->at(1) = l[1];
		this->at(2) = l[2];
	}
	Line_(std::initializer_list<TYPE> l) {
		auto b = l.begin();
		if(l.size() == 3){
			this->reconstruct(*b, *(std::next(b)), *(std::next(b,2)));
		}else if(l.size() == 4){
			auto arr = Construct(*b, *(std::next(b)), *(std::next(b,2)), *(std::next(b, 3)));
			this->at(0) = arr[0];
			this->at(1) = arr[1];
			this->at(2) = arr[2];
		}
    }
	void reconstruct(Vt a, Vt b, Vt c) {
		if (a == 0.0 && b == 0.0) {
			a = _SMALL_;
			b = _SMALL_;
		}
		this->a() = a;
		this->b() = b;
		this->alpha() = c;
	}
	inline reference a() {
		return this->at(0);
	}
	inline reference b() {
		return this->at(1);
	}
	inline reference alpha() {
		return this->at(2);
	}
	inline const_reference a() const {
		return this->at(0);
	}
	inline const_reference b() const {
		return this->at(1);
	}
	inline const_reference alpha() const {
		return this->at(2);
	}
	Vt cal_x(Vt y) const {
		return CalculateX(this->a(), this->b(), this->alpha(), y);
	}
	Vt cal_y(Vt x) const {
		return CalculateY(this->a(), this->b(), this->alpha(), x);
	}
	// _X_=v ---> value of _Y_
	// _Y_=v ---> value of _X_
	Vt cal(Axes a, Vt v) const{
		return Calculate(this->a(), this->b(), this->alpha(), a, v, _SMALL_);
	}
	Vt slope() const {
		return -this->a() / (this->b() + _SMALL_);
	}
	Vt intersept_x() const {
		return InterseptX(this->a(), this->alpha(), _SMALL_);
	}
	Vt intersept_y() const {
		return InterseptY(this->b(), this->alpha(), _SMALL_);
	}
	Vt intersept(Axes aix) const {
		ASSERT(aix!=_Z_);
		if(aix == _X_){
			return intersept_x();
		}else{
			return intersept_y();
		}
	}
	Vt norm_x() const {
		return this->a();
	}
	Vt norm_y() const {
		return this->b();
	}
	// (x,y) is another vector
	// true  --- (x, y) direction is normal to the line
	// false --- (x, y) direction is not norma to he line
	bool is_norm(Vt x, Vt y, Vt s = 1e-10) const{
		Vt dot = this->shear_x() * x + this->shear_y() * y;
		if (std::abs(dot) < s){
			return true;
		}else{
			return false;
		}
	}
	Vt shear_x() const {
		return this->b();
	}
	Vt shear_y() const {
		return -(this->a());
	}
	bool empty() const {
		if (this->a() != 0.0 || this->b() != 0.0) {
			return true;
		} else {
			return false;
		}
	}
	// line can be written to Parametric Form
	// Ref: Geometric tools for computer graphics P174
	Point parametric_p() const{
		const Vt& a = this->at(0);
		const Vt& b = this->at(1);
		const Vt& c = this->at(2);
		Vt s = a * a + b * b;
		s = (s == 0) ? _SMALL_ : s;
		return Point(-a * c / s, -b *c /s);
	}

	Point parametric_d() const{
		return Point(-(this->at(1)), this->at(0));
	}

	template<typename T>
	void transfer(const T&dx, const T& dy) {
		this->alpha() = this->a() * dx + this->b() *dy + this->alpha();
	}

	static std::shared_ptr<Point> Intersect(Vt a1, Vt b1, Vt c1,
			                                Vt a2, Vt b2, Vt c2){
		double det = a1 * b2 - a2 * b1;
		if(std::abs(det) < 1e-14){
			return std::shared_ptr<Point>(nullptr);
		}
		double x   = (b2 * c1 - b1 * c2) / det;
		double y   = (a1 * c2 - a2 * c1) / det;
		return std::make_shared<Point>(Point(x, y));
	}

	static std::shared_ptr<Point> Intersect(const Line& l1, const Line& l2){
		double det = l1.a() * l2.b() - l2.a() * l1.b();
		if(std::abs(det) < 1e-14){
			return std::shared_ptr<Point>(nullptr);
		}
		double x   = (l2.b() * l1.alpha() - l1.b() * l2.alpha()) / det;
		double y   = (l1.a() * l2.alpha() - l2.a() * l1.alpha()) / det;
		return std::make_shared<Point>(Point(x, y));
	}
};

template<typename TYPE>
std::ostream& operator<<(std::ostream& stream, const Line_<TYPE>& line) {
	stream << std::setprecision(4) 
	       << line.a() << " X + "
		   << line.b() << " Y = "
		   << line.alpha();
	return stream;
}

}

#endif
