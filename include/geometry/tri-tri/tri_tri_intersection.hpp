#ifndef TRI_TRI_INTERSECTION_HPP
#define TRI_TRI_INTERSECTION_HPP

#include <string>

#include "tri_tri_define.hpp"
#include "moller97.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/triangle.hpp"

namespace carpio {

/**
 * @brief Checks if two triangles intersect using the specified method.
 * 
 * @tparam CVT The type of the coordinates (e.g., float, double).
 * @param V0 Vertex 0 of the first triangle.
 * @param V1 Vertex 1 of the first triangle.
 * @param V2 Vertex 2 of the first triangle.
 * @param U0 Vertex 0 of the second triangle.
 * @param U1 Vertex 1 of the second triangle.
 * @param U2 Vertex 2 of the second triangle.
 * @param method The intersection method to use (default is "moller97").
 * @return int Returns 1 if the triangles intersect, 0 otherwise.
 * @throws std::invalid_argument If the specified method is unknown.
 */
template<typename CVT>
int IsIntersect(const CVT* V0, const CVT* V1, const CVT* V2,
                const CVT* U0, const CVT* U1, const CVT* U2,
                const std::string& method = "moller97") {
    if (method == "moller97") {
        return tri_tri_intersect(V0, V1, V2, U0, U1, U2);
    } else {
        throw std::invalid_argument("Unknown intersection method: " + method);
    }
}
template<typename CVT>
int Intersect(CVT V0[3], CVT V1[3], CVT V2[3],
              CVT U0[3], CVT U1[3], CVT U2[3], 
              bool& coplanar,
              CVT isectpt1[3],CVT isectpt2[3],
              const std::string& method = "moller97") {
    if (method == "moller97" || method == "") {
        return tri_tri_intersect_with_isectline(
                V0, V1, V2, U0, U1, U2, coplanar, isectpt1, isectpt2);
    } else {
        throw std::invalid_argument("Unknown intersection method: " + method);
    }
}
template<typename POINT>
typename std::enable_if<std::is_same<typename POINT::Tag, PointTag>::value, int>::type
Intersect(const POINT& V0, const POINT& V1, const POINT& V2,
          const POINT& U0, const POINT& U1, const POINT& U2,
          bool& coplanar,
          POINT& isectpt1, POINT& isectpt2,
          const std::string& method = "moller97") {
    typedef typename POINT::value_type CVT;
    if (method == "moller97" || method == "") {
        return tri_tri_intersect_with_isectline(
                V0.data(), V1.data(), V2.data(), 
                U0.data(), U1.data(), U2.data(), 
                coplanar, 
                isectpt1.data(), isectpt2.data());
    } else {
        throw std::invalid_argument("Unknown intersection method: " + method);
    }
}
/**
 * @brief Checks if two triangles intersect using the specified method, 
 *        with vertices represented as Point objects.
 * 
 * @tparam Point The type of the point objects.
 * @param V0 Vertex 0 of the first triangle.
 * @param V1 Vertex 1 of the first triangle.
 * @param V2 Vertex 2 of the first triangle.
 * @param U0 Vertex 0 of the second triangle.
 * @param U1 Vertex 1 of the second triangle.
 * @param U2 Vertex 2 of the second triangle.
 * @param method The intersection method to use (default is "moller97").
 * @return int Returns 1 if the triangles intersect, 0 otherwise.
 */
template<typename POINT>
typename std::enable_if<std::is_same<typename POINT::Tag, PointTag>::value, int>::type
IsIntersect(const POINT& V0, const POINT& V1, const POINT& V2,
            const POINT& U0, const POINT& U1, const POINT& U2,
            const std::string& method = "moller97") {
    return IsIntersect(V0.data(), V1.data(), V2.data(),
                       U0.data(), U1.data(), U2.data(), method);
}

/**
 * @brief Checks if two triangles intersect using the specified method, 
 *        with triangles represented as Triangle objects.
 * 
 * @tparam TRI The type of the triangle objects.
 * @param V0 The first triangle.
 * @param V1 The second triangle.
 * @param method The intersection method to use (default is "moller97").
 * @return int Returns 1 if the triangles intersect, 0 otherwise.
 */
template<typename TRI>
typename std::enable_if<std::is_same<typename TRI::Tag, TriangleTag>::value, int>::type
IsIntersect(const TRI& V0, 
            const TRI& V1,
            const std::string& method = "moller97") {
    return IsIntersect(V0[0], V0[1], V0[2],
                       V1[0], V1[1], V1[2], method);
}

} // namespace carpio

#endif // TRI_TRI_INTERSECTION_HPP