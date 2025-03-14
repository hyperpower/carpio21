import os
import sys
import time

FILE_ORIGINAL = [
    "main.cpp",
    "CMakeLists.txt",
    # "plot.py",
    "fv.h",
    "fd.h",
    "run.py",
    "report.rst"
]


PATH_PYTOOLS  = os.path.abspath(os.path.join(__file__, "../../pytools"))

sys.path.append(PATH_PYTOOLS)
import runtool as RT

if __name__ == '__main__':
    args = RT.parse_args()
    runer = RT.Runer(__file__, FILE_ORIGINAL)
    runer.run(args)