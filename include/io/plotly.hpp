#ifndef _PLOTLY_HPP_
#define _PLOTLY_HPP_

#include "io_define.hpp"
#include "algebra/array/array_list.hpp"

// #include <Python.h>
#include <map>
#include <cmath>
#include <memory>
#include <algorithm>

#ifdef _DEBUG
  #undef _DEBUG
  #include <python.h>
  #define _DEBUG
#else
  #include <Python.h>
#endif

#include "python_interpreter.hpp"


namespace carpio {


// class Plotly_actor {
// public:
//     typedef PyObject* pPO;
//     typedef std::map<std::string, pPO> Map;
//     typedef ArrayListV_<double> Arrd;
//     typedef std::list<double> Listd;

// protected:
//     Map _map;
//     pPO _module;

// public:
//     Plotly_actor(){
//     }
//     virtual ~Plotly_actor(){};

//     pPO get_p_python_object() {
//         pPO dict = PyDict_New();
//         for (Map::const_iterator iter = this->_map.begin();
//                 iter != this->_map.end(); iter++) {
//             const std::string& skey = iter->first;
//             pPO key = Py_BuildValue("s", skey.c_str());
//             pPO val = iter->second;
//             PyDict_SetItem(dict, key, val);
//         }
//         pPO args = Py_BuildValue("(O)", dict);
//         return PyObject_CallObject(this->_module, args);
//     }

//     pPO get_module(std::string name) {
//         pPO moduleName = PyUnicode_FromString("plotly.graph_objs");
//         pPO imn = PyImport_Import(moduleName);
//         pPO module = PyObject_GetAttrString(imn, name.c_str());
//         ASSERT(!(!module || !PyCallable_Check(module)));
//         return module;
//     }

//     void set_name(const std::string& name) {
//         pPO pv = Py_BuildValue("s", name.c_str());
//         this->_map["name"] = pv;
//         //this->_map["showlegend"] = Py_BuildValue("p", 1);
//     }

//     void set_opacity(const double& val) {
//         ASSERT(val >= 0 && val <= 1);
//         pPO pv = Py_BuildValue("d", val);
//         this->_map["opacity"] = pv;
//     }

//     // Any combination of ['lines', 'markers', 'text'] joined with '+' characters
//     // (e.g. 'lines+markers')
//     void set_mode(const std::string& mode) {
//         pPO pv = Py_BuildValue("s", mode.c_str());
//         this->_map["mode"] = pv;
//     }

// protected:

//     template<class Container>
//     pPO _to_list(const Container& arr, int jump = 0) const {
//         bool checkt = IsIterable<Container>::value;
//         ASSERT(checkt);
//         pPO pl = PyList_New(0);
//         int i = 0;
//         for (Listd::value_type v : arr) {
//             pPO p = Py_BuildValue("d", v);
//             if (jump > 0) {
//                 if (i % jump == 0 && i > 0) {
//                     PyList_Append(pl, Py_None);
//                 }
//             }
//             PyList_Append(pl, p);
//             i++;
//         }
//         return pl;
//     }

//     pPO _to_list_list(const Listd& arr,
//                       const typename Listd::size_type& ndim1) {
//         ASSERT(ndim1 > 0);
//         pPO pl = PyList_New(0);
//         int i = 0;
//         pPO prow = PyList_New(0);
//         for (Listd::value_type v : arr) {
//             if (i % ndim1 != 0 || i == 0) {
//                 pPO p = PyFloat_FromDouble(v);
//                 PyList_Append(prow, p);
//             } else {
//                 PyList_Append(pl, prow);
//                 prow = PyList_New(0);
//                 pPO p = PyFloat_FromDouble(v);
//                 PyList_Append(prow, p);
//             }
//             i++;
//         }
//         return pl;
//     }

// };

// class Plotly_actor_scatter: public Plotly_actor {
// public:
//     typedef PyObject* pPO;
//     typedef ArrayListV_<double> Arrd;
//     typedef std::list<double> Listd;
//     public:
//     template<class Container>
//     Plotly_actor_scatter(
//             const Container& x,
//             const Container& y,
//             int jump = 0) {
//         bool checkt = IsIterable<Container>::value;
//         //Py_Initialize();
//         _module = this->get_module("Scatter");
//         pPO px = this->_to_list(x, jump);
//         pPO py = this->_to_list(y, jump);
//         this->_map["x"] = px;
//         this->_map["y"] = py;
//     }

//     void set_colorscale_range(const double& minv, const double & maxv) {
//         // make sure the minv and maxv is the real min and max, otherwise the function will not work.
//         pPO dict = this->_map["marker"];
//         pPO cs = Py_BuildValue("s", "color");
//         pPO lv = PyDict_GetItem(dict, cs);
//         pPO minp = Py_BuildValue("d", minv);
//         PyList_Append(lv, minp);
//         pPO maxp = Py_BuildValue("d", maxv);
//         PyList_Append(lv, maxp);
//         //PyDict_SetItem(dict, cs, lv);
//         //this->_map["marker"] = dict;
//     }

//     void set_colorscale_name(const std::string& name) {
//         pPO dict = this->_map["marker"];
//         pPO key = Py_BuildValue("s", "colorscale");
//         pPO val = Py_BuildValue("s", name.c_str());
//         PyDict_SetItem(dict, key, val);
//     }

//     void set_colorscale(
//             const Listd& d, //
//             const int& size = 10, const std::string name = "Viridis",
//             const double& minv = 0.0, const double& maxv = 0.0) {
//         typedef typename Listd::value_type vt;
//         pPO dict = PyDict_New();
//         pPO key = Py_BuildValue("s", "color");
//         //vt min = *std::min_element(d.begin(), d.end());
//         //vt max = *std::max_element(d.begin(), d.end());
//         //vt scale = 1 / min;
//         //Listd nd;
//         //std::for_each(d.begin(), d.end(), [&nd](vt i) {
//         //std::cout<<-i<<" \n";
//         //	nd.push_back(-i * 10000);
//         //});
//         pPO val = this->_to_list(d);
//         if (!(minv == maxv && minv == 0.0)) {
//             pPO minp = Py_BuildValue("d", minv);
//             PyList_Append(val, minp);
//             pPO maxp = Py_BuildValue("d", maxv);
//             PyList_Append(val, maxp);
//         }
//         //
//         PyDict_SetItem(dict, key, val);
//         pPO cs = Py_BuildValue("s", "colorscale");
//         pPO val2 = Py_BuildValue("s", name.c_str());
//         PyDict_SetItem(dict, cs, val2);
//         pPO key3 = Py_BuildValue("s", "size");
//         pPO val3 = Py_BuildValue("i", size);
//         PyDict_SetItem(dict, key3, val3);
//         //pPO key4 = Py_BuildValue("s", "colorscale");
//         //pPO val4 = Py_BuildValue("i", 1);
//         //std::cout<<"val4 "<<PyObject_IsTrue(val4)<<"\n";
//         //PyDict_SetItem(dict, key4, val4);
//         this->_map["marker"] = dict;
//     }

//     void set_add_val(const Listd& d) {
//         pPO val = this->_to_list(d);
//         this->_map["text"] = val;
//     }

// };

// class Plotly_actor_scatter3d: public Plotly_actor {
// public:
//     typedef PyObject* pPO;
//     typedef ArrayListV_<double> Arrd;
//     typedef std::list<double> Listd;

// public:

//     template<class Container>
//     Plotly_actor_scatter3d(
//             const Container& x,
//             const Container& y,
//             const Container& z,
//             int jump = 0) {
//         bool checkt = IsIterable<Container>::value;
//         //Py_Initialize();
//         _module = this->get_module("Scatter3d");
//         pPO px = this->_to_list(x, jump);
//         pPO py = this->_to_list(y, jump);
//         pPO pz = this->_to_list(z, jump);
//         this->_map["x"] = px;
//         this->_map["y"] = py;
//         this->_map["z"] = pz;
//     }

//     void set_colorscale(
//             const Listd& d,
//             const int& size = 10,
//             const std::string name = "Viridis") {
//         typedef typename Listd::value_type vt;
//         pPO dict = PyDict_New();
//         pPO key  = Py_BuildValue("s", "color");
//         pPO val  = this->_to_list(d);
//         PyDict_SetItem(dict, key, val);
//         pPO key2 = Py_BuildValue("s", "colorscale");
//         pPO val2 = Py_BuildValue("s", name.c_str());
//         PyDict_SetItem(dict, key2, val2);
//         pPO key3 = Py_BuildValue("s", "size");
//         pPO val3 = Py_BuildValue("i", size);
//         PyDict_SetItem(dict, key3, val3);
//         pPO key4 = Py_BuildValue("s", "showscale");
//         pPO val4 = Py_BuildValue("i", size);
//         PyDict_SetItem(dict, key4, val4);
//         this->_map["marker"] = dict;
//     }

//     void set_add_val(const Listd& d) {
//         pPO val = this->_to_list(d);
//         this->_map["text"] = val;
//     }

// };

// class Plotly_actor_mesh3d: public Plotly_actor {
// public:
//     typedef PyObject* pPO;
//     typedef ArrayListV_<double> Arrd;
//     public:
//     template<class Container>
//     Plotly_actor_mesh3d(const Container& x, const Container& y,
//             const Container& z) {
//         bool checkt = IsIterable<Container>::value;
//         ASSERT(checkt);
//         _module = this->get_module("Mesh3d");
//         pPO px = this->_to_list(x);
//         pPO py = this->_to_list(y);
//         pPO pz = this->_to_list(z);
//         this->_map["x"] = px;
//         this->_map["y"] = py;
//         this->_map["z"] = pz;
//         //_data = nullptr;
//     }

//     void set_ijk(const Listd& x, const Listd& y, const Listd& z) {
//         pPO px = this->_to_list(x);
//         pPO py = this->_to_list(y);
//         pPO pz = this->_to_list(z);
//         this->_map["i"] = px;
//         this->_map["j"] = py;
//         this->_map["k"] = pz;
//     }
// };

// class Plotly_actor_heatmap: public Plotly_actor {
// public:
//     typedef PyObject* pPO;
//     typedef ArrayListV_<double> Arrd;
//     typedef std::list<double> Listd;
//     public:
//     // x, y indicate the location of the faces
//     Plotly_actor_heatmap(const Listd& x, const Listd& y, const Listd& z) {
//         //Py_Initialize();
//         _module = this->get_module("Heatmap");
//         pPO px = this->_to_list(x);
//         pPO py = this->_to_list(y);
//         pPO pz = this->_to_list_list(z, x.size() - 1);
//         this->_map["x"] = px;
//         this->_map["y"] = py;
//         this->_map["z"] = pz;
//     }

//     void colorscale(const std::string& cs = "Vidridis") {
//         pPO val = Py_BuildValue("s", cs.c_str());
//         this->_map["colorscale"] = val;
//     }

// };

// class Plotly {
// public:
//     typedef PyObject* pPO;
//     typedef std::shared_ptr<Plotly_actor> spPA;
//     typedef std::map<std::string, pPO> Map;

// public:
//     Plotly() {
//         _init();
//         set_auto_open(false);
//         set_filename("./out.html");
//     }

//     ~Plotly() {
//         //Py_Finalize();
//     }

//     std::string version() const {
//         pPO version = PyObject_GetAttrString(_modulePlotly, "__version__");
//         pPO repr = PyObject_Repr(version);
//         pPO cstr = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
//         const char *bytes = PyBytes_AS_STRING(cstr);
//         ASSERT(cstr != nullptr);
//         std::string str(bytes);
//         str.erase(0, str.find_first_not_of('\''));     //prefixing '\''
//         str.erase(str.find_last_not_of('\'')+1);
//         Py_XDECREF(repr);
//         Py_XDECREF(cstr);
//         return str;
//     }

//     void add(spPA spa) {
//         this->_actors.push_back(spa);
//     }
//     //-- If True, open the saved file in a web browser after saving.
//     //    This argument only applies if `output_type` is 'file'.
//     void set_auto_open(bool flag) {
//         /// default is false
//         pPO val = Py_BuildValue("i", flag ? 1 : 0);
//         _map_config["auto_open"] = val;
//     }

//     void set_filename(const std::string& fn) {
//         pPO val = Py_BuildValue("s", fn.c_str());
//         _map_config["filename"] = val;
//     }

//     // default is output file
//     // if your want to embed in other html file
//     // the output type should be "div"
//     void set_output_type(const std::string& fn) {
//         ASSERT(fn == "file" || fn == "div");
//         pPO val = Py_BuildValue("s", fn.c_str());
//         _map_config["output_type"] = val;
//     }

//     void set_include_plotlyjs(bool b){
//         pPO val = Py_BuildValue("i", b ? 1 : 0);
//         _map_config["include_plotlyjs"] = val;
//     }

//     void title(const std::string& t) {
//         pPO val = Py_BuildValue("s", t.c_str());
//         _map_layout["title"] = val;
//     }

//     void width(const int& w) {
//         ASSERT(w > 10);
//         pPO val = Py_BuildValue("i", w);
//         _map_layout["width"] = val;
//     }
//     void height(const int& w) {
//         ASSERT(w > 10);
//         pPO val = Py_BuildValue("i", w);
//         _map_layout["height"] = val;
//     }
//     void size(const int& w, const int& h) {
//         //this->disable_autosize();
//         this->width(w);
//         this->height(h);
//     }

//     std::string plot() const {
//         pPO args_fig = Py_BuildValue("(O)", _get_fig());
//         pPO args_o = Py_BuildValue("O", _get_config());
//         pPO pRet = PyObject_Call(_funPlot, args_fig, args_o);
//         // pPO to string
//         pPO repr = PyObject_Repr(pRet);
//         pPO cstr = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
//         const char *bytes = PyBytes_AS_STRING(cstr);
//         ASSERT(cstr != nullptr);
//         std::string str(bytes);
//         str.erase(0, str.find_first_not_of('\''));     //prefixing '\''
//         str.erase(str.find_last_not_of('\'')+1);
//         Py_XDECREF(repr);
//         Py_XDECREF(cstr);
//         //
//         return str;
//     }

// protected:
// // data
//     pPO _modulePlotly;
//     pPO _funPlot;
//     //pPO _filename;

//     Map _map_layout;
//     Map _map_config;

//     std::list<spPA> _actors;

//     pPO _get_layout() const {
//         pPO dict = PyDict_New();
//         for (Map::const_iterator iter = this->_map_layout.begin();
//                 iter != this->_map_layout.end(); iter++) {
//             const std::string& skey = iter->first;
//             pPO key = Py_BuildValue("s", skey.c_str());
//             pPO val = iter->second;
//             PyDict_SetItem(dict, key, val);
//         }
//         pPO args = Py_BuildValue("(O)", dict);
//         pPO _module = _get_module("Layout");
//         return PyObject_CallObject(_module, args);
//     }

//     pPO _get_fig() const {
//         pPO dict = PyDict_New();
//         /// data
//         pPO data = PyList_New(0);
//         //pPO list_actors;
//         for (spPA pa : _actors) {
//             pPO p = pa->get_p_python_object();
//             PyList_Append(data, p);
//         }
//         if (PyList_Size(data) == 0) {
//             return nullptr;
//         }
//         PyDict_SetItem(dict, Py_BuildValue("s", "data"), data);
//         /// layout
//         pPO val = _get_layout();
//         PyDict_SetItem(dict, Py_BuildValue("s", "layout"), val);
// //		pPO args = Py_BuildValue("(O)", dict);
//         return dict;
//     }

//     pPO _get_config() const {
//         pPO dict = PyDict_New();
//         for (auto& pair : _map_config) {
//             PyDict_SetItem(dict, Py_BuildValue("s", pair.first.c_str()), pair.second);
//         }
//         return dict;
//     }

//     pPO _get_module(std::string name) const {
//         pPO moduleName = PyUnicode_FromString("plotly.graph_objs");
//         pPO imn = PyImport_Import(moduleName);
//         pPO module = PyObject_GetAttrString(imn, name.c_str());
//         ASSERT(!(!module || !PyCallable_Check(module)));
//         return module;
//     }

//     void _init() {
//         Py_Initialize();
//         pPO moduleName = PyUnicode_FromString("plotly");
//         _modulePlotly = PyImport_Import(moduleName);

//         moduleName = PyUnicode_FromString("plotly.offline");
//         pPO offline = PyImport_Import(moduleName);
//         _funPlot = PyObject_GetAttrString(offline, "plot");

//         //_filename = nullptr;
//         ASSERT(!(!_funPlot || !PyCallable_Check(_funPlot)));
//     }

// };


// namespace PlotlyActor{

// typedef std::shared_ptr<carpio::Plotly_actor> spPA;
// typedef std::shared_ptr<carpio::Plotly_actor_scatter> spPA_scatter;
// typedef std::shared_ptr<carpio::Plotly_actor_scatter3d> spPA_scatter3d;
// typedef std::shared_ptr<carpio::Plotly_actor_mesh3d> spPA_mesh3d;
// typedef std::shared_ptr<carpio::Plotly_actor_heatmap> spPA_heatmap;
// }
class PlotlyActor {
public:
    typedef PyObject* pPO;
    // typedef std::map<std::string, pPO> Map;
    typedef ArrayListV_<double> Arrd;
    typedef std::list<double> Listd;

protected:
    // Map _map;
    PythonInterpreter* _py;
    std::string _trace_type;
    PyObject*   _trace;
    PyObject*   _go;

public:
    PlotlyActor(){
        this->_py = PythonInterpreter::Get();
        this->_go = this->_py->import("plotly.graph_objects");
        this->_trace = nullptr;
        this->_trace_type = "nullptr";
    }
    PlotlyActor(const std::string& trace_type){
        this->_py = PythonInterpreter::Get();
        this->_go = this->_py->import("plotly.graph_objects");
        _new_trace(trace_type);
    }
    PlotlyActor(const PlotlyActor& other):
        _trace_type(other._trace_type),_trace(other._trace),_go(other._go){
        this->_py = PythonInterpreter::Get();
    }
    PlotlyActor& operator=(const PlotlyActor &other) {
        if (this == &other) {
            return *this;
        }
        this->_trace_type = other._trace_type; 
        this->_trace = other._trace;
        this->_go = other._go;
        this->_py = other._py;
        return *this;
    }
    void update_colorscale(const std::string& cmin, const std::string& cmax){
        pPO dict    = PyDict_New();
        pPO tarray  = PyList_New(0);
        pPO row0    = PyList_New(0);
        PyList_Append(row0, Py_BuildValue("d", 0.0));
        PyList_Append(row0, Py_BuildValue("s", cmin.c_str()));
        pPO row1    = PyList_New(0);
        PyList_Append(row1, Py_BuildValue("d", 1.0));
        PyList_Append(row1, Py_BuildValue("s", cmax.c_str()));

        PyList_Append(tarray, row0);
        PyList_Append(tarray, row1);

        dict = _py->dict_set(dict, "colorscale", tarray);
        _py->call_method(this->_trace, "update", dict); 
        Py_DECREF(dict);
    }
    void update(const std::string& key, const double& val){
        pPO dict = PyDict_New();
        dict = _py->dict_set(dict, key, val);
        _py->call_method(this->_trace, "update", dict); 
        Py_DECREF(dict);
    }
    void update(const std::string& key, const std::string& val){
        pPO dict = PyDict_New();
        dict = _py->dict_set(dict, key, val);
        _py->call_method(this->_trace, "update", dict); 
        Py_DECREF(dict);
    }
    void update(const std::string& key, const int& val){
        pPO dict = PyDict_New();
        dict = _py->dict_set(dict, key, val);
        _py->call_method(this->_trace, "update", dict); 
        Py_DECREF(dict);
    }
    void update_true(const std::string& key){
        pPO dict = PyDict_New();
        dict = _py->dict_set(dict, key, Py_True);
        _py->call_method(this->_trace, "update", dict); 
        Py_DECREF(dict);
    }
    void update_false(const std::string& key){
        pPO dict = PyDict_New();
        dict = _py->dict_set(dict, key, Py_False);
        _py->call_method(this->_trace, "update", dict); 
        Py_DECREF(dict);
    }
    void name(const std::string& name){
        _py->set_attr(this->_trace, "name", name);
    }
    std::string name() const{
        return _py->get_attr_as_string(this->_trace, "name");
    }
    void opacity(const double& op){
        _py->set_attr(this->_trace, "opacity", op);
    }
    double opacity() const{
        return _py->get_attr_as_float(this->_trace, "opacity");
    }
    template<class CONTAINER, ENABLE_IF_1D_ARITHMATIC(CONTAINER)> 
    void data(const CONTAINER& con, const std::string& name,int jump = 0){
        PyObject* l = _py->to_list(con, jump);
        PyObject_SetAttrString(this->_trace, name.c_str(), l);
    }
    template<class CONTAINER, ENABLE_IF_1D_ARITHMATIC(CONTAINER)> 
    void data_x(const CONTAINER& con, int jump = 0){
        PyObject* l = _py->to_list(con, jump);
        PyObject_SetAttrString(this->_trace, "x", l);
    }

