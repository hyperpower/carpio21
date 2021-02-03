#ifndef _BBNODE_HPP_
#define _BBNODE_HPP_

#include "geometry/geometry_define.hpp"
#include <functional>

namespace carpio {

// Box must be one of AABBox, OBBox or BCircle

// AABBox  : Axes Align Bounding Box
// OBBox   : Orient Bounding Box
// BCircle : Bounding Circle

template<typename BOX>
class BBNode_ {
protected:
	typedef BBNode_<BOX> _Self;
	typedef BBNode_<BOX>* pNode;
public:
	BOX _box;

	pNode lchild;
	pNode rchild;
	pNode father;

	BBNode_() {
		_box = BOX();
		lchild = nullptr;
		rchild = nullptr;
		father = nullptr;
	}

	BBNode_(const BOX &data) {
		_box = BOX(data);
		lchild = nullptr;
		rchild = nullptr;
		father = nullptr;
	}
	BBNode_(const BOX &data, pNode lc, pNode rc, pNode fc) {
		_box = BOX(data);
		lchild = lc;
		rchild = rc;
		father = fc;
	}
	pNode leftmost() {
		pNode ptn = this;
		while (ptn->lchild != nullptr)
			ptn = ptn->lchild;
		return ptn;
	}
	pNode rightmost() {
		pNode ptn = this;
		while (ptn->rchild != nullptr)
			ptn = ptn->rchild;
		return ptn;
	}

	St height() const {
		return 0 + std::max(_height(this->lchild), _height(this->rchild));
	}

	St level() const {
		// root is root->lchild
		return 0 + _level(father) - 1;
	}

	bool is_leaf() const {
		_return_val_if_fail(this->lchild == nullptr, false);
		_return_val_if_fail(this->rchild == nullptr, false);
		return true;
	}
	BOX& box(){
		return this->_box;
	}
	const BOX& box() const{
		return this->_box;
	}
protected:
	St _height(const BBNode_<BOX>* cur) const {
		if (cur == nullptr)
			return 0;
		else
			return 1 + std::max(_height(cur->lchild), _height(cur->rchild));
	}
	St _level(const BBNode_<BOX>* cur) const {
		if (cur == nullptr)
			return 0;
		else
			return 1 + _level(cur->father);
	}
};

}
#endif
