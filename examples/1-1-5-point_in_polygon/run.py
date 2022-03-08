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

PATH_THIS     = os.path.abspath(os.path.join(__file__,   "../"))
PATH_CASES    = os.path.abspath(os.path.join(__file__,   "../.."))
PATH_PROJECT  = os.path.abspath(os.path.join(PATH_CASES, "../"))
PATH_DATA     = os.path.abspath(os.path.join(PATH_THIS,  "data"))
PATH_FIG      = os.path.abspath(os.path.join(PATH_THIS,  "fig"))
PATH_PYTOOLS  = os.path.abspath(os.path.join(__file__, "../../pytools"))

sys.path.append(PATH_PYTOOLS)
import runtool as RT

# import reporttool as REPORT


if __name__ == '__main__':
    args = RT.parse_args()
    runer = RT.Runer(__file__, FILE_ORIGINAL)
    # runer.run(args)
    # runer.clean()
    # runer.mkdir_all()
    # runer.cmake()
    # runer.build()
    # runer.execute()
    runer.build_doc() #