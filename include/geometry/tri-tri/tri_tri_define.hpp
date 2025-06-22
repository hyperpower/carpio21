#ifndef TRI_TRI_DEFINE_HPP
#define TRI_TRI_DEFINE_HPP

#include <math.h>

#include "geometry/geometry_define.hpp"

namespace carpio
{
namespace raw
{
// Function to compute the cross product of two 3D vectors
template<typename CVT>
inline void Cross(CVT dest[3], const CVT v1[3], const CVT v2[3]) {
    dest[0] = v1[1] * v2[2] - v1[2] * v2[1];
    dest[1] = v1[2] * v2[0] - v1[0] * v2[2];
    dest[2] = v1[0] * v2[1] - v1[1] * v2[0];
}
template<typename CVT>
inline CVT Dot(const CVT v1[3], const CVT v2[3]) {
    return (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2]);
}
template<typename CVT>
inline void Sub(CVT dest[3], const CVT v1[3], const CVT v2[3]) {
    dest[0]=v1[0]-v2[0];
    dest[1]=v1[1]-v2[1]; 
    dest[2]=v1[2]-v2[2]; 
}
template<typename CVT>
inline void Add(CVT dest[3], const CVT v1[3], const CVT v2[3]) {
    dest[0]=v1[0]+v2[0];
    dest[1]=v1[1]+v2[1]; 
    dest[2]=v1[2]+v2[2]; 
}
template<typename CVT>
inline void MultiValue(CVT dest[3], const CVT v1[3], const CVT factor) {
    dest[0]=v1[0]*factor;
    dest[1]=v1[1]*factor; 
    dest[2]=v1[2]*factor; 
}

} // namespace tri_tri
    
} // namespace carpio


#endif // TRI_TRI_DEFINE_HPP