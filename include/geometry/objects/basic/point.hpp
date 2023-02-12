#ifndef _POINT_HPP_
#define _POINT_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/predicate.hpp"
#include <array>
#include <sstream>
#include <iostream>
#include <cmath>
#include <type_traits>


namespace carpio {

struct PointTag: public GeometryTag {};
struct PointViewOnYZTag: public PointTag {};

template<class ANY>
struct IsPoint{
private:
	typedef char yes;
    typedef long no;

    template<class T, typename std::enable_if<
            std::is_base_of<PointTag, typename T::Tag>::value, bool>::type = true > 
    static yes  test(int i, typename T::Tag = typename T::Tag());
    template<class T>
    static no test(...);
public:
    static const bool value = sizeof(test<ANY>(0)) == sizeof(yes);

	typedef typename std::integral_constant<bool, value>::type type;
};

template<class CONTAINER, 
         typename std::enable_if<
            IsContainer<CONTAINER>::value
        &&  std::is_arithmetic<typename CONTAINER::value_type>::value, bool>::type = true>
void Normalize(CONTAINER& con){
    double n = 0;
    for (auto& v : con) {
        n += double(v * v);
    }
    n = std::sqrt(n);
    for (auto& v : con) {
        if (n != 0) {
            v /= n;
        } else {
            v = 0;
        }
    }
}
//Point T ====================================
template<typename TYPE, St DIM>
class Point_: public std::array<TYPE, DIM> {
public:
    static const St Dim = DIM;
    typedef PointTag Tag;
    typedef Point_<TYPE, DIM> Point;
    typedef Point_<TYPE, DIM> Self;
	typedef TYPE coord_value_type;
    typedef TYPE CV;  // short for CoordinateValue  
    // std like
    typedef std::array<TYPE, DIM> Base;
    typedef typename Base::value_type value_type;
    typedef typename Base::size_type  size_type;
    typedef typename Base::difference_type  difference_type;
    typedef typename Base::reference  reference;
    typedef typename Base::const_reference  const_reference;
    typedef typename Base::pointer  pointer;
    typedef typename Base::const_pointer  const_pointer;

    typedef const Self& const_ref_Self;
    typedef       Self& ref_Self;

    //constructor
    Point_() :
            std::array<CV, Dim>() {
        reconstruct(0, 0, 0);
    }

    Point_(const CV& a, 
           const CV& b = 0, 
           const CV& c = 0) :std::array<CV, Dim>() {
        reconstruct(a, b, c);
    }

    Point_(std::initializer_list<TYPE> l) {
        St i = 0;
        for(auto& v : l){
            (*this)[i] = v;
            ++i;
            if(i >= Dim){
                break;
            }
        }
    }
    template<class CONTAINER,
        typename std::enable_if<
            std::integral_constant< bool, 
                   IsContainer<CONTAINER>::value 
                && std::is_arithmetic<typename CONTAINER::value_type>::value
            >::value, 
        bool>::type = true>
    Point_(const CONTAINER& con) {
        St i = 0;
        for(auto& v : con){
            (*this)[i] = v;
            ++i;
            if(i >= Dim){
                break;
            }
        }
    }
    const_reference operator()(St idx) const {
        ASSERT(idx < Dim);
        return this->at(idx);
    }
    reference operator()(St idx) {
        ASSERT(idx < Dim);
        return this->at(idx);
    }
    const_reference operator()(Axes a) const {
        ASSERT(St(a) < Dim);
        return this->at(St(a));
    }
    reference operator()(Axes a) {
        ASSERT(St(a) < Dim);
        return this->at(St(a));
    }
    CV value(St idx) const {
        CV res;
        if (idx < Dim) {
            return this->operator ()(idx);
        } else {
            return 0;
        }
    }
    bool is_zero(){
        for(auto& v : *this){
            if (v != 0 ){
                return false;
            }
        }
        return true;
    }

    void set(Axes a, Vt value){
        (*this)(a) = value;
    }
    void x(const Vt& value){
        (*this)(0) = value;
    }
    void y(const Vt& value){
        if constexpr ( Dim > 1){
            (*this)(1) = value;
        }else{
            throw std::out_of_range("not y");
        }
    }
    void z(const Vt& value){
        if constexpr ( Dim > 2){
            (*this)(1) = value;
        }else{
            throw std::out_of_range("not z");
        }
    }

