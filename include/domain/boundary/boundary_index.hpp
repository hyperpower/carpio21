#ifndef _S_BOUNDARY_INDEX_HPP_
#define _S_BOUNDARY_INDEX_HPP_

#include "domain/domain_define.hpp"
#include "boundary_condition.hpp"
#include "bc_value.hpp"
#include "bc_fun_xyz.hpp"

#include <map>

namespace carpio{

class BoundaryIndex {

public:

	typedef BoundaryCondition BC;
	typedef std::shared_ptr<BoundaryCondition> spBC;
	typedef std::shared_ptr<const BoundaryCondition> spcBC;
	//typedef BoundaryCondition* pBoundaryCondition;
	//typedef const BoundaryCondition<cvt, vt>* const_pBoundaryCondition;

	typedef std::map<int, spBC> BCMap;
protected:
	BCMap _BCmap;
	spcBC _pdefault_BC;

	typedef typename BCMap::iterator iterator;
	typedef typename BCMap::const_iterator const_iterator;

public:
	//constructor
	BoundaryIndex() :
			_BCmap() {
		_pdefault_BC = spcBC(new BoundaryConditionValue());
	}
	~BoundaryIndex() {
	}
	//
	void insert(St si, spBC pbc) {
		//

		//key.seg_idx = segi;
		//key.shape_idx = si;
		//key.val_idx = vi;
		std::pair<int, spBC>bcn(si, pbc);
		_BCmap.insert(bcn);
	}

	spcBC find(St si) const {
		const_iterator it = _BCmap.find(si);
		if (it != _BCmap.end()) {
			// found
			return (it->second);
		} else {
			// not found
			return _pdefault_BC;
		}
	}

//	void copy(const std::string& vn1, const std::string& vn2) {
//		list_BCNode lbc;
//		for (const_iterator it = _BCmap.begin(); it != _BCmap.end(); ++it) {
//			const BCID& key = it->first;
//			if (key.val_name == vn1) {
//				BCID nkey = { key.shape_idx, key.seg_idx, vn2 };
//				spBC spbc = it->second;
//				BCNode bcn(nkey, spbc);
//				lbc.push_back(bcn);
//			}
//		}
//		for (auto term : lbc) {
//			this->_BCmap.insert(term);
//		}
//	}

	void show() const {
		std::cout << "BI: ---->\n";
		for (auto term : this->_BCmap) {
			std::cout << "idx " <<term.first;
			std::cout << " - type " << term.second->type() << "\n";
		}
		std::cout << "<--------\n";
	}

};



}

#endif
