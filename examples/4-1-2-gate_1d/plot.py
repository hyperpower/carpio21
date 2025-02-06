import matplotlib
import matplotlib.pyplot as plt
import os, sys
import numpy as np
import string
import math
import operator

import multiprocessing
from multiprocessing import Pool
PATH_CASES    = os.path.abspath(os.path.join(__file__, "../.."))
PATH_THIS     = os.path.abspath(os.path.join(__file__, "../"))
PATH_DATA     = os.path.abspath(os.path.join(PATH_THIS, "data"))
PATH_FIG      = os.path.abspath(os.path.join(PATH_THIS, "fig"))
PATH_PROJECT  = os.path.abspath(os.path.join(PATH_CASES, "../"))
PATH_PYTOOLS  = os.path.abspath(os.path.join(PATH_CASES, "pytools"))

plt.style.use(os.path.join(PATH_PYTOOLS, "web.mplstyle"))
#
sys.path.append(PATH_PYTOOLS)
import runtool as RT


def plot_illustration_fig():
    plt.figure(figsize=(6, 4))

    """
    Set labels
    """
    plt.xlabel(r'x')
    plt.ylabel(r'$\phi$')

    """
    Set range
    """
    x_st = 0
    x_ed = 2
    y_st = -0.25
    y_ed = 1.25

    plt.xlim([x_st, x_ed])
    plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')

    """
    Data part
    """
    arrx_start = [x_st, 0.25, 0.25, 0.75, 0.75, x_ed] 
    arr_v      = [0.0,  0.0,  1.0,  1.0 , 0.0,  0.0]
    arrx_end   = [x_st, 1.25, 1.25, 1.75, 1.75, x_ed]    

    plt.plot(arrx_end  , arr_v, linewidth=2.0)
    plt.plot(arrx_start, arr_v, linewidth=3.0)

    plt.text(0.25, 1.05, "Time = "+ "%.2f" % float(0.0))
    plt.text(1.25, 1.05, "Time = "+ "%.2f" % float(1.0))

    plt.annotate('Initial profile', 
            xy=(0.26, 0.75), xytext=(0.55, 0.85),
            arrowprops = dict(facecolor='black', shrink = 0.05),
            )
    plt.annotate('Advected profile', 
            xy=(1.75, 0.35), xytext=(0.9, 0.2),
            arrowprops = dict(facecolor='black', shrink = 0.05),
            )
    plt.arrow(0.4, 0.5, 0.2, 0.0, width=0.01, color = "k")

    plt.text(0.42, 0.55, "u = 1.0")

    # plt.legend(llg, scheme, loc= 'upper right')

    plt.grid(True)
    #plt.axes().set_aspect('equal')
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/illustration.png")
    plt.close()
    # plt.show()

def main():
    plot_illustration_fig()
    RT.make_gif("fou_phi", "fou_iter")
    RT.make_gif("QUICK_phi", "QUICK_iter")
    RT.rename_last_png("fou_phi")
    RT.rename_last_png("QUICK_phi")
    RT.delete_png_series("fou_phi")
    RT.delete_png_series("QUICK_phi")
    

if __name__ == '__main__':
    main()


