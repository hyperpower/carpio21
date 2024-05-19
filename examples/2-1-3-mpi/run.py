import os
import sys
import time

FILE_ORIGINAL = [
    "main.cpp",
    "CMakeLists.txt",
    # "plot.py",
    "run.py",
    "report.rst"
]


PATH_PYTOOLS  = os.path.abspath(os.path.join(__file__, "../../pytools"))

sys.path.append(PATH_PYTOOLS)
import runtool as RT

# import reporttool as REPORT

if __name__ == '__main__':
    args = RT.parse_args()
    runer = RT.Runer(__file__, FILE_ORIGINAL, 1, 4)
    runer.run(args)