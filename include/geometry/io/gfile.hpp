/*
 * _actor_gnuplot.hpp
 *
 *  Created on: Jul 5, 2017
 *      Author: zhou
 */

#ifndef _GEOMETRY_FILE_HPP_
#define _GEOMETRY_FILE_HPP_

#include <array>
#include <memory>
#include <cmath>
#include <sstream>

#include "geometry/geometry_define.hpp"
#include "geometry/objects/objects.hpp"
#include "utility/tinyformat.hpp"
#include "io/text_file.hpp"

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
};

inline void _FileTypeCheck(const TextFile& file, St d, PointChainTag){
    const St dimf = St(std::stoi(file.get_config("Dim")));
    if (dimf < d){
        throw std::invalid_argument("File Dim is " + ToString(dimf) + ", Less than " + ToString(d));
    }
    if (dimf > d){
        std::cout << "File Dim is " << ToString(dimf) <<  ", larger than " << ToString(d);
    }
    const St size = St(std::stoi(file.get_config("Size")));
    const std::string type = file.get_config("Type");
    if (ToLowerCase(type) != "pointchain"){
        std::cout << "File Type is " << type <<  "is not PointChain";
    }   
}

template<typename TYPE, St DIM>
Point_<TYPE, DIM> _ParseLineAs(const std::vector<std::string>& tokens, PointTag){
    St td = tokens.size();
    Point_<TYPE, DIM> res;
    if (td > 0) {
        St d = std::min(td, DIM);
        for (St i = 0; i < d; ++i) {
            res[i] = TYPE(std::stod(tokens[i]));
        }
    }
    return res;
}
template<class ANY>
ANY _ReadLines(const TextFile& tf, PointChainTag){
    ANY pc;
    for (auto& line : tf.content()) {
        if (line == "") {
            continue;
        }
        std::vector<std::string> tokens;
        Tokenize(line, tokens);
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
        Tokenize(line, tokens_d, ",");
        pc.push_back(_ParseLineAs<typename ANY::value_type::value_type, ANY::Dim>(tokens_d, PointTag()));
    }
    try{
        if( 1 ==  std::stoi(tf.get_config("Closed")))
            pc.set_close();
    }catch(std::invalid_argument& e){
        pc.set_open();
    }
    return pc;
}

// =============================
template<typename ANY,
        typename std::enable_if<
            IsGeometry<ANY>::value,
        bool>::type = true>
ANY ReadFile(const std::string& fullname){
    TextFile txtf(fullname);
    txtf.read();
    txtf.parse_config();
    _FileTypeCheck(txtf, ANY::Dim, ANY::Tag());
    
    return _ReadLines<ANY>(txtf, ANY::Tag());

}


}
#endif
