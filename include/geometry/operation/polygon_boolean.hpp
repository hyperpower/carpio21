#ifndef _POLYGON_BOOLEAN_HPP_
#define _POLYGON_BOOLEAN_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/basic/point_chain.hpp"
#include "geometry/operation/boolean/segment_segment.hpp"
#include "algebra/array/array_list.hpp"

#include "io/gnuplot.hpp"

#include <memory>
#include <set>
#include <functional>
#include <iomanip>
#include <algorithm>    // std::sort

namespace carpio {

template<class TYPE>
class PolygonBoolean_{
public:
	static const int _PN_  = 0;  // new point
	static const int _PC_  = 1;  // point from clip
	static const int _PO_  = 2;  // point from object
	static const int _PCO_ = 3;  // point from clip and object


	static const int _INTERSECTION_ = 10;
	static const int _UNION_        = 11;
	static const int _SUBSTRACT_    = 12;

	static const int _REFLECT_in_ = 1011;
	static const int _REFLECT_out_ = 1033;
	static const int _OVERLAP_ = 1022;
	static const int _IN_           = 1031;
	static const int _OUT_          = 1013;
	static const int _HALF_IN_in_ = 1021;
	static const int _HALF_IN_out_ = 1032;
	static const int _HALF_OUT_in_ = 1012;
	static const int _HALF_OUT_out_ = 1023;

	typedef TYPE Vt;
	typedef Point_<TYPE, 2> Point;
	typedef Box_<TYPE, 2> Box;
	typedef PointChain_<TYPE, 2> PointChain;
	typedef PolygonBoolean_<TYPE> PB;

	PolygonBoolean_(
			const PointChain& clip,
			const PointChain& object) : cli(nullptr), obj(nullptr){
		phase_1(clip, object);
		if(is_box_inon(clip, object)){
			_isboxinon = true;
			phase_2();
			phase_3();
			phase_4();
		}else{
			_isboxinon = false;
		}
	}

	~PolygonBoolean_(){
		_delete_nodes();
	}

	std::vector<PointChain> output(int op = _INTERSECTION_) {
		if(!_isboxinon){
			// clip and object are box separate
			return _output_box_separate(op);
		}else if(!_hasswitch){
			// trivial case has no switch node
			// object and clip are separate
			return _output_separate(op);
		}else{
			std::list<pNode> lin;
			pNode oin = obj;
			do {
				oin = _find_object_in(oin);
				if (oin != nullptr) {
					lin.push_back(oin);
					oin = oin->nexto;
				}
			} while (oin != nullptr);

			if(lin.size() > 0){
				return _output_normal(lin, op);
			} else {
				// find Half IN in
				std::list<pNode> lin;
				pNode oin = obj;
				bool fii = false;
				do {
					oin = _find_object_half_in(oin);
					if (oin != nullptr) {
						if(oin->type == _HALF_IN_in_){
							fii = true;
						}
						lin.push_back(oin);
						oin = oin->nexto;
					}
				} while (oin != nullptr);

				if(lin.size() > 0){
					if(fii == true){
						return _output_no_in(lin, op);
					}else{
						// clip is inside object
						std::cout << "no half IN in\n";
						PointChain pc;
						for_each_edge_clip([&pc](pNode ps, pNode pe) {
							pc.push_back(ps->point);
						});
						PointChain po;
						for_each_edge_object([&po](pNode ps, pNode pe) {
							po.push_back(ps->point);
						});
						// clip in object
						bool cino = true;
						for (auto& p : pc) {
							if (!IsInOn(p, po)) {
								cino = false;
								break;
							}
						}
						if(cino == true){
							return _output_no_in(lin, op);
						}else{
							// clip out side object
							// has no HALF IN in
							return _output_box_separate(op);
						}
					}
				}else{
					std::cout << "no half IN\n";
					return std::vector<PointChain>();
				}
			}
		}
	}

protected:
	std::vector<PointChain> _output_box_separate(int op = _INTERSECTION_) {
		std::vector<PointChain> vpc;
		// clip and object are box seperate
		switch (op) {
		case _INTERSECTION_:
			return vpc; //empty
		case _UNION_: {
			PointChain pc;
			for_each_edge_clip([&pc](pNode ps, pNode pe) {
				pc.push_back(ps->point);
			});
			PointChain po;
			for_each_edge_object([&po](pNode ps, pNode pe) {
				po.push_back(ps->point);
			});
			vpc.push_back(pc);
			vpc.push_back(po);
			return vpc;
		};
		case _SUBSTRACT_: {
			PointChain pc;
			for_each_edge_clip([&pc](pNode ps, pNode pe) {
				pc.push_back(ps->point);
			});
			vpc.push_back(pc);
			return vpc;
		};
		}
		return vpc;
	}

