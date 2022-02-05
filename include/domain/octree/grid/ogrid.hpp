#ifndef _OGRID_H_
#define _OGRID_H_

#include "domain/octree/octree_define.hpp"
#include "onode.hpp"
#include "ocell.hpp"


namespace carpio {

template<typename DATA, St DIM>
class Grid_ {
public:
	static const St Dim = DIM;
	static const St NumFaces = DIM + DIM;
	static const St NumVertexes = (DIM == 3) ? 8 : (DIM + DIM);
	static const St NumNeighbors = NumFaces;



};// <- End Grid_
}// <- End namespace carpio


#endif