#ifndef _EVENT_GNUPLOT_FIELD_HPP
#define _EVENT_GNUPLOT_FIELD_HPP

#include <map>
#include <utility>
#include <iostream>
#include <list>
#include "type_define.hpp"
#include "event.hpp"
#include "utility/clock.hpp"
#include "utility/tinyformat.hpp"
#include "io/gnuplot.hpp"

namespace carpio{

template<class D>
class EventGnuplotField_ : public Event_<D>{
public:
    typedef Event_<D> Event;
    typedef EquationBase_<D> EquationBase;

    typedef typename D::FieldCenter  FieldCenter;

    typedef std::function<int(Gnuplot&, const FieldCenter&, St, Vt, int, EquationBase&)> FunPlot;
protected:
    std::string _sn;     // scalar name
    std::string _format; // format string
    std::string _path;   // path for output file

    std::shared_ptr<Gnuplot> _spgnu;
    std::string _terminal_name;
    Vt          _size_x;
    Vt          _size_y;
    std::string _font;
    St          _size_font;


    FunPlot _fun;

public:
    EventGnuplotField_(const std::string& sname,
            int is    = -1, int ie   = -1,
            int istep = -1, int flag = 0) :
         _sn(sname),
         Event(is, ie, istep, flag) {
        _format = "%s_%d_%8.4e.png";
        _path   = "./";

        _spgnu         = std::shared_ptr<Gnuplot>(new Gnuplot());
        _terminal_name = "png";
        _size_x        = 800;
        _size_y        = 600;
        _font          = "Helvetica";
        _size_font     = 12;

        _init_fun_plot();
    }

    EventGnuplotField_(const std::string& sname, FunPlot fun,
                int is    = -1, int ie   = -1,
                int istep = -1, int flag = 0) :
             _sn(sname),
             Event(is, ie, istep, flag) {
            _format = "%s_%d_%8.4e.png";
            _path   = "./";

            _spgnu         = std::shared_ptr<Gnuplot>(new Gnuplot());
            _terminal_name = "png";
            _size_x        = 800;
            _size_y        = 600;
            _font          = "Helvetica";
            _size_font     = 12;

            _fun = fun;
    }

    int execute(St step, Vt t, int fob, EquationBase& equ) {
        auto fn = _file_name(step, t, fob);
        if(equ.has_field(_sn)){
            _set_terminal(_terminal_name, fn);
            _fun(*(this->_spgnu), equ.field(_sn), step, t, fob, equ);
        }else{
            std::cerr<< "EventGnuplotField : " << _sn << " not found!" << std::endl;
        }
        return -1;
    }

    void set_format_string(const std::string& format){
        _format = format;
    }

    void set_figure_width(const int& w){
        _size_x = w;
    }
    void set_figure_height(const int& w){
        _size_y = w;
    }
    void set_figure_font(const std::string& w){
        _font = w;
    }

    void set_path(const std::string& path){
        _path = path;
    }

    Gnuplot& gnuplot(){
        return *(_spgnu);
    }

protected:
    void _set_terminal(
            const std::string& tn, // terminal name
            const std::string& fn  // file name
            ){
        if(tn == "png"){
            _spgnu->set_terminal_png(fn, _size_x, _size_y, _font, _size_font);
            return;
        }
        std::cerr << "EventGnuplotField: " << "Wrong terminal!" << std::endl;
    }

//    void _plot(const Field& s){
//        if(DIM == 1){
//            _spgnu->add(GnuplotActor::Lines(s));
//        }else if(DIM == 2){
//            _spgnu->add(GnuplotActor::Contour(s));
//        }
//        _spgnu->plot();
//        _spgnu->clear();
//    }

    void _init_fun_plot(){
        this->_fun = [](Gnuplot& gnu, const Field& f, St step , Vt t, int fob, pEqu pd){
            if(DIM == 1){
                gnu.add(GnuplotActor::Lines(f));
            }else if(DIM == 2){
                gnu.add(GnuplotActor::Contour(f));
            }
            gnu.plot();
            gnu.clear();
            return 1;
        };
    }

    std::string _file_name(St step, Vt t, int fob) const{
        std::stringstream ss;
        ss << _path;
        tfm::format(ss, _format.c_str() , _sn, step, t);
        return ss.str();
    }
};


}
#endif