	std::vector<PointChain> _output_separate(int op = _INTERSECTION_) {
		std::vector<PointChain> res;
		PointChain pc;
		for_each_edge_clip([&pc](pNode ps, pNode pe) {
			pc.push_back(ps->point);
		});
		PointChain po;
		for_each_edge_object([&po](pNode ps, pNode pe) {
			po.push_back(ps->point);
		});
		// object in clip
		int num_case = 0;
		for(auto& p : po){
			if(IsInOn(p, pc)){
				num_case = 1;
				break;
			}
		}
		// clip in object
		for (auto& p : pc) {
			if (IsInOn(p, po)) {
				num_case = 2;
				break;
			}
		}
		// really separate
		num_case = (num_case == 0 )? 3: num_case;
		if (num_case == 1){
			switch (op) {
			case _INTERSECTION_:{
				res.push_back(po);
				return res;
			};
			case _UNION_: {
				res.push_back(pc);
				return res;
			};
			case _SUBSTRACT_: {
				pc.reverse();
				res.push_back(pc);
				res.push_back(po);
				return res;
			};
			}
		}else if(num_case == 2){
			switch (op) {
			case _INTERSECTION_:{
				res.push_back(pc);
				return res;
			};
			case _UNION_: {
				res.push_back(po);
				return res;
			};
			case _SUBSTRACT_: {
				return res;
			};
			}
		}else{ // 3
			switch (op) {
			case _INTERSECTION_:{
				return res;
			};
			case _UNION_: {
				res.push_back(po);
				res.push_back(pc);
				return res;
			};
			case _SUBSTRACT_: {
				return res;
			};
			}
		}
		return res;
	}



	struct Node {
		int   type;
		Point point;

		typedef Node* pNode;

		pNode prevc; // previous clip
		pNode nextc; // next clip
		pNode prevo; // previous object
		pNode nexto; // previous object

		Node(const Point& p, int t) :
				point(p) {
			type = t;

			prevc = nullptr;
			nextc = nullptr;
			prevo = nullptr;
			nexto = nullptr;
		}
	};

	typedef Node* pNode;

	typedef std::function<void(pNode)> FunN;
	typedef std::function<void(pNode, pNode)> FunNN;
	typedef std::function<void(pNode&, pNode&, pNode&, pNode&)> FunNNNN;

	struct Probe {
		typedef PolygonBoolean_<Vt>* pPB;
		pPB   ppb;
		int   op;  // operation type
		pNode bgn;
		pNode cur;
		int   sflag;
		int   ooc; // object or clip
		int   dir; // direction  1 next
				 //           -1 prev

		Probe(pPB _ppb, pNode in, int t) :
				ppb(_ppb), op(t), bgn(in) {
			cur = bgn;
			if (op == _INTERSECTION_) {
				sflag = 0;
				ooc = 1; //on object = 1
						 //on clip   = 0
				dir = 1;
			} else if (op == _UNION_) {
				sflag = 0;
				ooc = 0;
				dir = 1;
			} else if (op == _SUBSTRACT_) {
				sflag = 0;
				ooc = 0;
				dir = 1;
			}
		}

		pNode walk() {
			if (cur->type == _HALF_IN_in_ ||
			    cur->type == _HALF_IN_out_) {
				sflag += 1;
			} else if (cur->type == _HALF_OUT_in_
					|| cur->type == _HALF_OUT_out_) {
				sflag += -1;
			}
			if (op == _INTERSECTION_) {
				if (cur->type == _IN_ || sflag == 2) {
					ooc = 1;
					dir = 1;
					sflag = 0;
				} else if (cur->type == _OUT_ || sflag == -2) {
					ooc = 0;
					dir = 1;
					sflag = 0;
				}
			} else if (op == _UNION_) {
				if (cur->type == _IN_ || sflag == 2) {
					ooc = 0;
					dir = 1;
					sflag = 0;
				} else if (cur->type == _OUT_ || sflag == -2) {
					ooc = 1;
					dir = 1;
					sflag = 0;
				}
			} else if (op == _SUBSTRACT_) {
				if (cur->type == _IN_ || sflag == 2) {
					ooc = 0;
					dir = 1;
					sflag = 0;
				} else if (cur->type == _OUT_ || sflag == -2) {
					ooc = 1;
					dir = -1;
					sflag = 0;
				}
			}
			//
			if (dir == 1) {
				cur = ppb->_next_loop(cur, ooc);
			} else {
				cur = ppb->_prev_loop(cur, ooc);
			}
			return cur;
		}

