import os
import sys
import time

FILE_ORIGINAL = [
    "main.cpp",
    "CMakeLists.txt",
    "plot.py",
    "run.py",
    "report.rst"
]

PATH_THIS     = os.path.abspath(os.path.join(__file__,   "../"))
PATH_CASES    = os.path.abspath(os.path.join(__file__,   "../.."))
PATH_PROJECT  = os.path.abspath(os.path.join(PATH_CASES, "../"))
PATH_DATA     = os.path.abspath(os.path.join(PATH_THIS,  "data"))
PATH_FIG      = os.path.abspath(os.path.join(PATH_THIS,  "fig"))
PATH_PYTOOLS  = os.path.abspath(os.path.join(__file__, "../../pytools"))

sys.path.append(PATH_PYTOOLS)
import runtool as RT

import reporttool as REPORT

def build(path):
    runtime = []
    tprev = time.time()
    print("cmake --------------------------------- ")
    # cmake ====
    os.system("mkdir data")
    os.system("mkdir fig")
    os.system("cmake .")
    runtime.append(("dt_cmake",time.time() - tprev))
    tprev = time.time()
    print("make  --------------------------------- ")
    os.system("make")
    runtime.append(("dt_make", time.time() - tprev))
    tprev = time.time()
    print("run   --------------------------------- ")
    os.system("./build/main ")
    runtime.append(("dt_run",  time.time() - tprev))
    tprev = time.time()
    print("plot   -------------------------------- ")
    os.system("python3 plot.py")
    runtime.append(("dt_plot", time.time() - tprev))
    tprev = time.time()
    print("report   ------------------------------ ")
    # os.system("python3 report.py")
    REPORT.run(path, runtime, FILE_ORIGINAL)
    return runtime

def main():
    path = RT.Path(__file__)
    RT.clean(path.this, FILE_ORIGINAL)
    rt = build(path)
    # print(rt)

if __name__ == '__main__':
    main()