    PyObject* trace() const{
        return _trace;
    }
    template<class CONTAINER, ENABLE_IF_1D_ARITHMATIC(CONTAINER)> 
    void data(const CONTAINER& con, 
              const std::string& n1, const std::string& n2, int jump = 0){
        typedef typename CONTAINER::value_type::value_type value_type;
        std::list<value_type> colx, coly; 
        for(auto& row : con){
            auto iter = row.begin();
            colx.push_back(*iter);
            std::advance(iter, 1);
            coly.push_back(*iter);
        }
        PyObject* lx = _py->to_list(colx, jump);
        PyObject* ly = _py->to_list(coly, jump);
        PyObject_SetAttrString(this->_trace, n1.c_str(), lx);
        PyObject_SetAttrString(this->_trace, n2.c_str(), ly);
    }
    template<class CONTAINER, ENABLE_IF_2D_ARITHMATIC(CONTAINER) > 
    void data_xy(const CONTAINER& con, int jump = 0){
        this->data(con, "x", "y", jump); 
    }
    template<class CONTAINER, ENABLE_IF_2D_ARITHMATIC(CONTAINER)> 
    void data(const CONTAINER& con,
              const std::string& n1, const std::string& n2, const std::string& n3, 
              int jump = 0){
        typedef typename CONTAINER::value_type::value_type value_type;
        std::list<value_type> colx, coly, colz; 
        for(auto& row : con){
            auto iter = row.begin();
            colx.push_back(*iter);
            std::advance(iter, 1);
            coly.push_back(*iter);
            std::advance(iter, 1);
            colz.push_back(*iter);
        }
        PyObject* lx = _py->to_list(colx, jump);
        PyObject* ly = _py->to_list(coly, jump);
        PyObject* lz = _py->to_list(colz, jump);
        PyObject_SetAttrString(this->_trace, n1.c_str(), lx);
        PyObject_SetAttrString(this->_trace, n2.c_str(), ly);
        PyObject_SetAttrString(this->_trace, n3.c_str(), lz);
    }
    template<class CONTAINER, ENABLE_IF_2D_ARITHMATIC(CONTAINER)> 
    void data_append(const CONTAINER& con,
                     const std::string& n1, const std::string& n2, const std::string& n3, 
                     int jump = 0){
        typedef typename CONTAINER::value_type::value_type value_type;
        std::list<value_type> colx, coly, colz; 
        for(auto& row : con){
            auto iter = row.begin();
            colx.push_back(*iter);
            std::advance(iter, 1);
            coly.push_back(*iter);
            std::advance(iter, 1);
            colz.push_back(*iter);
        }
        
        PyObject* lx = _py->to_list(colx, jump);
        PyObject* ly = _py->to_list(coly, jump);
        PyObject* lz = _py->to_list(colz, jump);
        PyObject_SetAttrString(this->_trace, n1.c_str(), lx);
        PyObject_SetAttrString(this->_trace, n2.c_str(), ly);
        PyObject_SetAttrString(this->_trace, n3.c_str(), lz);
    }
    template<class CONTAINER, ENABLE_IF_2D_ARITHMATIC(CONTAINER) > 
    void data_xyz(const CONTAINER& con, int jump = 0){
        this->data(con, "x", "y", "z", jump); 
    }
    virtual ~PlotlyActor(){};
protected:
//  trace name should be one of: 
//  [‘bar’, ‘barpolar’, ‘box’, ‘candlestick’, ’carpet’, 
//   ‘choropleth’, ‘choroplethmapbox’, ‘cone’, ‘contour’,
//   ‘contourcarpet’, ‘densitymapbox’, ‘funnel’, ‘funnelarea’,
//   ‘heatmap’, ‘heatmapgl’, ‘histogram’, ‘histogram2d’,
//   ‘histogram2dcontour’, ‘icicle’, ‘image’, ‘indicator’,
//   ‘isosurface’, ‘mesh3d’, ‘ohlc’, ‘parcats’, ‘parcoords’,
//   ‘pie’, ‘pointcloud’, ‘sankey’, ‘scatter’, ‘scatter3d’,
//   ‘scattercarpet’, ‘scattergeo’, ‘scattergl’, ‘scattermapbox’,
//   ‘scatterpolar’, ‘scatterpolargl’, ‘scattersmith’, ‘scatterternary’,
//   ‘splom’, ‘streamtube’, ‘sunburst’, ‘surface’, ‘table’,
//   ‘treemap’, ‘violin’, ‘volume’, ‘waterfall’]
    void _new_trace(const std::string& trace_type){
        pPO c_trace  = _py->import_sub(this->_go, trace_type);
        this->_trace = PyObject_CallObject(c_trace, nullptr);
        Py_DECREF(c_trace); 
    }
};


typedef std::map<std::string, PlotlyActor> PlotlyActorGroup; 



class Plotly {
public:
    typedef PyObject* pPO;
    typedef std::map<std::string, pPO> Map;
protected:
    PythonInterpreter* _py ;