		pNode walk2(){
			// no IN
			// HALF IN is IN
			// HALF OUT is OUT
			if (op == _INTERSECTION_) {
				if (cur->type == _HALF_IN_in_ ||
					cur->type == _HALF_IN_out_) {
					ooc = 1;
					dir = 1;
				} else if (
					cur->type == _HALF_OUT_in_ ||
					cur->type == _HALF_OUT_out_ ) {
					ooc = 0;
					dir = 1;
				}
			} else if (op == _UNION_) {
				if (cur->type == _HALF_IN_in_ ||
					cur->type == _HALF_IN_out_) {
					ooc = 0;
					dir = 1;
				} else if (
					cur->type == _HALF_OUT_in_ ||
					cur->type == _HALF_OUT_out_ ) {
					ooc = 1;
					dir = 1;
				}
			} else if (op == _SUBSTRACT_) {
				if (cur->type == _HALF_IN_in_ ||
					cur->type == _HALF_IN_out_) {
					ooc = 0;
					dir = 1;
				} else if (
					cur->type == _HALF_OUT_in_ ||
					cur->type == _HALF_OUT_out_ ) {
					ooc = 1;
					dir = -1;
				}
			}
			//
			if (dir == 1) {
				cur = ppb->_next_loop(cur, ooc);
			} else {
				cur = ppb->_prev_loop(cur, ooc);
			}
			return cur;
		}
	};

	std::vector<PointChain> _output_normal(
			std::list<pNode>& lin, int op = _INTERSECTION_) {
		std::vector<PointChain> vpc;
		std::set<pNode> used;
		for (auto& in : lin) {
			if (used.find(in) == used.end()) { // IN has NOT been used
				Probe pb(this, in, op);
				std::list<pNode> lpn;
				do {
					used.insert(pb.cur);
					lpn.push_back(pb.cur);
					pb.walk();
				} while (pb.cur != pb.bgn);

				_delete_overlap_edge(lpn);
				PointChain pc;
				for (auto& pn : lpn) {
					pc.push_back(pn->point);
				}
				vpc.push_back(pc);
			}
		}
		return vpc;
	}

	std::vector<PointChain> _output_no_in(
			std::list<pNode>& lin,
			int op = _INTERSECTION_) {
		std::vector<PointChain> vpc;
		std::set<pNode> used;
		for (auto& in : lin) {
			if (used.find(in) == used.end()) { // IN has NOT been used
				Probe pb(this, in, op);
				std::list<pNode> lpn;
				do {
					used.insert(pb.cur);
					lpn.push_back(pb.cur);
					pb.walk2();
				} while (pb.cur != pb.bgn);

				_delete_overlap_edge(lpn);
				PointChain pc;
				for (auto& pn : lpn) {
					pc.push_back(pn->point);
				}
				vpc.push_back(pc);
			}
		}
		return vpc;
	}

	struct IntersectionGroup {
			pNode cs;     // clip start
			pNode ce;     // clip end
			pNode os;     // object start
			pNode oe;     // object end
			pNode inter;  // intersection
			int type;   // type -1 error
						//       0 two  start point connect
						//       1 clip start point touch
						//       2 object start point touch
						//       3 normal intersection

			IntersectionGroup() :
					cs(nullptr), ce(nullptr), os(nullptr), oe(nullptr), inter(
							nullptr), type(-1) {
			}

			IntersectionGroup(const IntersectionGroup& other) :
					cs(other.cs), ce(other.ce), os(other.os), oe(other.oe), inter(
							other.inter), type(other.type) {
			}

			void set(pNode& pncs, pNode& pnce, pNode& pnos, pNode& pnoe,
					pNode& pinter, int t) {
				cs = pncs;
				ce = pnce;
				os = pnos;
				oe = pnoe;
				inter = pinter;
				type = t;
			}
		};

		typedef std::vector<IntersectionGroup> IntersectionTable;

		pNode cli;
		pNode obj;

