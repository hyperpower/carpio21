import os
import sys
import time

PATH_PYTOOLS  = os.path.abspath(os.path.join(__file__, "../../pytools"))
sys.path.append(PATH_PYTOOLS)
import runtool as RT

FILE_ORIGINAL = [
    "main.cpp",
    "CMakeLists.txt",
    "run.py",
    "report.rst",
    "plot.py"
]

if __name__ == '__main__':
    args = RT.parse_args()
    runer = RT.Runer(__file__, FILE_ORIGINAL)
    runer.run(args)