    const_reference x() const {
        return this->at(0);
    }

    reference x() {
        return this->at(0);
    }

    const_reference y() const {
        ASSERT(Dim >= 2);
        return this->at(1);
    }

    reference y() {
        ASSERT(Dim >= 2);
        return this->at(1);
    }

    const_reference z() const {
        ASSERT(Dim == 3);
        return this->at(2);
    }

    reference z() {
        ASSERT(Dim == 3);
        return this->at(2);
    }

    void reconstruct(const CV& a, const CV& b = 0, const CV& c = 0) {
        if constexpr (DIM == 1){
            this->at(0) = a;
            return;
        }
        if constexpr (DIM == 2){
            this->at(0) = a;
            this->at(1) = b;
            return;
        }
        if constexpr (DIM == 3){
            this->at(0) = a;
            this->at(1) = b;
            this->at(2) = c;
            return;
        }

    }
public:   
    Point operator-() const{
        Point res(*this);
        for(auto& v : res){
            v = -v;
        }
        return res;
    }
    Point operator-() {
        Point res;
        res[0] = -(this->at(0));
        if constexpr (Dim >= 2) {
            res[1] = -(this->at(1));
        }
        if constexpr (Dim >= 3) {
            res[2] = -(this->at(2));
        }
        return res;
    } 
    bool operator<(const Point_<CV, Dim>& a) const {
        for (St i = 0; i < Dim; i++) {
            if (this->at(i) < a[i]) {
                return true;
            }
        }
        return false;
    }

    bool operator==(const Point_<CV, Dim> &a) const {
        if (Dim == 2) {
            return (this->at(0) == a[0] && this->at(1) == a[1]) ? true : false;
        } else {
            return (this->at(0) == a[0] && this->at(1) == a[1]
                    && this->at(2) == a[2]) ? true : false;
        }
    }
    bool operator!=(const Point_<CV, Dim> &a) const {
        if constexpr (Dim == 2) {
            return !((this->at(0) == a[0] && this->at(1) == a[1]) ? true : false);
        } else {
            return !(
            (this->at(0) == a[0] && this->at(1) == a[1]
                    && this->at(2) == a[2]) ? true : false);
        }
    }
    void show() const {
        std::cout << std::scientific << "( " << this->at(0);
        if (Dim >= 2) {
            std::cout << " , " << this->at(1);
        }
        if (Dim >= 3) {
            std::cout << " , " << this->at(2);
        }
        std::cout << " )\n";
    }

    std::string to_string() const {
        std::stringstream sstr;
        sstr.precision(4);
        sstr << std::scientific << "( " << this->at(0);
        if constexpr (Dim >= 2) {
            sstr << " , " << this->at(1);
        }
        if constexpr (Dim == 3) {
            sstr << " , " << this->at(2) << " )";
        } else {
            sstr << " )";
        }
        return sstr.str();
    }
    
    std::string to_string(const std::string& sep) const {
        std::stringstream sstr;
        sstr.precision(4);
        sstr << std::scientific << this->at(0);
        if constexpr (Dim >= 2) {
            sstr << sep << this->at(1);
        }
        if constexpr (Dim == 3) {
            sstr << sep << this->at(2);
        }
        return sstr.str();
    }
    template<typename T>
    void transfer(const T&dx, const T& dy = 0, const T& dz = 0) {
        this->at(0) = this->at(0) + CV(dx);
        this->at(1) = this->at(1) + CV(dy);
        if (Dim == 3) {
            this->at(2) = this->at(2) + CV(dz);
        }
    }
    template<typename T>
    void scale(const T&dx, const T&dy = 1, const T&dz = 1) {
        this->at(0) = this->at(0) * CV(dx);
        this->at(1) = this->at(1) * CV(dy);
        if (Dim == 3) {
            this->at(2) = this->at(2) * CV(dz);
        }
    }