		IntersectionTable _table;

		bool _isboxinon;
		bool _hasswitch;

		bool is_box_inon(const PointChain& clip,
				const PointChain& object) {
			Box cbox = clip.box();
			Box obox = object.box();
			return IsInOn(cbox, obox);
		}

		void phase_1(const PointChain& clip,
				const PointChain& object) {
			// copy point chain to node
			ASSERT(clip.size() >=3);
			ASSERT(object.size() >=3);
			_build_clip_link(clip);
			_build_object_link(object);
		}

		void phase_2() {
			// build intersection table
			FunNNNN fun = [this](pNode& pncs, pNode& pnce, pNode& pnos, pNode& pnoe) {
				IntersectionPairSS_<TYPE, 2> ssp(pncs->point,
						pnce->point,
						pnos->point,
						pnoe->point);
				IntersectionTypeSS itss = ssp.cal_intersection_type();
				IntersectionGroup row;
				if( itss != _SS_NO_ &&
						itss != _SS_INTERSECT_ ) {
					// two start point connect
					if(_PS_ON_START_ == ssp.point_position(0)) {
						row.set(pncs, pnce, pnos, pnoe, pncs, 0);
					}
				}
				if( itss == _SS_TOUCH_ ||
						itss == _SS_OVERLAP_) {
					if(_PS_IN_ == ssp.point_position(0)) {
						row.set(pncs, pnce, pnos, pnoe, pncs, 1);
					} else if(_PS_IN_ == ssp.point_position(2)) {
						row.set(pncs, pnce, pnos, pnoe, pnos, 2);
					}
				} else if(itss == _SS_INTERSECT_) {
					Point iterp = ssp.cal_intersection_point();
					pNode pnn = new Node(iterp, _PN_);
					row.set(pncs, pnce, pnos, pnoe, pnn, 3);
				}
				if(row.type != -1) {
					this->_table.push_back(row);
				}
			};

			for_each_for_each(fun);
		}

		void phase_3() {
			// combine two list
			// 1 combine two start point connect
			for (auto i = 0; i < this->_table.size(); i++) {
				auto& row = this->_table[i];
				if (row.type == 0) {
					this->_merge(row.cs, row.os);
					// change all os to cs in table
					for(auto ii = 0; ii < this->_table.size(); ii++) {
						auto& r = this->_table[ii];
						if ( i != ii
								&& r.os == row.os) {
							r.os = row.cs;
						}
					}
				}
			}
			for (auto it = this->_table.begin();
					it != this->_table.end();) {
				if (it->type == 0) {
					it = this->_table.erase(it);    //
				} else {
					++it;    //
				}
			}
			// 2 merge to clip
			this->_merge_to_clip();
			// 3 merge to object
			this->_merge_to_object();
		}

		void phase_4() {
			// calculate the switch node
			this->_hasswitch = false;
			FunN fun = [this](pNode pn) {
				if(this->_is_switch(pn)) {
					this->_hasswitch = true;
					pn->type = 1000 + this->_cal_switch_type(pn);
				}
			};
			for_each_node(fun);
		}

		pNode _next_loop(pNode cur, int _ooc) const {
			if(cur == nullptr) {
				return nullptr;
			}
			if(_ooc == 0) {  // walk on clip
				if(cur->nextc == nullptr) {
					return cli;
				} else {
					return cur->nextc;
				}
			} else {        // walk on object
				if(cur->nexto == nullptr) {
					return obj;
				} else {
					return cur->nexto;
				}
			}
		}

		pNode _prev_loop(pNode cur, int _ooc) const {
			if (cur == nullptr) {
				return nullptr;
			}
			if (_ooc == 0) {
				if (cur->prevc == nullptr) {
					return _last_c();
				} else {
					return cur->prevc;
				}
			} else {
				if (cur->prevo == nullptr) {
					return _last_o();
				} else {
					return cur->prevo;
				}
			}
		}

		// this function find the first IN node on object
		// It also considered the two HALF_IN case
		pNode _find_object_in(pNode start) {
			if(start == nullptr) {
				return nullptr;
			}
			int hinflag = 0;
			pNode cur = start;
			pNode bgn = nullptr;
			while (true) {
				if (hinflag == 0) {
					if (cur->type == _IN_) {
						bgn = cur;
						break;
					}
					if (cur->type == _HALF_IN_in_ ||
						cur->type == _HALF_IN_out_) {
						hinflag = 1;
					}
				} else { //hinflag = 1
					if (cur->type == _HALF_IN_in_ ||
						cur->type == _HALF_IN_out_) {
						bgn = cur;
						break;
					} else if (
							cur->type == _HALF_OUT_in_ ||
							cur->type == _HALF_OUT_out_) {
						hinflag = 0;
					}
				}
				if(cur->nexto !=nullptr) {
					cur = cur->nexto;
				} else {
					break;
				}
			}
			return bgn;
		}

