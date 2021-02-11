/*
 * _actor_gnuplot.hpp
 *
 *  Created on: Jul 5, 2017
 *      Author: zhou
 */

#ifndef _GEOMETRY_FILE_HPP_
#define _GEOMETRY_FILE_HPP_

#include "geometry/objects/objects.hpp"
#include "utility/tinyformat.hpp"
#include "io/io_define.hpp"
#include <array>
#include <memory>
#include <cmath>
#include <sstream>

namespace carpio {

template<typename TYPE, St DIM>
class GFile_ {
public:
	static const St Dim = DIM;
	typedef TYPE Vt;
	typedef Point_<TYPE, DIM> Point;
	typedef Point_<TYPE, DIM>& ref_Point;
	typedef Point_<TYPE, DIM>* pPoint;
	typedef const Point_<TYPE, DIM>* const_pPoint;
	typedef const Point_<TYPE, DIM>& const_ref_Point;

	typedef PointChain_<TYPE, DIM> PointChain;

public:
	static void ReadPointChain(const std::string& fn, PointChain& pc) {
		pc.clear();
		TextFile txtf(fn);
		txtf.read();
		txtf.parse_config();

		const St dimf = St(std::stoi(txtf.get_config("Dim")));
		const St size = St(std::stoi(txtf.get_config("Size")));
		const std::string type = txtf.get_config("Type");
		ASSERT(type == "PointChain");

		for (auto& line : txtf.content()) {
			std::vector<std::string> tokens;
			if (line == "") {
				continue;
			}
			TextFile::Tokenize(line, tokens);
			if (tokens[0] == "##") {
				/// ignore dict line
				continue;
			}
			if (tokens[0] == "#") {
				/// ignore comment line
				continue;
			}
			/// data line
			std::vector<std::string> tokens_d;
			TextFile::Tokenize(line, tokens_d, ",");
			if (tokens_d.size() > 0) {
				Point p(0);
				for (St i = 0; i < dimf; i++) {
					if (i < Dim) {
						p[i] = Vt(std::stod(tokens_d[i]));
					}
				}
				pc.push_back(p);
			}
		}
	}

	static void _WritePointsHead(TextFile& txtf, St size) {
		// write head
		int d = Dim;
		txtf.add_line(tfm::format("## DIM  : %d", d));
		txtf.add_line(tfm::format("## SIZE : %d", size));
		txtf.add_line(tfm::format("## TYPE : %s", "Points"));
	}
	static void _WriteTrianglesHead(TextFile& txtf, St size) {
		// write head
		int d = Dim;
		txtf.add_line(tfm::format("## DIM  : %d", d));
		txtf.add_line(tfm::format("## SIZE : %d", size));
		txtf.add_line(tfm::format("## TYPE : %s", "Triangles"));
	}

	template<class Container>
	static void _WritePointsBody(TextFile& txtf, const Container& con,
			const_pPoint dummy) {
		for (auto& p : con) {
			std::stringstream ss;
			for (St i = 0; i < Dim; i++) {
				if (i == Dim - 1) {
					tfm::format(ss, "%10.5f", (*p)[i]);
				} else {
					tfm::format(ss, "%10.5f,", (*p)[i]);
				}
			}
			txtf.add_line(ss.str());
		}
	}
	template<class Container>
	static void _WritePointsBody(TextFile& txtf, const Container& con,
			const Point& dummy) {
		for (auto& p : con) {
			std::stringstream ss;
			for (St i = 0; i < Dim; i++) {
				if (i == Dim - 1) {
					tfm::format(ss, "%10.5f", p[i]);
				} else {
					tfm::format(ss, "%10.5f,", p[i]);
				}
			}
			txtf.add_line(ss.str());
		}
	}
	static void WritePointChain(const std::string& fn, const PointChain& pc) {
		TextFile txtf(fn);
		// write head
		int d = Dim;
		txtf.add_line(tfm::format("## DIM  : %d", d));
		txtf.add_line(tfm::format("## SIZE : %d", pc.size()));
		txtf.add_line(tfm::format("## TYPE : %s", "PointChain"));
		txtf.add_line(tfm::format("## CLOSED : %d", pc.closed()));
		typedef typename PointChain::value_type value_type;
		value_type dummy;
		_WritePointsBody(txtf, pc, dummy);
		txtf.write();
	}

	template<class Container>
	static void WritePoints(const std::string& fn, const Container& con) {
		ASSERT(IsIterable<Container>::value);
		TextFile txtf(fn);
		// write head
		_WritePointsHead(txtf, con.size());
		typedef typename Container::value_type value_type;
		value_type dummy;
		_WritePointsBody(txtf, con, dummy);
		txtf.write();
	}

	template<class Container, class Triangles>
	static void _WriteTrianglesBody(
			TextFile& txtf,
			const Container& con,
			const Triangles& tri) {
		typedef typename Triangles::value_type value_type;
		value_type dummy;
		_WritePointsBody(txtf, tri, dummy);
	}

	template<class Container>
	static void WriteTriangles(const std::string& fn, const Container& con) {
		ASSERT(IsIterable<Container>::value);
		TextFile txtf(fn);
		// write head
		_WriteTrianglesHead(txtf, con.size());
		typedef typename Container::value_type value_type;
		value_type dummy;
		for (auto& tri : con) {
			_WriteTrianglesBody(txtf, con, tri);
			txtf.add_line(""); // add empty line
		}
		txtf.write();
	}
}
;
}
#endif
