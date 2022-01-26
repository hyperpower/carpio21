import os
import sys

PATH_PYTOOLS  = os.path.abspath(os.path.join(__file__, "../../pytools"))
sys.path.append(PATH_PYTOOLS)
import runtool as RT

FILE_ORIGINAL = [
    "main.cpp",
    "CMakeLists.txt",
    # "plot.py",
    "run.py",
    "report.rst"
]


if __name__ == '__main__':
    runer = RT.Runer(__file__, FILE_ORIGINAL)
    runer.clean()
    runer.mkdir()
    runer.cmake()
    runer.build()
    runer.execute()
    
