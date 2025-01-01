import os
from jinja2 import Environment, Template 
import datetime
import runtool as RT
import platform
import cpuinfo

def generate_0_svg(p):
    fn = "0.svg"
    f = open(os.path.abspath(os.path.join(p, fn)), "w")
    f.write("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"0\" height=\"0\"/>")
    f.close()


def float_sec_to_str(sec):
    # sec in seconds
    if 0.00001 < sec < 1.0:
        return "%.2f ms" % (sec * 1000)
    elif 1.0 <= sec < 1000.0:
        return "%.2f s" % sec
    elif sec >= 1000.0:
        return "%.2e s" % sec
    else:
        return "%.2e ms" % (sec * 1000)


def revise_report_rst(dir_in, info, dir_out):
    fullname = os.path.join(dir_in, "report.rst")
    if not os.path.exists(fullname):
        print(" >! report.rst is not in " + path)
        return 0

    f = open(fullname, "r")
    
    fstr = f.read()
    f.close()

    # order is important !!!
    fstr = add_title(info, fstr)
    # fstr = add_hidden_toc(info, fstr)

    fstr = append_sys_info(fstr)
    fstr = append_wall_time_table(info, fstr)

    outname = os.path.join(dir_out, "report.rst")
    fout = open(outname, "w")
    fout.write(fstr)
    fout.close 


def add_title(info, fstr):
    # Capital first letter in a string
    cn = " ".join(map(lambda d: d.capitalize(), info["name"].split('_')))
    sl = "=" * (len(cn) + 3)
    text = """
%s
%s
%s
""" % (sl, cn, sl)

    text2 = """
.. |date| date::
.. |time| date:: %H:%M

This document was generated on |date| at |time|

"""
    return text + text2 + fstr


def add_hidden_toc(info, fstr):
    text = """
.. contents::
   :local:
   :backlinks: none

"""

#    text = """
#.. toctree::
#    :maxdepth: 3
#    :hidden:
#
#    self
#    development
#
#"""
    return text + fstr


def append_wall_time_table(info, fstr):
    text = """

Run time information
--------------------

.. csv-table:: Summary of Running Time.
    :header: "Step", "Wall Time"
    :widths: 20, 40

"""

    keys = [
        ["make_wall_time",    "make"],
        ["build_wall_time",   "build"],
        ["execute_wall_time", "execute"],
    ] 

    for row in keys:
        k = row[0] # key
        n = row[1] # name
        tstr = float_sec_to_str(info[k])
        row.append(tstr)
        
        textrow ="    %s,%s\n" % (n, tstr)
        text += textrow

    text +="\n\n"


    return fstr + text

def comma_to_space(input):
    return input.replace(",", " ")

def get_sys_info():
    d = {}
    cpu = cpuinfo.get_cpu_info()
    d["Operating System"]  = platform.platform()
    d["CPU"] = cpu["brand_raw"]
    d["CPU Architecture"] = cpu["arch"]
    d["Python Version"] = platform.python_version()
    result = os.popen("cmake --version")
    d["Cmake Version"] = result.read().split("\n")[0]
    result = os.popen("gnuplot --version")
    d["Gnuplot Version"] = result.read().split("\n")[0]
    return d

def append_sys_info(fstr):
    info = get_sys_info()
    text = """

Platform Info
==============

Syetem Enviroment Information
--------------------------------

.. csv-table:: System Infomation
   :header: "Item", "Info"
   :widths: 20, 40

"""

    for k, v in info.items():
        row = "   %s, %s\n" % (k, comma_to_space(v))
        text +=row
    
    text +="\n\n"
    
    return fstr + text; 


if __name__ == '__main__':


    for key, value in cpuinfo.get_cpu_info().items():
        print("{0}: {1}\n".format(key, value))

    print("="*40, "System Information", "="*40)
    print(append_sys_info(""))
   


