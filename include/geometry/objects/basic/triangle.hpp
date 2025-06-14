#ifndef _TRIANGLE_HPP_
#define _TRIANGLE_HPP_

#include "geometry/geometry_define.hpp"
#include "point.hpp"
#include <array>
#include "math.h"

#undef max
#undef min

namespace carpio {

struct TriangleTag: public GeometryTag {};

template<typename TYPE, St DIM, class POINT = Point_<TYPE, DIM> >
class Triangle_: public std::array<POINT, 3> {
public:
    static const St Dim = DIM;
    typedef TriangleTag Tag;
    typedef TYPE ValueType;
    typedef TYPE vt;
    typedef TYPE& ref_vt;
    typedef const TYPE& const_ref_vt;
    typedef Triangle_<TYPE, DIM> Self;
    typedef Triangle_<TYPE, DIM>& ref_Self;
    typedef const Triangle_<TYPE, DIM>& const_ref_Self;
    typedef POINT Point;
    typedef POINT* pPoint;
    typedef POINT& ref_Point;
    typedef const POINT& const_ref_Point;

public:
    Triangle_() :
            std::array<Point_<TYPE, DIM>, 3>() {
        _set_empty();
    }
    Triangle_(const Point& a, const Point& b, const Point& c) {
        ASSERT(a != b);
        this->pa() = a;
        this->pb() = b;
        this->pc() = c;
    }
    Triangle_(const vt& ax,     const vt& bx, //
              const vt& ay,     const vt& by,  //
              const vt& az = 0, const vt& bz = 0) {
        Point s(ax, ay, az);
        Point e(bx, by, bz);
        reconstruct(s, e);
    }
    Triangle_(const_ref_Self rhs) {
        this->pa() = rhs.pa();
        this->pb() = rhs.pb();
        this->pc() = rhs.pc();
    }
    void reconstruct(const_ref_Self rhs) {
        this->pa() = rhs.pa();
        this->pb() = rhs.pb();
        this->pc() = rhs.pc();
    }
    void reconstruct(const Point& s, const Point& e) {
        ASSERT(s != e);
        this->pa() = s;
        this->pb() = e;
    }
    void reconstruct(const vt& ax, const vt& bx, //
            const vt& ay, const vt& by,  //
            const vt& az = 0, const vt& bz = 0) {
        Point s(ax, ay, az);
        Point e(bx, by, bz);
        reconstruct(s, e);
    }

    ref_Point p(const St& idx) {
        return this->at(idx);
    }
    const_ref_Point p(const St& idx) const {
        return this->at(idx);
    }

    ref_Point pa() {
        return this->at(0);
    }
    const_ref_Point pa() const {
        return this->at(0);
    }
    ref_Point pb() {
        return this->at(1);
    }
    const_ref_Point pb() const {
        return this->at(1);
    }
    ref_Point pc() {
        return this->at(2);
    }
    const_ref_Point pc() const {
        return this->at(2);
    }
    Point center() const {
        Point res;
        for (int i = 0; i <Dim; ++i){
            res[i] = ((*this)[0][i] + (*this)[1][i] + (*this)[2][i]) / 3.0;
        }
        return res;
    }

    const_ref_vt p(const St& idx, const Axes& a) const {
        ASSERT(idx < 3 && idx >= 0);
        return this->at(idx).val(a);
    }

    const_ref_vt pax() const {
        return this->pa().x();
    }
    const_ref_vt pbx() const {
        return this->pb().x();
    }
    const_ref_vt pcx() const {
        return this->pc().x();
    }
    const_ref_vt pay() const {
        ASSERT(Dim >= 2);
        return this->pa().y();
    }
    const_ref_vt pby() const {
        ASSERT(Dim >= 2);
        return this->pb().y();
    }
    const_ref_vt pcy() const {
        ASSERT(Dim >= 2);
        return this->pc().y();
    }
    const_ref_vt paz() const {
        ASSERT(Dim >= 3);
        return this->pa().z();
    }
    const_ref_vt pbz() const {
        ASSERT(Dim >= 3);
        return this->pb().z();
    }
    const_ref_vt pcz() const {
        ASSERT(Dim >= 3);
        return this->pc().z();
    }
    ref_vt pax() {
        return this->pa().x();
    }
    ref_vt pbx() {
        return this->pb().x();
    }
    ref_vt pay() {
        ASSERT(Dim >= 2);
        return this->pa().y();
    }
    ref_vt pby() {
        ASSERT(Dim >= 2);
        return this->pb().y();
    }
    ref_vt paz() {
        ASSERT(Dim >= 3);
        return this->pa().z();
    }
    ref_vt pbz() {
        ASSERT(Dim >= 3);
        return this->pb().z();
    }
    vt max(const Axes& a) const {
        vt m = this->p(0, a);
        for (St i = 1; i < 3; i++) {
            if (this->p(i, a) > m) {
                m = this->p(i, a);
            }
        }
        return m;
    }

