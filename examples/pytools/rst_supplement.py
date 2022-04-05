import os
from jinja2 import Environment, Template 
import datetime
import runtool as RT

def generate_0_svg(p):
    fn = "0.svg"
    f = open(os.path.abspath(os.path.join(p, fn)), "w")
    f.write("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"0\" height=\"0\"/>")
    f.close()

def text_runing_time(info):
    text = """

Run time infomation:
.. table:: Summary of Running Time.
    :widths: auto
    :align: center 

    ============= =====================
     Steps         Wall Time (s)            
    ============= ====================="""

    tstr = """
     make          %s
    """ % float_sec_to_str(info["make_wall_time"]) 

    return text + tstr

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
    fstr = add_hidden_toc(info, fstr)
    fstr = add_title(info, fstr)
    
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

    return text + fstr


def add_hidden_toc(info, fstr):
    text = """
.. toctree::
    :hidden:

    report

"""
    return text + fstr


def append_wall_time_table(info, fstr):
    text = """

Run time infomation
=====================

.. table:: Summary of Running Time.
    :widths: auto
    :align: center 

    ============= =====================
     Steps         Wall Time (s)            
    ============= ====================="""

    keys = [
        ["make_wall_time",    "make"],
        ["build_wall_time",   "build"],
        ["execute_wall_time", "execute"],
    ]

    textrow ="""
    %14s %s
""" 

    for row in keys:
        k = row[0] # key
        n = row[1] # name
        tstr = float_sec_to_str(info[k])
        row.append(tstr)
        
        textrow ="""
    %-14s %s""" % (n, tstr)
        text += textrow

    text +="""
    ============= ====================="""

    return fstr + text


if __name__ == '__main__':
    print(float_sec_to_str(2))


