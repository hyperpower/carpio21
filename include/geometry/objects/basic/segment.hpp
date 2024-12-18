#ifndef _SEGMENT_HPP_
#define _SEGMENT_HPP_

#include "geometry/geometry_define.hpp"
#include "algebra/number/number.hpp"
#include "point.hpp"
//#include "box.hpp"
#include <array>
#include <vector>
#include <type_traits>
#include <typeinfo>
#include "math.h"

namespace carpio {
struct SegmentTag: public GeometryTag {};

template<typename TYPE, St DIM> class Box_;

template<class ANY>
struct IsSegment{
private:
	typedef char yes;
    typedef long no;

    template<class T, typename std::enable_if<
            std::is_base_of<SegmentTag, typename T::Tag>::value, bool>::type = true > 
    static yes  test(int i, typename T::Tag = typename T::Tag());
    template<class T>
    static no test(...);
public:
    static const bool value = sizeof(test<ANY>(0)) == sizeof(yes);

	typedef typename std::integral_constant<bool, value>::type type;
};

template<typename TYPE, St DIM>
class Segment_: public std::array< Point_<TYPE, DIM>, 2> {
public:
	static const St Dim = DIM;
	typedef SegmentTag Tag;
	typedef TYPE Vt;
	typedef TYPE coord_value_type;
	typedef Vt& ref_Vt;
	typedef const Vt& const_ref_Vt;
	typedef Segment_<TYPE, DIM> Self;
	typedef Segment_<TYPE, DIM>& ref_Self;
	typedef const Segment_<TYPE, DIM>& const_ref_Self;
	typedef Point_<TYPE, DIM> Point;
    typedef Point value_type;
	typedef Point* pPoint;
	typedef Point& ref_Point;
	typedef const Point& const_ref_Point;
	typedef Box_<Vt, Dim> Box;
	//typedef Operation_<Vt, Dim> Op;

public:
	Segment_() :
			std::array<Point, 2>() {
		_set_empty();
	}
	Segment_(const Point& s, const Point& e) {
		ASSERT(s != e);
		this->ps() = s;
		this->pe() = e;
	}
	Segment_(const Vt& ax, const Vt& bx, //
			const Vt& ay, const Vt& by,  //
			const Vt& az = 0, const Vt& bz = 0) {
		Point s(ax, ay, az);
		Point e(bx, by, bz);
		reconstruct(s, e);
	}
	Segment_(const_ref_Self rhs) {
		this->ps() = rhs.ps();
		this->pe() = rhs.pe();
	}
	void reconstruct(const_ref_Self rhs) {
		this->ps() = rhs.ps();
		this->pe() = rhs.pe();
	}
	void reconstruct(const Point& s, const Point& e) {
		// ASSERT(s != e);
		this->ps() = s;
		this->pe() = e;
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
		return (this->ps() == rhs.ps() && this->pe() == rhs.pe()) ? true : false;
	}

	ref_Point p(int i){
		ASSERT( (i==0) || (i==1));
		return this->operator [](i);
	}