    Axes max_axes() const{
        return this->_max_axes(typename DimTagTraits_<Dim>::Type());
    }
    Axes _max_axes(Dim2Tag) const{
        return this->at(0) >= this->at(1) ? _X_ : _Y_;
    }
    Axes _max_axes(Dim3Tag) const{
        auto idx = (this->at(0) >= this->at(1)) ? 0 : 1;
        idx = (this->at(2) > this->at(idx)) ? 2 : idx;
        return ToAxes(idx);
    }
    Axes abs_max_axes() const{
        return this->_abs_max_axes(typename DimTagTraits_<Dim>::Type());
    }
    Axes _abs_max_axes(Dim2Tag) const{
        return (std::abs(this->at(0)) >= std::abs(this->at(1))) ? _X_ : _Y_;
    }
    Axes _abs_max_axes(Dim3Tag) const{
        auto idx = (std::abs(this->at(0)) >= std::abs(this->at(1))) ? 0 : 1;
        idx = (std::abs(this->at(2)) > std::abs(this->at(idx))) ? 2 : idx;
        return ToAxes(idx);
    }
    /**
     *  Distance to other point
     *
     *  The distance formula:
     *   \f$ d = \sqrt{(x - x_p)^2 + (y - y_p)^2 + (z - z_p)^2} \f$
     *
     *  \param p - other point
     *
     *  \return Vt - an distance from to this point
     */
    CV dist(const Point& p) const {
        double dx = x() - p.x();
        double dy = y() - p.y();
        return sqrt(dx * dx + dy * dy);
    }

    /**
     *  Normalize the point as a vector from (0,0,0) to this point.
     */
    void normalize() {
        Normalize(*this);
    }
    inline size_type size() const {
        return size_type(Dim);
    }
    inline pointer ptr() {
        return this->data();
    }
    inline const_pointer ptr() const {
        return this->data();
    }