	pNode _find_object_half_in(pNode start) {
		if (start == nullptr) {
			return nullptr;
		}
		int hinflag = 0;
		pNode cur = start;
		pNode bgn = nullptr;
		while (true) {
			if (cur->type == _HALF_IN_in_
			 || cur->type == _HALF_IN_out_
			 ) {
				bgn = cur;
				break;
			}
			if (cur->nexto != nullptr) {
				cur = cur->nexto;
			} else {
				break;
			}
		}
		return bgn;
	}

	pNode _find_object_reflect_in(pNode start) {
		if (start == nullptr) {
			return nullptr;
		}
		int hinflag = 0;
		pNode cur = start;
		pNode bgn = nullptr;
		while (true) {
			if (cur->type == _REFLECT_in_) {
				bgn = cur;
				break;
			}
			if (cur->nexto != nullptr) {
				cur = cur->nexto;
			} else {
				break;
			}
		}
		return bgn;
	}

		void for_each_node(FunN fun) {
			std::set<pNode> setp;
			pNode cur = cli;
			while (cur != nullptr) {
				setp.insert(cur);
				cur = cur->nextc;
			}
			cur = obj;
			while (cur != nullptr) {
				setp.insert(cur);
				cur = cur->nexto;
			}
			for(auto& pn : setp) {
				fun(pn);
			}
		}

		void for_each_edge_clip(FunNN fun) {
			pNode first = cli;
			pNode second;
			while(first->nextc !=nullptr) {
				second = first->nextc;
				fun(first, second);
				first = second;
			}
			fun(first, cli);
		}

		void for_each_edge_object(FunNN fun) {
			pNode first = obj;
			pNode second;
			while (first->nexto != nullptr) {
				second = first->nexto;
				fun(first, second);
				first = second;
			}
			fun(first, obj);
		}

		void for_each_for_each(FunNNNN fun) {
			pNode fc = cli;
			pNode sc;
			pNode fo = obj;
			pNode so;
			while (fc->nextc != nullptr) {
				sc = fc->nextc;
				fo = obj;
				while( fo->nexto != nullptr) {
					so = fo->nexto;
					fun(fc, sc, fo, so);
					fo = so;
				}
				fun(fc, sc, fo, obj);
				fc = sc;
			}

			fo = obj;
			while (fo->nexto != nullptr) {
				so = fo->nexto;
				fun(fc, cli, fo, so);
				fo = so;
			}
			fun(fc, cli, fo, obj);
		}

	protected:
		void _merge_to_clip() {
			typedef typename IntersectionTable::size_type Tst;
			std::set<Tst> used;
			for (Tst ic = 0; ic != this->_table.size(); ic++) {
				if (used.find(ic) == used.end()) {
					used.insert(ic);
					typedef std::vector<Tst> IndexTable;
					IndexTable idxtable;
					idxtable.push_back(ic);
					for (Tst io = ic + 1; io != this->_table.size(); io++) {
						if (this->_table[ic].cs == this->_table[io].cs
								&& this->_table[ic].ce == this->_table[io].ce) {
							idxtable.push_back(io);
							used.insert(io);
						}
					}
					if (idxtable.size() > 1) {
						// table needs sort
						std::sort(idxtable.begin(), idxtable.end(),
								[this, &ic](const Tst& a, const Tst& b) {
									auto& pois = this->_table[ic].cs->point;
									auto da = Distance(pois, this->_table[a].inter->point);
									auto db = Distance(pois, this->_table[b].inter->point);
									return da < db;
								});
					}
					pNode pn = this->_table[ic].cs;
					for (auto& idx : idxtable) {
						if (pn != this->_table[idx].inter) {
							this->insert_c(pn, this->_table[idx].inter);
						}
						pn = this->_table[idx].inter;
					}
				}
			}
		}