    const_ref_Point p(int i) const{
		ASSERT( (i==0) || (i==1));
		return this->operator [](i);
	}
	ref_Point p_less_x(){
		return (pex() < psx()) ? p(1) : p(0);
	}
	const_ref_Point p_less_x() const{
		return (pex() < psx()) ? p(1) : p(0);
	}
	ref_Point p_less_y(){
		return (pey() < psy()) ? p(1) : p(0);
	}
	const_ref_Point p_less_y() const{
		return (pey() < psy()) ? p(1) : p(0);
	}
	ref_Point p_greater_x(){
		return (pex() < psx()) ? p(0) : p(1);
	}
	const_ref_Point p_greater_x() const{
		return (pex() < psx()) ? p(0) : p(1);
	}
	ref_Point p_greater_y(){
		return (pey() < psy()) ? p(0) : p(1);
	}
	const_ref_Point p_greater_y() const{
		return (pey() < psy()) ? p(0) : p(1);
	}
	Point& ps() {
		return this->at(0);
	}
	Point& ps(const Point& other) {
		this->at(0) = other;
		return this->at(0);
	}
	const Point& ps() const {
		return this->at(0);
	}
	Point& pe() {
		return this->at(1);
	}
	Point& pe(const Point& other) {
		this->at(1) = other;
		return this->at(1);
	}
	const Point& pe() const {
		return this->at(1);
	}
	Point pc() const {
		return Point((pex() + psx()) * 0.5, (pey() + psy()) * 0.5,
				(Dim == 3) ? ((pez() + psz()) * 0.5) : 0);
	}
	const_ref_Vt psx() const {
		return this->ps().x();
	}
	const_ref_Vt pex() const {
		return this->pe().x();
	}
	const_ref_Vt psy() const {
		ASSERT(Dim >= 2);
		return this->ps().y();
	}
	const_ref_Vt pey() const {
		ASSERT(Dim >= 2);
		return this->pe().y();
	}
	const_ref_Vt psz() const {
		ASSERT(Dim >= 3);
		return this->ps().z();
	}
	const_ref_Vt pez() const {
		ASSERT(Dim >= 3);
		return this->pe().z();
	}
	void   psx(const Vt& v){ (*this)[0][0] = v;}
	ref_Vt psx()           { return (*this)[0][0]; }
	
	void   pex(const Vt& v){ (*this)[1][0] = v;}
	ref_Vt pex() { return this->pe().x(); }
	
	ref_Vt psy() {
		ASSERT(Dim >= 2);
		return this->ps().y();
	}
	ref_Vt pey() {
		ASSERT(Dim >= 2);
		return this->pe().y();
	}
	ref_Vt psz() {
		ASSERT(Dim >= 3);
		return this->ps().z();
	}
	ref_Vt pez() {
		ASSERT(Dim >= 3);
		return this->pe().z();
	}
	Vt dx() const {
		return pex() - psx();
	}
	Vt dy() const {
		return pey() - psy();
	}
	Vt dz() const {
		return pez() - psz();
	}
	Vt length() const {
		Vt len = 0.0;
		len = sqrt(
				double(
						(psx() - pex()) * (psx() - pex())
								+ (psy() - pey()) * (psy() - pey())));
		return len;
	}
	Vt slope() const {
		ASSERT(Dim == 2);
		return (pey() - psy()) / (pex() - psx() + _SMALL_);
	}

	Box box() const {
		Point min = Min(ps(), pe());
		Point max = Max(ps(), pe());
		return Box(min, max);
	}
	Point mid() const {
        Point res;
        for (int i = 0; i <Dim; ++i){
            res[i] = ((*this)[0][i] + (*this)[1][i]) * 0.5;
        }
        return res;
    }

	void scale(const Point& pc, Vt xfactor, Vt yfactor, Vt zfactor = 1) {
        std::vector<Vt> fvec = {{xfactor, yfactor, zfactor}};
        for(auto& p : (*this)){
            for(int i=0; i<Dim; ++i){
                p[i] = fvec[i] * (p[i] - pc[i]) + pc[i];
            }
        }
    }

	void scale(Vt xfactor, Vt yfactor, Vt zfactor = 1) {
		std::vector<Vt> fvec = {{xfactor, yfactor, zfactor}};
        for(auto& p : (*this)){
            for(int i=0; i<Dim; ++i){
                p[i] = fvec[i] * p[i];
            }
        }
		if (pe() == ps()) {
			_set_empty();
		}
	}
	void transfer(Vt dx, Vt dy, Vt dz = 0.0) {
		if (!empty()) {
			psx() = psx() + dx;
			psy() = psy() + dy;
			pex() = pex() + dx;
			pey() = pey() + dy;
		}
		if (Dim == 3) {
			psz() = psz() + dz;
			pez() = pez() + dz;
		}
	}

	void rotate90() {
		auto dx = this->dx();
		auto dy = this->dy();
    	this->pex() -= dy;
    	this->pey() += dx;
	}

	Point tangent_vector() const{
		Point nv;
		for(int d = 0; d<Dim; d++){
			nv[d] = (*this)[1][d] - (*this)[0][d];
		}
		return nv;
	}	

