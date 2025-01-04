#ifndef _BOOLEAN_BASE_HPP_
#define _BOOLEAN_BASE_HPP_

#include <list>
#include <memory>

#include "geometry/objects/objects.hpp"

namespace carpio{

template<class GEO1, class GEO2, class TAG1, class TAG2>
class IntersectionImplement_{};
template<class GEO1, class GEO2, class TAG1, class TAG2>
class IntersectionResultImplement_{};

template<class GEO1, class GEO2>
using Intersection_ = IntersectionImplement_<GEO1, GEO2, typename GEO1::Tag, typename GEO2::Tag>;

template<class GEO1, class GEO2>
using IntersectionResult_ = IntersectionResultImplement_<GEO1, GEO2, typename GEO1::Tag, typename GEO2::Tag>;

}

#endif