    pPO _plotly;
    pPO _plotly_go;

    pPO _clase_figure;

    pPO _figure;
    pPO _layout;

public:
    Plotly() {
        this->_py = PythonInterpreter::Get();
        _init();
    }

    ~Plotly() {
        Py_DECREF(this->_plotly);
        Py_DECREF(this->_plotly_go);
        Py_DECREF(this->_figure);
        // Py_DECREF(this->_layout);
    }

    std::string version() const {
        pPO version = PyObject_GetAttrString(_plotly, "__version__");
        pPO repr    = PyObject_Repr(version);
        pPO cstr    = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
        const char *bytes = PyBytes_AS_STRING(cstr);
        ASSERT(cstr != nullptr);
        std::string str(bytes);
        str.erase(0, str.find_first_not_of('\''));     //prefixing '\''
        str.erase(str.find_last_not_of('\'')+1);
        Py_DECREF(version);
        Py_DECREF(repr);
        Py_DECREF(cstr);
        return str;
    }

    void layout_true(const std::string& key){
        pPO dict = PyDict_New();
        dict = _py->dict_set(dict, key, Py_True);
        _py->call_method(this->_figure, "update_layout", dict); 
        Py_DECREF(dict);
    }
    void layout_false(const std::string& key){
        pPO dict = PyDict_New();
        dict = _py->dict_set(dict, key, Py_False);
        _py->call_method(this->_figure, "update_layout", dict); 
        Py_DECREF(dict);
    }
    void layout(const std::string& key, const double& d){
        pPO dict = PyDict_New();
        dict = _py->dict_set(dict, key, d);
        _py->call_method(this->_figure, "update_layout", dict); 
        Py_DECREF(dict);
    }
    void layout(const std::string& key, const std::string& str){
        pPO dict = PyDict_New();
        dict = _py->dict_set(dict, key, str);
        
        _py->call_method(this->_figure, "update_layout", dict); 
        Py_DECREF(dict);
    }
    void layout(const std::string& key, const double& v1, const double& v2){
        pPO dict = PyDict_New();
        dict = _py->dict_set(dict, key, v1, v2);
        _py->call_method(this->_figure, "update_layout", dict); 
        Py_DECREF(dict);
    }
    void title(const std::string& t) {
        this->layout("title", t);
    }