	Point normal_unit_vector() const{
		Point nv;
		for(int d = 0; d<Dim; d++){
			nv[d] = (*this)[1][d] - (*this)[0][d];
		}
		if constexpr (Dim > 1){
			auto x = -nv[1];
			nv[1] = nv[0];
			nv[0] = x;
		}else{
			throw std::logic_error("Dim = 1, no normal unit vector");
		}
		Normalize(nv);
		return nv;
	}

	/** Set the beginning point */
	void set_s(const Point& p) {
		this->ps() = p;
	}
	/** Set the end point */
	void set_e(const Point& p) {
		this->pe() = p;
	}

	/** Change the segment orientation */
	Self& change_orientation() {
		Point tmp = this->ps();
		this->ps() = this->pe();
		this->pe() = tmp;
		return *this;
	}

	bool empty() const {
		if (psx() == Zero(psx()) && psy() == Zero(psy()) && pex() == Zero(pex()) && pey() == Zero(pey())
				&& ((Dim == 3) ? (psz() == Zero(psz()) && pez() == Zero(pez())) : true)) {
			return true;
		} else {
			return false;
		}
	}
	void show() const {
		std::cout.precision(4);
		std::cout << "(" << psx() << ", " << psy();
		if (Dim == 3) {
			std::cout << ", " << psz();
		} else {
			std::cout << "";
		}
		std::cout << ")-->(" << this->pex() << ", " << pey();
		if (Dim == 3) {
			std::cout << ", " << pez();
		} else {
			std::cout << "";
		}
		std::cout << ")\n";
	}

	/*
	 *  compare
	 */

	bool is_gt_x(const Vt& v) const {    //>
		return (this->pex() > v && this->psx() > v);
	}
	bool is_gt_y(const Vt& v) const {    //>
		return (this->pey() > v && this->psy() > v);
	}
	bool is_ge_x(const Vt& v) const {    //>=
		return (this->pex() >= v && this->psx() >= v);
	}
	bool is_ge_y(const Vt& v) const {    //>=
		return (this->pey() >= v && this->psy() >= v);
	}

	bool is_lt_x(const Vt& v) const {    //<
		return (this->pex() < v && this->psx() < v);
	}
	bool is_lt_y(const Vt& v) const {    //<
		return (this->pey() < v && this->psy() < v);
	}
	bool is_lt_z(const Vt& v) const {    //<
		ASSERT(Dim == 3);
		return (this->pez() < v && this->psz() < v);
	}
	bool is_le_x(const Vt& v) const {    //<=
		return (this->pex() <= v && this->psx() <= v);
	}
	bool is_le_y(const Vt& v) const {    //<=
		return (this->pey() <= v && this->psy() <= v);
	}

	bool is_vertical() const {
		ASSERT(!empty());
		return psx() == pex();
	}