    vt min(const Axes& a) const {
        vt m = this->p(0, a);
        for (St i = 1; i < 3; i++) {
            if (this->p(i, a) < m) {
                m = this->p(i, a);
            }
        }
        return m;
    }

    vt d(const Axes& a) const {
        vt max = this->max(a);
        vt min = this->min(a);
        return max - min;
    }

    vt dx() const {
        return this->d(_X_);
    }
    vt dy() const {
        return this->d(_Y_);
    }
    vt dz() const {
        return this->d(_Z_);
    }
    vt length(St idx) const {
        St ia[] = { 0, 1, 2 };
        St ib[] = { 1, 2, 0 };
        const Point& a = this->p(ia[idx]);
        const Point& b = this->p(ib[idx]);
        return Distance(a, b);
    }
    bool is_valid() const {
        vt a = this->length(0);
        vt b = this->length(1);
        vt c = this->length(2);
        if ((a > 0) && (b > 0) && (c > 0)) {
            return ((a + b) > c) || ((b + c) > a) || ((c + a) > b);
        } else {
            return false;
        }
    }
//	vt slope() const {
//		//ASSERT(Dim == 2);
//		//return (pby() - pay()) / (pbx() - pax() + SMALL);
//	}

    void scale(vt xfactor, vt yfactor, vt zfactor = 1) {
        auto pc = this->center();
        this->scale(pc, xfactor, yfactor, zfactor); 
    }

    void scale(const Point& pabout, 
                     vt xfactor, vt yfactor, vt zfactor = 1){
        std::vector<vt> fvec = {{xfactor, yfactor, zfactor}};
        for(auto& p : (*this)){
            for(int i=0; i < Dim; ++i){
                p[i] = fvec[i] * (p[i] - pabout[i]) + pabout[i];
            }
        }
    }
    void transfer(vt dx, vt dy, vt dz = 0.0) {
        if (!empty()) {
            pax() = pax() + dx;
            pay() = pay() + dy;
            pbx() = pbx() + dx;
            pby() = pby() + dy;
        }
        if (Dim == 3) {
            paz() = paz() + dz;
            pbz() = pbz() + dz;
        }
    }

    bool empty() const {
        if (pax() == 0.0 && pay() == 0.0 && pbx() == 0.0 && pby() == 0.0
                && ((Dim == 3) ? (paz() == 0.0 && pbz() == 0.0) : true)) {
            return true;
        } else {
            return false;
        }
    }
    void show() const {
        std::cout.precision(4);
        std::cout << "( " << pax() << ", " << pay();
        if (Dim == 3) {
            std::cout << ", " << paz();
        } else {
            std::cout << "";
        }
        std::cout << " )--->( " << this->pbx() << ", " << pby();
        if (Dim == 3) {
            std::cout << ", " << pbz();
        } else {
            std::cout << "";
        }
        std::cout << " )--->( " << this->pcx() << ", " << pcy();
        if (Dim == 3) {
            std::cout << ", " << pcz();
        } else {
            std::cout << "";
        }
        std::cout << " )\n";
    }

    /*
     *  compare
     */
    bool is_gt(const vt& v, const Axes& a) const {
        vt max = this->max(a);
        return max > v;
    }
    bool is_ge(const vt& v, const Axes& a) const {
        vt max = this->max(a);
        return max >= v;
    }
    bool is_lt(const vt& v, const Axes& a) const {
        vt max = this->max(a);
        return max < v;
    }
    bool is_le(const vt& v, const Axes& a) const {
        vt max = this->max(a);
        return max <= v;
    }

    bool is_normal(const Axes& a) const {
        return (this->p(0, a) == this->p(1, a) && this->p(1, a) == this->p(2, a));
    }

//	bool is_in_box(const Point_<TYPE, DIM> &pt) const {
//		ASSERT(false);
//	}
    bool is_equal(const Self& tri) const{
        return (this->p(0) == tri.p(0) && this->p(1) == tri.p(1)
                && this->p(2) == tri.p(2));
    }
    bool operator==(const Self& tri) const{
        return this->is_equal(tri);
    }

protected:
    void _set_empty() {
        pa().x() = 0.0;
        pb().x() = 0.0;
        pc().x() = 0.0;
        pa().y() = 0.0;
        pb().y() = 0.0;
        pc().y() = 0.0;
        if (Dim == 3) {
            pa().z() = 0.0;
            pb().z() = 0.0;
            pc().z() = 0.0;
        }
    }
};

}
#endif