		void _merge_to_object() {
			typedef typename IntersectionTable::size_type Tst;
			std::set<Tst> used;
			for (Tst io = 0; io != this->_table.size(); io++) {
				if (used.find(io) == used.end()) {
					used.insert(io);
					typedef std::vector<Tst> IndexTable;
					IndexTable idxtable;
					idxtable.push_back(io);
					for (Tst ic = io + 1; ic != this->_table.size(); ic++) {
						if ( this->_table[io].os == this->_table[ic].os
								&& this->_table[io].oe == this->_table[ic].oe) {
							idxtable.push_back(ic);
							used.insert(ic);
						}
					}
					if (idxtable.size() > 1) {
						// table needs sort
						std::sort(idxtable.begin(), idxtable.end(),
								[this, &io](const Tst& a, const Tst& b) {
									auto& pois = this->_table[io].os->point;
									auto da = Distance(pois, this->_table[a].inter->point);
									auto db = Distance(pois, this->_table[b].inter->point);
									return da < db;
								});
					}
					pNode& pn = this->_table[io].os;
					for (auto& idx : idxtable) {
						if (pn != this->_table[idx].inter) {
							this->insert_o(pn, this->_table[idx].inter);
						}
						pn = this->_table[idx].inter;
					}
				}
			}

		}

		void _build_clip_link(const PointChain& pc) {
			pNode last = cli;
			for(auto& p : pc) {
				pNode pn = new Node(p, _PC_);
				if(cli == nullptr) {
					cli = pn;
					last = pn;
				} else {
					last->nextc = pn;
					pn->prevc = last;
					last = pn;
				}
			}
		}

		void _build_object_link(const PointChain& po) {
			pNode last = obj;
			for (auto& p : po) {
				pNode pn = new Node(p, 2);
				if (obj == nullptr) {
					obj = pn;
					last = pn;
				} else {
					last->nexto = pn;
					pn->prevo = last;
					last = pn;
				}
			}
		}

		void _delete_nodes() {
			std::set<pNode> setp;
			pNode cur = cli;
			while (cur != nullptr) {
				setp.insert(cur);
				cur = cur->nextc;
			}
			cur = obj;
			while (cur != nullptr) {
				setp.insert(cur);
				cur = cur->nexto;
			}
			for (auto& pn : setp) {
				delete pn;
			}
		}

		void insert_c(pNode& p, pNode& pnew) {
			pnew->nextc = p->nextc;
			pnew->prevc = p;
			if (p->nextc != nullptr) {
				p->nextc->prevc = pnew;
			}
			p->nextc = pnew;
		}

		void insert_o(pNode& p, pNode& pnew) {
			pnew->nexto = p->nexto;
			pnew->prevo = p;
			if (p->nexto != nullptr) {
				p->nexto->prevo = pnew;
			}
			p->nexto = pnew;
		}

		void _merge(pNode& pc, pNode& po) {
			// merge po to pc
			pc->prevo = po->prevo;
			pc->nexto = po->nexto;

			if (po->prevo != nullptr) {
				po->prevo->nexto = pc;
			}
			if (po->nexto != nullptr) {
				po->nexto->prevo = pc;
			}

			pc->type = _PCO_;

			if(po == obj) {
				obj = pc;
			}
			delete po;
		}

		pNode _last(int _ooc) const {
			if(_ooc == 0) {
				return _last_c();
			} else {
				return _last_o();
			}
		}

		pNode _last_o()
		const {
			pNode p = obj;
			while (p->nexto != nullptr) {
				p = p->nexto;
			}
			return p;
		}

		pNode _last_c()
		const {
			pNode p = cli;
			while (p->nextc != nullptr) {
				p = p->nextc;
			}
			return p;
		}

		bool _is_switch(pNode& p) {
			if( (p->nextc != nullptr || p->prevc != nullptr)
					&& (p->nexto != nullptr || p->prevo != nullptr)) {
				return true;
			} else {
				return false;
			}
		}

