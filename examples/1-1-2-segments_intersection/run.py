import os
import sys

PATH_PYTOOLS  = os.path.abspath(os.path.join(__file__, "../../pytools"))
sys.path.append(PATH_PYTOOLS)
import runtool as RT

FILE_ORIGINAL = [
    "main.cpp",
    "two_segments.hpp",
    "multi_seg_simple.hpp",
    "segment_with_name.hpp",
    "CMakeLists.txt",
    "run.py",
    "report.rst",
    # "ben-ott"
]

if __name__ == '__main__':
    args = RT.parse_args()
    runer = RT.Runer(__file__, FILE_ORIGINAL)
    runer.run(args)