    void size(const int& width = 800, const int& height = 600){
        pPO dict = PyDict_New();
        dict = _py->dict_set(dict, "width",  double(width));
        dict = _py->dict_set(dict, "height", double(height));
        _py->call_method(this->_figure, "update_layout", dict); 
    }

    void margin(const int& l=80, const int& r=80, const int& t=100, const int& b = 80){
        pPO dict = PyDict_New();
        _py->dict_set(dict, "l", double(l));
        _py->dict_set(dict, "r", double(r));
        _py->dict_set(dict, "t", double(t));
        _py->dict_set(dict, "b", double(b));
        
        pPO mdict = PyDict_New();
        _py->dict_set(mdict, "margin", dict);
        _py->call_method(this->_figure, "update_layout", mdict); 

        Py_DecRef(mdict);
        Py_DecRef(dict);
    }
    template<class ACTOR, typename std::enable_if< 
             std::is_same<ACTOR, PlotlyActor>::value , bool>::type = true>
    void add(const ACTOR& actor){
        // pPO class_Figure = _py->import_sub(this->_plotly_go, "Figure");
        auto pt = _py->import_sub(this->_figure, "add_trace");
        auto trace = actor.trace();
        if(!trace){
            throw std::invalid_argument("Trace in actor is null");
        }
        pPO args = Py_BuildValue("(O)", trace);
        PyObject_Call(pt, args, nullptr);
        
        Py_DecRef(args); 
        Py_DecRef(pt);
    }

