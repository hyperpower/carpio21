import os
from jinja2 import Environment, Template 
import datetime
import runtool as RT


def text_runing_time():
    text = """

Run time infomation:
.. table:: Summary of Running Time.
   :widths: auto
   :align: center 

   ============= =====================
    Functions      Wall Time (s)            
   ============= ====================="""

    return text

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

if __name__ == '__main__':
    print(float_sec_to_str(2))