		int _cal_switch_type(pNode& p) {
			// p must be a switch point
			// p clip
			pNode nc = p->nextc;
			pNode no = p->nexto;
			pNode pc = p->prevc;
			pNode po = p->prevo;
			if(pc == nullptr) {
				pc = _last_c();
			}
			if(po == nullptr) {
				po = _last_o();
			}
			if(nc == nullptr) {
				nc = cli;
			}
			if(no == nullptr) {
				no = obj;
			}
			// Switch Type
			// NOTHING  --  0
			// IN       --  2
			// OUT      -- -2
			// HALF_IN  --  1
			// HALF_OUT -- -1
			// tri == POSVITIVE
			//       ein  eon   eout
			// sin   Ri   HOUTi OUT
			//       11   12    13
			// son   HINi O     HOUTo
			//       21   22    23
			// sout  IN   HINo  Ro
			//       31   32    33

			// Step 1 clip is angle is acute or obtuse
			int type = 0;
			auto tri = OnWhichSide3(pc->point, p->point, nc->point);
			if (tri == _POSITIVE_ ) { // Straight line or acute angle
				auto os1 = OnWhichSide3(pc->point, p->point, po->point);
				auto os2 = OnWhichSide3( p->point, nc->point, po->point);
				if( os1 == _POSITIVE_
						&& os2 == _POSITIVE_) {
					type += 10;    //object start is inside
				} else if( (os1 == _ZERO_ && os2 == _POSITIVE_)
						||(os1 == _POSITIVE_ && os2 == _ZERO_ )) {
					type += 20;    //object start is online
				} else {
					type += 30;    // object start is outside
				}
				// ----- end point ---
				auto oe1 = OnWhichSide3(pc->point, p->point, no->point);
				auto oe2 = OnWhichSide3(p->point, nc->point, no->point);
				if (oe1 == _POSITIVE_ && oe2 == _POSITIVE_) {
					type += 1;    //object end is inside
				} else if ((oe1 == _ZERO_ && oe2 == _POSITIVE_)
						|| (oe1 == _POSITIVE_ && oe2 == _ZERO_)) {
					type += 2;    //object end is online
				} else {
					type += 3;   // object end is outside
				}
			} else if( tri == _ZERO_) {
				auto os1 = OnWhichSide3(pc->point, p->point, po->point);
				if (os1 == _POSITIVE_) {
					type += 10;   //object start is inside
				} else if (os1 == _ZERO_) {
					type += 20;    //object start is online
				} else {
					type += 30;    // object start is outside
				}
				auto oe1 = OnWhichSide3(pc->point, p->point, no->point);
				if (oe1 == _POSITIVE_) {
					type += 1;    //object end is inside
				} else if (oe1 == _ZERO_) {
					type += 2;    //object end is online
				} else {
					type += 3;   // object end is outside
				}
			} else { // clip is obtuse angle
				auto os1 = OnWhichSide3(pc->point, p->point, po->point);
				auto os2 = OnWhichSide3(p->point, nc->point, po->point);
				if (os1 == _NEGATIVE_ && os2 == _NEGATIVE_) {
					type += 30;   //object start is outside
				} else if ((os1 == _ZERO_ && os2 == _NEGATIVE_)
						|| (os1 == _NEGATIVE_ && os2 == _ZERO_ )) {
					type += 20;   //object start is online
				} else {
					type += 10;  // object start is inside
				}
				//
				auto oe1 = OnWhichSide3(pc->point, p->point, no->point);
				auto oe2 = OnWhichSide3(p->point, nc->point, no->point);
				if (oe1 == _NEGATIVE_ && oe2 == _NEGATIVE_) {
					type += 3;    //object end is outside
				} else if ((oe1 == _ZERO_ && oe2 == _NEGATIVE_)
						|| (oe1 == _NEGATIVE_ && oe2 == _ZERO_)) {
					type += 2;     //object end is online
				} else {
					type += 1;     // object end is inside
				}
			}
			return type;
		}

		void _delete_overlap_edge(std::list<pNode>& lpn) {
			// this list is an output of an operation
			// edge could be overlap each other in some special cases
			typedef typename std::list<pNode>::iterator Iterpn;
			for(auto it = lpn.begin(); it != lpn.end();) {
				auto itp = std::next(it,1);
				if(itp == lpn.end()) {
					itp = lpn.begin();
				}
				auto itm = std::prev(it,1);
				if(itm == lpn.end()) {
					itm = std::prev(lpn.end(),1);
				}
				if((*itp) == (*itm)) {
					// address equal
					lpn.erase(itm);
					it = lpn.erase(it);
				} else {
					++it;
				}
			}
		}

