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
#include <algorithm>

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

    PyObject* import(const std::string& name) const{
        PyObject* po_name = PyUnicode_FromString("name");
        PyObject* pkg     = PyImport_Import(po_name);
        Py_DECREF(po_name);
        if (!pkg) {
            PyErr_Print();
            throw std::runtime_error("Error loading module " + name + "!");
        }
    }

    PyObject* import_sub(PyObject* module, const std::string& fname) {
        PyObject* fn = PyObject_GetAttrString(module, fname.c_str());

        if (!fn)
            throw std::runtime_error(std::string("Couldn't find required function/module: ") + fname);

        if (!PyFunction_Check(fn))
            throw std::runtime_error(fname + std::string(" is unexpectedly not a PyFunction."));

        return fn;
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