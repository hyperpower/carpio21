#pragma once
#ifndef _PYTHON_INTERPRETER_HPP_
#define _PYTHON_INTERPRETER_HPP_

#ifdef _DEBUG
  #undef _DEBUG
  #include <python.h>
  #define _DEBUG
#else
  #include <Python.h>
#endif

#include <map>
#include <cmath>
#include <mutex>
#include <memory>
#include <iostream>
#include <algorithm>

#include "type_define.hpp"

namespace carpio {

class PythonInterpreter
{
public:
    // std::array<std::array<int,3>, 3> _mapte0e1;
private:
    static PythonInterpreter * pinstance_;
    static std::mutex mutex_;
protected:
    PythonInterpreter()
    {
        // optional but recommended
#if PY_MAJOR_VERSION >= 3
        wchar_t name[] = L"plotting";
#else
        char name[] = "plotting";
#endif
        std::cout << "Singleton Python Interpreter" << std::endl;
        Py_SetProgramName(name);
        Py_Initialize();

        wchar_t const *dummy_args[] = {L"Python", NULL};  // const is needed because literals must not be modified
        wchar_t const **argv = dummy_args;
        int             argc = sizeof(dummy_args)/sizeof(dummy_args[0])-1;

#if PY_MAJOR_VERSION >= 3
        PySys_SetArgv(argc, const_cast<wchar_t **>(argv));
#else
        PySys_SetArgv(argc, (char **)(argv));
#endif

    }
    ~PythonInterpreter() {
        Py_Finalize();
    }
public:
    PyObject* import(const std::string& name) const{
        PyObject* po_name = PyUnicode_FromString(name.c_str());
        PyObject* pkg     = PyImport_Import(po_name);
        Py_DECREF(po_name);
        if (!pkg) {
            PyErr_Print();
            throw std::runtime_error("Error loading module " + name + "!");
        }
        return pkg;
    }

    PyObject* import_sub(PyObject* module, const std::string& fname) {
        PyObject* fn = PyObject_GetAttrString(module, fname.c_str());

        if (!fn)
            throw std::runtime_error(std::string("Couldn't find required function/module: ") + fname);

        if (!PyCallable_Check(fn))
            throw std::runtime_error(fname + std::string(" is not Callable."));

        return fn;
    }
    PyObject* call_method(PyObject* module, const std::string& fname) {
        PyObject* fn = PyObject_CallMethod(module, fname.c_str(), nullptr);
        return fn;
    }
    PyObject* call_method(PyObject* module, const std::string& method, PyObject* obj) {
        PyObject* args = Py_BuildValue("(O)",  obj);
        PyObject* fun = this->import_sub(module, method);
        PyObject* ret = PyObject_CallObject(fun, args);
        Py_DecRef(fun);
        Py_DecRef(args);
        return ret;
    }
    PyObject* call_method(PyObject* module, const std::string& method, const double& args_d) {
        PyObject* pod    = Py_BuildValue("d", args_d); 
        PyObject* ret = this->call_method(module, method, pod); 
        Py_DecRef(pod);
        return ret;
    }
    PyObject* dict_set(PyObject* dict, const std::string& key, PyObject* obj){
        PyObject* pok    = Py_BuildValue("s", key.c_str()); 
        PyObject* pov    = Py_BuildValue("O", obj);
        PyDict_SetItem(dict, pok, pov);
        Py_DecRef(pok); 
        // Py_DecRef(pov); 
        return dict;
    }
    PyObject* dict_set(PyObject* dict, const std::string& key, const double& val){
        PyObject* pok    = Py_BuildValue("s", key.c_str()); 
        PyObject* pov    = Py_BuildValue("d", val);
        PyDict_SetItem(dict, pok, pov);
        Py_DecRef(pok); 
        // Py_DecRef(pov); 
        return dict;
    }
    PyObject* dict_set(PyObject* dict, const std::string& key, const std::string& val){
        PyObject* pok    = Py_BuildValue("s", key.c_str()); 
        PyObject* pov    = Py_BuildValue("s", val.c_str());
        PyDict_SetItem(dict, pok, pov);
        Py_DecRef(pok); 
        return dict;
    }
    PyObject* dict_set(PyObject* dict, const std::string& key, const double& val1, const double& val2){
        PyObject* pok     = Py_BuildValue("s", key.c_str()); 
        PyObject* pov1    = Py_BuildValue("d", val1);
        PyObject* pov2    = Py_BuildValue("d", val2);
        
        PyObject* l   = PyList_New(2);
        PyList_SetItem(l, 0, pov1);
        PyList_SetItem(l, 1, pov2);

        PyDict_SetItem(dict, pok, l);
        Py_DecRef(pok); 
        return dict;
    }