    // operator
    Point& operator+=(const Point& p) {
        for (St i = 0; i < Dim; i++) {
            this->at(i) += p[i];
        }
        return *this;
    }
    Point& operator+=(const CV& val) {
        for (St i = 0; i < Dim; i++) {
            this->at(i) += val;
        }
        return *this;
    }
    Point& operator-=(const Point& p) {
        for (St i = 0; i < Dim; i++) {
            this->at(i) -= p[i];
        }
        return *this;
    }
    Point& operator-=(const CV& val) {
        for (St i = 0; i < Dim; i++) {
            this->at(i) -= val;
        }
        return *this;
    }
    Point& operator*=(const Point& p) {
        for (St i = 0; i < Dim; i++) {
            this->at(i) *= p[i];
        }
        return *this;
    }
    Point& operator*=(const CV& val) {
        for (St i = 0; i < Dim; i++) {
            this->at(i) *= val;
        }
        return *this;
    }
    Point& operator/=(const Point& p) {
        for (St i = 0; i < Dim; i++) {
            this->at(i) /= p[i];
        }
        return *this;
    }
    Point& operator/=(const CV& val) {
        for (St i = 0; i < Dim; i++) {
            this->at(i) /= val;
        }
        return *this;
    }

};
template<typename TYPE, St DIM>
inline Point_<TYPE, DIM> operator*(
        Point_<TYPE, DIM> lhs,
        const TYPE& rhs) {
    lhs *= rhs;
    return lhs;
}
template<typename TYPE, St DIM>
inline Point_<TYPE, DIM> operator+(
        Point_<TYPE, DIM> lhs,
        const Point_<TYPE, DIM>& rhs) {
    lhs += rhs;
    return lhs;
}

template<typename TYPE, St DIM>
inline Point_<TYPE, DIM> operator-(
        Point_<TYPE, DIM> lhs,
        const Point_<TYPE, DIM>& rhs) {
    lhs -= rhs;
    return lhs;
}

template<typename TYPE, St DIM>
inline Point_<TYPE, DIM> operator/(
        Point_<TYPE, DIM> lhs,
        const typename Point_<TYPE, DIM>::Vt& rhs) {
    lhs /= rhs;
    return lhs;
}


template<typename TYPE, St DIM>
std::ostream& operator<<(std::ostream& stream, const Point_<TYPE, DIM>& point) {
    stream << "(";
    for (St d = 0; d < DIM; ++d) {
        stream << point[d];
        if (d != DIM - 1) {
            stream << ", ";
        }
    }
    stream << ")";
    return stream;
}

template<typename TYPE, St DIM>
std::string ToString(const Point_<TYPE, DIM>& p){
    return p.to_string();
}
template<typename TYPE, St DIM>
std::string ToString(const Point_<TYPE, DIM>& p, const std::string& sep){
    return p.to_string(sep);
}

template <typename NUM>
inline NUM Distance2_PointToPoint2d(const NUM& x0, const NUM& y0,
                                    const NUM& x1, const NUM& y1){
    NUM dx = x0 - x1;
    NUM dy = y0 - y1;
    return dx * dx + dy * dy;
}

template <typename NUM>
inline NUM Distance2_PointToPoint3d(const NUM& x0, const NUM& y0, const NUM& z0,
                                    const NUM& x1, const NUM& y1, const NUM& z1){
    NUM dx = x0 - x1;
    NUM dy = y0 - y1;
    NUM dz = z0 - z1;
    return dx * dx + dy * dy + dz * dz;
}

template <typename NUM>
inline NUM Dot(const NUM& x0, const NUM& y0,
               const NUM& x1, const NUM& y1){
    return x0 * x1 + y0 * y1;
}

template<typename TYPE, St DIM>
double Distance2(const Point_<TYPE, DIM>& p1,
                 const Point_<TYPE, DIM>& p2) {
    if (DIM == 1) {
        double dis = p1[0] - p2[0];
        return std::abs(dis);
    }
    if (DIM == 2) {
        return double(
                Distance2_PointToPoint2d(p1.x(), p1.y(),
                                         p2.x(), p2.y()));
    }
    if (DIM == 3) {
        return double(
                Distance2_PointToPoint3d(p1.x(), p1.y(), p1.z(),
                                         p2.x(), p2.y(), p2.z()));
    }
    SHOULD_NOT_REACH;
    return 0.0;
}
template<typename TYPE, St DIM>
double Distance(const Point_<TYPE, DIM>& p1,
                const Point_<TYPE, DIM>& p2) {
    return std::sqrt(Distance2(p1, p2));
}
template<typename TYPE, St DIM>
bool IsSame(const Point_<TYPE, DIM>& p1,
            const Point_<TYPE, DIM>& p2,
            double tol = 1e-10) {
    return Distance(p1, p2) < tol ? true : false;
}
template<typename TYPE, St DIM>
Point_<TYPE, DIM> Mid(
                 const Point_<TYPE, DIM>& p1,
                 const Point_<TYPE, DIM>& p2) {
    Point_<TYPE, DIM> res;
    for (St i = 0; i < DIM; i++) {
        res[i] = 0.5 * (p1[i] + p2[i]);
    }
    return res;
}

/**
 * Get minimun loaction of
 *
 * for example:
 * a   = ( 1, 2, 3)
 *            ^
 * b   = ( 0, 4, 2)
 *         ^     ^
 * res = Min(a, b);
 * res = ( 0, 2, 2)
 */
template<typename TYPE, St DIM>
Point_<TYPE, DIM> Min(
        const Point_<TYPE, DIM>& a,
        const Point_<TYPE, DIM>& b) {
    Point_<TYPE, DIM> res;
    for (St i = 0; i < DIM; i++) {
        res[i] = std::min(a[i], b[i]);
    }
    return res;
}
/**
 * Get max location of
 */
template<typename TYPE, St DIM>
Point_<TYPE, DIM> Max(
        const Point_<TYPE, DIM>& a,
        const Point_<TYPE, DIM>& b) {
    Point_<TYPE, DIM> res;
    for (St i = 0; i < DIM; i++) {
        res[i] = std::max(a[i], b[i]);
    }
    return res;
}

template<typename TYPE, St DIM>
Point_<TYPE, DIM> BetweenByRatio(
        const Point_<TYPE, DIM>& start,
        const Point_<TYPE, DIM>& end,
        const Vt& ratio) {
    // -1 <= ratio <= 1
    if (ratio == 1) {
        return end;
    }
    if (ratio == -1) {
        return start;
    }
    Point_<TYPE, DIM> res;
    if (ratio < 0) {
        for (St i = 0; i < DIM; ++i) {
            res[i] = end[i] + (end[i] - start[i]) * ratio;
        }
    } else if (ratio > 0) {
        for (St i = 0; i < DIM; ++i) {
            res[i] = start[i] + (end[i] - start[i]) * ratio;
        }
    }
    return res;
}
template<typename TYPE, St DIM>
Point_<TYPE, DIM> NewPointFromStart(
        const Point_<TYPE, DIM>& start,
        const Point_<TYPE, DIM>& end,
        const Vt& distance) {
    auto dis = Distance(start, end);
    auto ratio = distance / dis;
    // -1 <= ratio <= 1
    if (distance == 1) {
        return end;
    }
    if (distance == 0) {
        return start;
    }
    Point_<TYPE, DIM> res;
    for (St i = 0; i < DIM; ++i) {
        res[i] = start[i] + (end[i] - start[i]) * ratio;
    }
    return res;
}
template<typename TYPE, St DIM>
Point_<TYPE, DIM> NewPointFromEnd(
        const Point_<TYPE, DIM>& start,
        const Point_<TYPE, DIM>& end,
        const Vt& distance) {
    auto dis = Distance(start, end);
    auto ratio = distance / dis;
    // -1 <= ratio <= 1
    if (distance == 1) {
        return end;
    }
    if (distance == 0) {
        return start;
    }
    Point_<TYPE, DIM> res;
    for (St i = 0; i < DIM; ++i) {
        res[i] = end[i] - (end[i] - start[i]) * ratio;
    }
    return res;
}
//===============================================
// Cross product (v1 - v4) . ((v2-v4) x (v3-v4))
// for Point
// @param    p1 Point1
// @param    p2 Point1
// @return      the result of cross multiply
//              The scalar triple product
//              (also called the mixed product, box product)
//-----------------------------------------------
template<typename TYPE, St DIM>
double Orient(
        const Point_<TYPE, DIM>&v1,
        const Point_<TYPE, DIM>&v2,
        const Point_<TYPE, DIM>&v3,
        const Point_<TYPE, DIM>&v4 = Point_<TYPE, DIM>()) {
    if (DIM == 2) {
        /// d1    = v1 - v3
        /// d2    = v2 - v3
        /// cross = d1x * d2y - d1y * d2x
        return exact::orient2d(v1.data(), v2.data(), v3.data());
    }

    if (DIM == 3) {
        return exact::orient3d(v1.data(), v2.data(), v3.data(), v4.data());
    }
    SHOULD_NOT_REACH;
    return 0.0;
}
template<typename TYPE>
Point_<TYPE, 3> Cross(const Point_<TYPE, 3>& a, const Point_<TYPE, 3>& b){
    auto _x = a.y() * b.z() - a.z() * b.y();
    auto _y = a.z() * b.x() - a.x() * b.z();
    auto _z = a.x() * b.y() - a.y() * b.x();

    return {_x, _y, _z};
}
// 1D
template<typename TYPE>
Trinary OnWhichSide3(
        const TYPE& p,
        const TYPE& v) {
    if (v > p) {
        return _POSITIVE_;
    } else if (v < p) {
        return _NEGATIVE_;
    } else {
        return _ZERO_;
    }
}

// 2D
template<typename TYPE>
Trinary OnWhichSide3(
        const Point_<TYPE, 2>& p0,
        const Point_<TYPE, 2>& p1,
        const Point_<TYPE, 2>& p2) {
    double tmp = Orient(p0, p1, p2);
    if (tmp > 0) {
        return _POSITIVE_;
    } else if (tmp < 0) {
        return _NEGATIVE_;
    } else {
        return _ZERO_;
    }
}
// 3D
template<typename TYPE>
Trinary OnWhichSide3(
        const Point_<TYPE, 3>& p0,
        const Point_<TYPE, 3>& p1,
        const Point_<TYPE, 3>& p2,
        const Point_<TYPE, 3>& p3) {
    double tmp = Orient(p0, p1, p2, p3);
    if (tmp > 0) {
        return _POSITIVE_;
    } else if (tmp < 0) {
        return _NEGATIVE_;
    } else {
        return _ZERO_;
    }
}

// template<class POINT, class TAG>
// class PointView{};

// template<class POINT>
// class PointView<POINT, PointViewOnYZTag>{
// public:
//     typedef PointViewOnYZTag Tag;
//     typedef POINT            Point;
// protected:
//     const Point* _p;
// public:
    
// };


} //end namespace

#endif /* POINT_H_ */
