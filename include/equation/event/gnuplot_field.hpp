#ifndef _EVENT_GNUPLOT_FIELD_HPP
#define _EVENT_GNUPLOT_FIELD_HPP

#include <map>
#include <utility>
#include <iostream>
#include <list>
#include "type_define.hpp"
#include "event.hpp"
// #include "utility/clock.hpp"
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
        _format = "%s_%d_%8.4e";
        _path   = "./";

        _spgnu         = std::shared_ptr<Gnuplot>(new Gnuplot());
        _terminal_name = "png";
        _size_x        = 800;
        _size_y        = 600;
        _font          = "Helvetica";
        _size_font     = 12;

        // _init_fun_plot();
    }

    EventGnuplotField_(const std::string& sname, FunPlot fun,
                int is    = -1, int ie   = -1,
                int istep = -1, int flag = 0) :
             _sn(sname),
             Event(is, ie, istep, flag) {
            _format = "%s_%d_%8.4e";
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

    virtual bool is_condition_event() const{
        return false;
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

    std::string _file_name(St step, Vt t, int fob) const{
        std::list<std::string> tokens;
        Tokenize(_format, tokens ,"%");
        if(tokens.size() == 1){
            return tfm::format(_format.c_str(), _path + _sn);
        }else if (tokens.size() == 2){
            return tfm::format(_format.c_str(), _path + _sn, step);
        }else if (tokens.size() == 3){
            return tfm::format(_format.c_str(), _path + _sn, step, t);
        }else if (tokens.size() == 4){
            return tfm::format(_format.c_str(), _path +_sn, step, t, fob);
        }
        SHOULD_NOT_REACH;
        return "";
    }
};


}
#endif