    PyObject* dict_set(PyObject* dict, const std::string& key, const bool& val){
        PyObject* pok    = Py_BuildValue("s", key.c_str()); 
        if (val){
            PyDict_SetItem(dict, pok, Py_True);
        }else{
            PyDict_SetItem(dict, pok, Py_False);
        }
        // Py_DecRef(pok); 
        return dict;
    }
    void set_attr(PyObject* module, const std::string& attr_name, const std::string& str) {
        PyObject* s = Py_BuildValue("s", str.c_str()); 

        auto rc = PyObject_SetAttrString(module, attr_name.c_str(), s);

        if (rc != 0){
            throw std::runtime_error("Attribute " + attr_name + std::string("not set."));
        } 
    }
    template<class FLOAT, 
            typename std::enable_if<
                std::is_floating_point<FLOAT>::value, 
            bool>::type = true>
    void set_attr(PyObject* module, const std::string& attr_fname, const FLOAT& f) {
        PyObject* s = Py_BuildValue("d", f); 

        auto rc = PyObject_SetAttrString(module, attr_fname.c_str(), s);

        if (rc != 0){
            throw std::runtime_error("Attribute " + attr_fname + std::string("not set."));
        } 
    }
    PyObject* get_attr(PyObject* obj, const std::string& attr_name){
        PyObject* res = PyObject_GetAttrString(obj, attr_name.c_str());
        if (!res)
            throw std::runtime_error(std::string("Couldn't find required attribute/function: ") + attr_name);
        return res;
    }
    
    std::string get_attr_as_string(PyObject* obj, const std::string& attr_name){
        PyObject* p = this->get_attr(obj, attr_name);
        PyObject* repr = PyObject_Repr(p);
        PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
        std::string res(PyBytes_AS_STRING(str));
        Py_DECREF(p);
        Py_DECREF(repr);
        Py_DECREF(str);
        return res;
    }

    int get_attr_as_int(PyObject* obj, const std::string& attr_name){
        PyObject* p = this->get_attr(obj, attr_name);
        auto l = PyLong_AsLong(p);
        Py_DECREF(p);
        return int(l);
    }

    double get_attr_as_float(PyObject* obj, const std::string& attr_name){
        PyObject* p = this->get_attr(obj, attr_name);
        auto v =  PyFloat_AS_DOUBLE(p);
        Py_DECREF(p);
        return v;
    }

    std::string& to_string(PyObject* obj) const{
        // pPO to string
        PyObject* repr = PyObject_Repr(obj);
        PyObject* cstr = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
        const char *bytes = PyBytes_AS_STRING(cstr);
        ASSERT(cstr != nullptr);
        std::string str(bytes);
        str.erase(0, str.find_first_not_of('\''));     //prefixing '\''
        str.erase(str.find_last_not_of('\'')+1);
        return str;
    }
    
    template<class CONTAINER, ENABLE_IF_1D_ARITHMATIC(CONTAINER) > 
    PyObject* to_list(const CONTAINER& arr, int jump = 0) const {
        PyObject* pl = PyList_New(0);
        int i = 0;
        for (auto& v : arr) {
            PyObject* p = Py_BuildValue("d", v);
            if (jump > 0) {
                if (i % jump == 0 && i > 0) {
                    PyList_Append(pl, Py_None);
                }
            }
            PyList_Append(pl, p);
            i++;
        }
        return pl;
    } 
    
public:
    
    PythonInterpreter(PythonInterpreter &other) = delete;
    void operator=(const PythonInterpreter &) = delete;

    static PythonInterpreter *Get();
    
};

PythonInterpreter* PythonInterpreter::pinstance_{nullptr};
std::mutex PythonInterpreter::mutex_;

PythonInterpreter *PythonInterpreter::Get()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new PythonInterpreter();
    }
    return pinstance_;
}


}


#endif