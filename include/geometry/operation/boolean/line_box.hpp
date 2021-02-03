#ifndef _BOOLEAN_LINE_BOX_HPP_
#define _BOOLEAN_LINE_BOX_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/basic/point_chain.hpp"

#include "algebra/algebra.hpp"

#include <memory>
#include <array>

namespace carpio {

// Line : ax + by = alpha
// Box  : Pmin Pmax

template<class NUM>
void _SortPointsAlongTheLine(
        std::list<std::shared_ptr<Point_<NUM, 2> > >& listp,
        const NUM& a,   const NUM& b){
    // Direction vector (b, -a)

    // Lambda for comparing spPoints
    auto fun = [&a, &b](const std::shared_ptr<Point_<NUM, 2> >& sp1,
                        const std::shared_ptr<Point_<NUM, 2> >& sp2){
        auto pv = *sp2 - *sp1;
        auto d  = Dot(b, -a, pv[0], pv[1]);
        return d > 0 ? true : false;
    };

    listp.sort(fun);
}
template<class NUM>
int _RemoveSamePoints(std::list<std::shared_ptr<Point_<NUM, 2> > >& listp,
                      NUM s){
    // list must be sorted !!!
    // Binary Predicate
    auto fun = [s](const std::shared_ptr<Point_<NUM, 2> >& sp1,
                        const std::shared_ptr<Point_<NUM, 2> >& sp2){
        return IsSame(*sp1, *sp2, s);
    };
    listp.unique(fun);
}

// Point min is (0, 0)
template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineBox(const NUM& xmax,const NUM& ymax,    // Point max
         const NUM& a,   const NUM& b, const NUM& alpha,
                    const double& tol = 1e-10){         // Line
    std::array<Axes, 4> ao = {_Y_  , _Y_,  _X_,  _X_};
    std::array<Vt, 4>   av = {0.0  , ymax, 0.0,  xmax};
    std::array<Vt, 4>   rv = {xmax , xmax, ymax, ymax};
    std::list<std::shared_ptr<Point_<NUM, 2> > > res;
    for(int i = 0; i < 4; i++){
        Vt cv = Calculate(a, b, alpha, ao[i], av[i]);
        if(IsInRange(0.0, cv, rv[i], _cc_)){
            std::shared_ptr<Point_<NUM, 2> > spp(new Point_<NUM, 2>);
            spp->x() = (ao[i] == _Y_) ? cv : av[i];
            spp->y() = (ao[i] == _X_) ? cv : av[i];
            res.push_back(spp);
        }
    }
    //_SortPointsAlongTheLine(res, a, b);
    //_RemoveSamePoints(res, tol);
    return res;
}


template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineBox(const NUM& xmin,const NUM& ymin,                 // Point min
                 const NUM& xmax,const NUM& ymax,                 // Point max
                 const NUM& a,   const NUM& b, const NUM& alpha){ // Line
    // transfer to (0,0)
    auto listspp = IntersectLineBox(xmax - xmin, ymax - ymin,
                                    a, b, alpha - (a * xmin) - (b * ymin));
    for(auto& sp : listspp){
        sp->transfer(xmin, ymin);
    }
    return listspp;
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLinePositiveBox(const NUM& xmin,const NUM& ymin,                 // Point min
                         const NUM& xmax,const NUM& ymax,                 // Point max
                         const NUM& a,   const NUM& b, const NUM& alpha){ // Line
    std::list<std::shared_ptr<Point_<NUM, 2> > > res;
    // 4 vertexes of box
    std::vector<NUM> arrx = {xmin, xmax, xmax, xmin};
    std::vector<NUM> arry = {ymin, ymin, ymax, ymax};
    for(St i = 0; i < 4; i++){
        auto x = arrx[i];
        auto y = arry[i];
        if(a* x + b * y > alpha){
            res.push_back(std::shared_ptr<Point_<NUM, 2> >(new Point_<NUM, 2>(x, y)));
        }
    }
    // get intersect points
    auto listinter = IntersectLineBox(xmin, ymin, xmax, ymax, a, b, alpha);
    res.insert(res.end(), listinter.begin(), listinter.end());
    // sort by left turn
    auto ori = *(res.front());
    auto fun = [&ori](const std::shared_ptr<Point_<NUM, 2> >& sp1,
                      const std::shared_ptr<Point_<NUM, 2> >& sp2){
         auto  rv  = Cross(*sp2, *sp1, ori);
         return rv > 0 ? true : false;
    };
    res.sort(fun);
    return res;
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineNegativeBox(const NUM& xmin,const NUM& ymin,                 // Point min
                         const NUM& xmax,const NUM& ymax,                 // Point max
                         const NUM& a,   const NUM& b, const NUM& alpha){ // Line
    std::list<std::shared_ptr<Point_<NUM, 2> > > res;
    // 4 vertexes of box
    std::vector<NUM> arrx = {xmin, xmax, xmax, xmin};
    std::vector<NUM> arry = {ymin, ymin, ymax, ymax};
    for(St i = 0; i < 4; i++){
        auto x = arrx[i];
        auto y = arry[i];
        if(a* x + b * y < alpha){
            res.push_back(std::shared_ptr<Point_<NUM, 2> >(new Point_<NUM, 2>(x, y)));
        }
    }
    // get intersect points
    auto listinter = IntersectLineBox(xmin, ymin, xmax, ymax, a, b, alpha);
    res.insert(res.end(), listinter.begin(), listinter.end());
    // sort by left turn
    auto ori = *(res.front());
    auto fun = [&ori](const std::shared_ptr<Point_<NUM, 2> >& sp1,
                      const std::shared_ptr<Point_<NUM, 2> >& sp2){
         auto  rv  = Cross(*sp2, *sp1, ori);
         return rv > 0 ? true : false;
    };
    res.sort(fun);
    return res;
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineBox(const Point_<NUM, 2>& pmax,          // Point max
                 const Line_<NUM>&     line){         // Line
    return IntersectLineBox(pmax.x(), pmax.y(), line.a(), line.b(), line.alpha());
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineBox(const Point_<NUM, 2>& pmin,          // Point min
                 const Point_<NUM, 2>& pmax,          // Point max
                 const Line_<NUM>&     line){         // Line
    return IntersectLineBox(pmin.x(), pmin.y(),
                            pmax.x(), pmax.y(),
                            line.a(), line.b(), line.alpha());
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLinePositiveBox(const Point_<NUM, 2>& pmin,          // Point min
                         const Point_<NUM, 2>& pmax,          // Point max
                         const Line_<NUM>&     line){         // Line
    return IntersectLinePositiveBox(pmin.x(), pmin.y(),
                                    pmax.x(), pmax.y(),
                                    line.a(), line.b(), line.alpha());
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineNegativeBox(const Point_<NUM, 2>& pmin,          // Point min
                         const Point_<NUM, 2>& pmax,          // Point max
                         const Line_<NUM>&     line){         // Line
    return IntersectLineNegativeBox(pmin.x(), pmin.y(),
                                    pmax.x(), pmax.y(),
                                    line.a(), line.b(), line.alpha());
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineBox(const Box_<NUM,2>&  box,                    // Box
                 const Line_<NUM>&   line){                  // Line
    return IntersectLineBox(box.min(), box.max(), line);
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLinePositiveBox(const Box_<NUM,2>&  box,            // Box
                         const Line_<NUM>&   line){         // Line
    return IntersectLinePositiveBox(box.min(), box.max(), line);
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineNegativeBox(const Box_<NUM,2>&  box,           // Box
                         const Line_<NUM>&   line){         // Line
    return IntersectLineNegativeBox(box.min(), box.max(), line);
}


}

#endif