		std::string _to_string_node_type(int type) {
			if(type < 10) {
				switch (type) {
					case _PN_ : return "N";
					case _PC_ : return "C";
					case _PO_ : return "O";
					case _PCO_: return "CO";
					default:    return "ERROR TYPE";
				}
			} else { //Switch type
				const int base = 100;
				switch(type) {
					case _OVERLAP_     : return "ON";
					case _REFLECT_in_  : return "Rin";
					case _REFLECT_out_ : return "Rout";
					case _HALF_IN_in_  : return "HINi";
					case _HALF_IN_out_ : return "HINo";
					case _IN_          : return "IN";
					case _HALF_OUT_in_ : return "HOUTi";
					case _HALF_OUT_out_: return "HOUTo";
					case _OUT_         : return "OUT";
					default            : return "ERROR TYPE";
				}
			}
		}

	public:
		void show_pnode(pNode pn) {
			ASSERT(pn!=nullptr);
			std::cout << "clip  : ";
			if (pn->nextc != nullptr) {
				std::cout << pn->nextc->point;
			} else {
				std::cout << "NULL   ";
			}
			if (pn->prevc != nullptr) {
				std::cout << "    " <<pn->prevc->point << std::endl;
			} else {
				std::cout << "   NULL\n";
			}

			std::cout << "              " << pn->point <<std::endl;

			std::cout << "object: ";
			if (pn->nexto != nullptr) {
				std::cout << pn->nexto->point;
			} else {
				std::cout << "NULL   ";
			}
			if (pn->prevo != nullptr) {
				std::cout << "    " << pn->prevo->point << std::endl;
			} else {
				std::cout << "   NULL\n";
			}

		}

		void show_table() {
			int count = 0;
			std::cout << std::setw(5) << "Idx"
			<< std::setw(7) << "type"
			<< std::setw(7) << "c str"
			<< std::setw(8) << "c end"
			<< std::setw(9) << "o str"
			<< std::setw(9) << "o end"
			<< std::setw(9) << "n p"

			<< std::endl;
			for(auto& row : this->_table) {
				std::cout << std::setw(5) << count
				<< std::setw(7) << row.type
				<< std::setw(3) << row.cs->point
				<< std::setw(3) << row.ce->point
				<< std::setw(3) << row.os->point
				<< std::setw(3) << row.oe->point
				<< std::setw(3) << row.inter->point
				<<std::endl;
				count++;
			}
		}

		GnuplotActor::spActor
		actor_clip(Gnuplot& gnu, Vt ratio=0.1) {
			GnuplotActor::spActor actor = Gnuplot::spActor(new Gnuplot_actor());
			actor->command() = "using 1:2:3:4:5 title \"\" ";
			actor->style() = "with vectors lc variable";

			FunNN fun = [&actor, &gnu, this, &ratio](pNode f, pNode s) {
				Vt len = Distance(f->point, s->point);
				Vt dx = s->point.x() - f->point.x();
				Vt dy = s->point.y() - f->point.y();
				Vt ddx = dx * ratio;
				Vt ddy = dy * ratio;
				actor->data().push_back(
						ToString(f->point.x() + ddx,
								f->point.y() + ddy,
								dx - 2 * ddx,
								dy - 2 * ddy, 1, " "));
			};

			for_each_edge_clip(fun);

			return actor;
		}

		GnuplotActor::spActor
		actor_object(Gnuplot& gnu, Vt ratio = 0.1) {
			GnuplotActor::spActor actor = Gnuplot::spActor(new Gnuplot_actor());
			actor->command() = "using 1:2:3:4:5 title \"\" ";
			actor->style() = "with vectors lc variable";

			FunNN fun = [&actor, &gnu, this, &ratio](pNode f, pNode s) {
				Vt len = Distance(f->point, s->point);
				Vt dx = s->point.x() - f->point.x();
				Vt dy = s->point.y() - f->point.y();
				Vt ddx = dx * ratio;
				Vt ddy = dy * ratio;
				actor->data().push_back(
						ToString(f->point.x() + ddx,
								f->point.y() + ddy,
								dx - 2 * ddx,
								dy - 2 * ddy, 2, " "));
			};

			for_each_edge_object(fun);

			return actor;
		}

		GnuplotActor::spActor
		actor_label(Gnuplot & gnu)
		{
			GnuplotActor::spActor actor = Gnuplot::spActor(new Gnuplot_actor());

			int count = 1;
			FunN fun = [&actor, &count, &gnu, this](pNode pn) {
				gnu.set_label(count,
						this->_to_string_node_type(pn->type),
						pn->point.x(),pn->point.y(), "center");
				count++;
			};

			for_each_node(fun);

			return actor;
		}

	};


}

#endif