    template<class CON, typename std::enable_if< 
                    std::is_same<typename CON::mapped_type, PlotlyActor>::value , bool>::type = true>
    void add(const CON& container){
        for(const auto& pair : container){
            this->add(pair.second);
        }
    }

    void show(){
        _py->call_method(this->_figure, "show");
    }

    void write(const std::string& fullname, const std::string& type){
        std::string name = fullname + "." + type;
        pPO poname = Py_BuildValue("s", name.c_str());
        pPO args   = Py_BuildValue("(O)", poname);
        pPO fun;
        if (type == "html"){
            fun = _py->import_sub(this->_figure, "write_html");
            PyObject_Call(fun, args, nullptr);
            return;
        }else if (type == "div"){
            poname = Py_BuildValue("s", name.c_str());
            PyObject *kwargs = Py_BuildValue("{s:s, s:O}", "include_plotlyjs", "cdn", "full_html", Py_False);
            fun = _py->import_sub(this->_figure, "write_html");
            PyObject_Call(fun, args, kwargs);
            Py_DECREF(kwargs);
            return;
        }else if(type == "png" || type == "jpeg" || type == "pdf"){
            fun = _py->import_sub(this->_figure, "write_image");
            PyObject_Call(fun, args, nullptr);
            return;
        }else{
            throw std::invalid_argument("Invalid type :" + type);
        }
    }

protected:

    void _init() {    
        // std::cout << Py_GetVersion() << std::endl;

        this->_plotly    = _py->import("plotly");
        this->_plotly_go = _py->import("plotly.graph_objects");

        // new empty figure
        pPO class_figure = _py->import_sub(this->_plotly_go, "Figure");
        this->_figure  = PyObject_CallObject(class_figure, nullptr);
        Py_DECREF(class_figure);
    }

};





}

#endif