	bool is_horizontal() const {
		ASSERT(!empty());
		return psy() == pey();
	}
	bool is_in_box(const Point& pt) const {
		ASSERT(!empty());
		if (is_horizontal()) {
			return (((psx() <= pt.x) && (pt.x <= pex()))
					|| ((pex() <= pt.x) && (pt.x <= psx())));
		}
		if (is_vertical()) {
			return (((psy() <= pt.y) && (pt.y <= pey()))
					|| ((pey() <= pt.y) && (pt.y <= psy())));
		}
		return (((psx() <= pt.x) && (pt.x <= pex()))
				|| ((pex() <= pt.x) && (pt.x <= psx())))
				&& (((psy() <= pt.y) && (pt.y <= pey()))
						|| ((pey() <= pt.y) && (pt.y <= psy())));
	}


protected:
	void _set_empty() {
		ps().x() = 0.0;
		pe().x() = 0.0;
		ps().y() = 0.0;
		pe().y() = 0.0;
		if (Dim == 3) {
			ps().z() = 0.0;
			pe().z() = 0.0;
		}
	}
};
//==================================================================
// Functions
//
template<typename TYPE, St DIM>
inline std::ostream& operator<<(std::ostream& o, const Segment_<TYPE, DIM>& p) {
	// o << std::fixed;
	o << std::setiosflags (std::ios::scientific);
	o << std::setprecision(4);
	return o << p.ps() << " --> " << p.pe();
}
template<typename TYPE, St DIM>
inline std::string ToString(const Segment_<TYPE, DIM>& p){
    std::ostringstream sst;
    sst << p;
	return sst.str(); 
}

template<typename TYPE, St DIM>
inline TYPE MinX(const Segment_<TYPE, DIM>& p) {
	return p.psx() < p.pex() ? p.psx() : p.pex();
}
template<typename TYPE, St DIM>
inline TYPE MinX(const Segment_<TYPE, DIM>& s1, const Segment_<TYPE, DIM>& s2) {
	auto vs1 = MinX(s1);
	auto vs2 = MinX(s2);
	return vs1 < vs2 ? vs1 : vs2;
}
template<typename TYPE, St DIM>
inline TYPE MinY(const Segment_<TYPE, DIM>& p) {
	return p.psy() < p.pey() ? p.psy() : p.pey();
}
template<typename TYPE, St DIM>
inline TYPE MinY(const Segment_<TYPE, DIM>& s1, const Segment_<TYPE, DIM>& s2) {
	auto vs1 = MinY(s1);
	auto vs2 = MinY(s2);
	return vs1 < vs2 ? vs1 : vs2;
}
template<typename TYPE, St DIM>
inline TYPE MaxX(const Segment_<TYPE, DIM>& p) {
	return p.psx() > p.pex() ? p.psx() : p.pex();
}
template<typename TYPE, St DIM>
inline TYPE MaxX(const Segment_<TYPE, DIM>& s1, const Segment_<TYPE, DIM>& s2) {
	auto vs1 = MaxX(s1);
	auto vs2 = MaxX(s2);
	return vs1 > vs2 ? vs1 : vs2;
}
template<typename TYPE, St DIM>
inline TYPE MaxY(const Segment_<TYPE, DIM>& p) {
	return p.psy() > p.pey() ? p.psy() : p.pey();
}
template<typename TYPE, St DIM>
inline TYPE MaxY(const Segment_<TYPE, DIM>& s1, const Segment_<TYPE, DIM>& s2) {
	auto vs1 = MaxY(s1);
	auto vs2 = MaxY(s2);
	return vs1 > vs2 ? vs1 : vs2;
}
template<typename TYPE, St DIM>
inline auto EnclosureDiagonal(const Segment_<TYPE, DIM>& s1, const Segment_<TYPE, DIM>& s2) {
	return Segment_<TYPE, DIM>(MinX(s1, s2), MaxX(s1, s2), MinY(s1, s2), MaxY(s1, s2));	
}

// Point loaction relative to a segment in 2D
// It has 7 possibilities
// ================================
//   0  |  o--x-->    |  IN
// ________________________________
//   1  |     x       |  LEFT
//      |  o----->    |
// ________________________________
//   2  |  o----->    |  RIGHT
//      |     x       |
// ________________________________
//   3  |--x---->---  |  ON_START
// ________________________________
//   4  |--o----x---  |  ON_END
// ________________________________
//   5  |-x-o---->--  |  OUT_START
// ________________________________
//   6  |---o---->-x- |  OUT_END
enum PointToSegmentPosition {
	_PS_IN_        = 0,  //
	_PS_LEFT_      = 1,  //
	_PS_RIGHT_     = 2,  //
	_PS_ON_START_  = 3,  //
	_PS_ON_END_    = 4,  //
	_PS_OUT_START_ = 5,  //
	_PS_OUT_END_   = 6,  //
};

inline PointToSegmentPosition ToPointToSegmentPosition(int idx){
	switch(idx){
		case 0 : return _PS_IN_       ;  break;
		case 1 : return _PS_LEFT_     ;  break;
		case 2 : return _PS_RIGHT_    ;  break;
		case 3 : return _PS_ON_START_ ;  break;
		case 4 : return _PS_ON_END_   ;  break;
		case 5 : return _PS_OUT_START_;  break;
		case 6 : return _PS_OUT_END_  ;  break;
	}
	SHOULD_NOT_REACH;
	return _PS_IN_; //make complier happy
}

inline std::string ToString(const PointToSegmentPosition& ps){
	switch(ps){
	case _PS_IN_       :  return "IN";        break;
	case _PS_LEFT_     :  return "LEFT";      break;
	case _PS_RIGHT_    :  return "RIGHT";     break;
	case _PS_ON_START_ :  return "ON_START";  break;
	case _PS_ON_END_   :  return "ON_END";    break;
	case _PS_OUT_START_:  return "OUT_START"; break;
	case _PS_OUT_END_  :  return "OUT_END";   break;
	}
	SHOULD_NOT_REACH;
	return "ERROR PointToSegmentPosition";
}

// For 1D
// NO _PS_LEFT_ and _PS_RIGHT_
template<typename TYPE>
PointToSegmentPosition OnWhichSide5Increase(
		const TYPE& min,  // start
		const TYPE& max,  // end
		const TYPE& v){
	ASSERT(min <= max);
	if (v ==  min){
		return _PS_ON_START_;
	}else if(v == max){
		return _PS_ON_END_;
	}else if (v < min){
		return _PS_OUT_START_;
	}else if (v > max){
		return _PS_OUT_END_;
	}else{
		return _PS_IN_;
	}
}

template<typename TYPE>
PointToSegmentPosition OnWhichSide5Decrease(
		const TYPE& max,  // start
		const TYPE& min,  // end
		const TYPE& v){
	ASSERT(min >= max);
	if (v ==  min){
		return _PS_ON_END_;
	}else if(v == max){
		return _PS_ON_START_;
	}else if (v < min){
		return _PS_OUT_END_;
	}else if (v > max){
		return _PS_OUT_START_;
	}else{
		return _PS_IN_;
	}
}

template<typename TYPE>
PointToSegmentPosition OnWhichSide5(
		const TYPE& start,  // start
		const TYPE& end,    // end
		const TYPE& v){
	if(start <= end){
		return OnWhichSide5Increase(start, end, v);
	}else{
		return OnWhichSide5Decrease(start, end, v);
	}
}

// 2D
template<typename TYPE>
PointToSegmentPosition OnWhichSide7(
		const Point_<TYPE, 2>& ps,
		const Point_<TYPE, 2>& pe,
		const Point_<TYPE, 2>& p) {
	// 1 Is equal
	if(p == ps){
		return _PS_ON_START_;
	}
	if(p == pe){
		return _PS_ON_END_;
	}
	// 2 calculate cross
	double tmp = Orient(ps, pe, p);
	if (tmp > 0) {
		return _PS_LEFT_;
	} else if (tmp < 0) {
		return _PS_RIGHT_;
	} else {
		TYPE dx = pe.x() - ps.x();
		if (dx > 0) {
			if (p.x() > pe.x()) {
				return _PS_OUT_END_;
			}else if(p.x() < ps.x()){
				return _PS_OUT_START_;
			}else if(p.x() < pe.x() && p.x() > ps.x()){
				return _PS_IN_;
			}else{
				throw std::logic_error("Invalid Location");
			}
		} else if (dx < 0) {
			if (p.x() < pe.x()) {
				return _PS_OUT_END_;
			} else if (p.x() > ps.x()) {
				return _PS_OUT_START_;
			} else if (p.x() > pe.x() && p.x() < ps.x()) {
				return _PS_IN_;
			} else {
				throw std::logic_error("Invalid Location");
			}
		} else { //equal to zero
			TYPE dy = pe.y() - ps.y();
			if (dy > 0) {
				if (p.y() > pe.y()) {
					return _PS_OUT_END_;
				} else if (p.y() < ps.y()) {
					return _PS_OUT_START_;
				} else {
					return _PS_IN_;
				}
			} else if (dy < 0) {
				if (p.y() < pe.y()) {
					return _PS_OUT_END_;
				} else if (p.y() > ps.y()) {
					return _PS_OUT_START_;
				} else {
					return _PS_IN_;
				} 
			}else{
				throw std::logic_error("Invalid Location");
			}
		}
	}
}

template<typename TYPE>
PointToSegmentPosition OnWhichSide7(
		const Segment_<TYPE, 2>& seg,
		const Point_<TYPE, 2>& pe) {
	return OnWhichSide7(seg.ps(), seg.pe(), pe);
}

// ------>
//



}
#endif
