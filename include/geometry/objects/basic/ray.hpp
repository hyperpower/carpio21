#ifndef _RAY_HPP_
#define _RAY_HPP_

#include "geometry/geometry_define.hpp"
#include "point.hpp"
#include <array>
#include <type_traits>
#include <typeinfo>
#include "math.h"

namespace carpio {
struct TagRay: public TagGeometry {
	TagRay() {
	};
};

template<typename TYPE, St DIM>
class Ray_: public std::array< Point_<TYPE, DIM>, 2>{
public:
	static const St Dim = DIM;
	typedef TagRay Tag;
	typedef TYPE                Vt;
	typedef       Vt&       ref_Vt;
	typedef const Vt& const_ref_Vt;
	typedef       Ray_<TYPE, DIM>            Self;
	typedef       Ray_<TYPE, DIM>&       ref_Self;
	typedef const Ray_<TYPE, DIM>& const_ref_Self;
	typedef Point_<TYPE, DIM>      Point;
	typedef       Point*          pPoint;
	typedef       Point&       ref_Point;
	typedef const Point& const_ref_Point;

public:
	Ray_():
		std::array<Point, 2>() {
				_set_empty();
	}
	Ray_(const Point& o, const Point& v) {
		this->po() = o;
		this->pv() = v;
		this->pv().normalize();
	}

	Ray_(const_ref_Self rhs) {
		this->po() = rhs.po();
		this->pv() = rhs.pv();
		this->pv().normalize();
	}
	void reconstruct(const_ref_Self rhs) {
		this->po() = rhs.po();
		this->pv() = rhs.pv();
		this->pv().normalize();
	}
	void reconstruct(const Point& s, const Point& e) {
		ASSERT(s != e);
		this->po() = s;
		this->pv() = e;
		this->pv().normalize();
	}
	void reconstruct(
			const Vt& ax, const Vt& bx,  //
			const Vt& ay, const Vt& by,  //
			const Vt& az = 0, const Vt& bz = 0) {
		Point s(ax, ay, az);
		Point e(bx, by, bz);
		reconstruct(s, e);
	}

	bool operator==(const_ref_Self rhs) const {
		return (this->po() == rhs.po() && this->pv() == rhs.pv()) ? true : false;
	}
	// po = point origin
	Point& po() {
		return this->at(0);
	}
	const Point& po() const {
		return this->at(0);
	}
	// pv = point vector
	const Point& pv() const {
		return this->at(1);
	}
	const_ref_Vt pox() const {
		return this->po().x();
	}
	const_ref_Vt pvx() const {
		return this->pv().x();
	}
	const_ref_Vt poy() const {
		ASSERT(Dim >= 2);
		return this->po().y();
	}
	const_ref_Vt pvy() const {
		ASSERT(Dim >= 2);
		return this->pv().y();
	}
	const_ref_Vt poz() const {
		ASSERT(Dim >= 3);
		return this->po().z();
	}
	const_ref_Vt pvz() const {
		ASSERT(Dim >= 3);
		return this->pv().z();
	}
	ref_Vt pox() {
		return this->po().x();
	}
	ref_Vt poy() {
		ASSERT(Dim >= 2);
		return this->po().y();
	}
	ref_Vt poz() {
		ASSERT(Dim >= 3);
		return this->po().z();
	}
	void show() const {
		std::cout.precision(4);
		std::cout << "(" << pox() << ", " << poy();
		if (Dim == 3) {
			std::cout << ", " << poz();
		} else {
			std::cout << "";
		}
		std::cout << ")-->(" << this->pvx() << ", " << pvy();
		if (Dim == 3) {
			std::cout << ", " << pvz();
		} else {
			std::cout << "";
		}
		std::cout << ")\n";
	}

	/** Set the beginning point */
	void set_o(const Point& p) {
		this->po() = p;
	}
		/** Set the end point */
	void set_v(const Point& p) {
		this->pv() = p;
		this->pv().normalize();
	}

protected:
	void _set_empty() {
		po().x() = 0.0;
		pv().x() = 0.0;
		po().y() = 0.0;
		pv().y() = 0.0;
		if (Dim == 3) {
			po().z() = 0.0;
			pv().z() = 0.0;
		}
	}

};

}

#endif
