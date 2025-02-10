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
PATH_FIG      = os.path.abspath(os.path.join(PATH_THIS, "fig"))
PATH_PYTOOLS  = os.path.abspath(os.path.join(PATH_CASES, "pytools"))

plt.style.use(os.path.join(PATH_PYTOOLS, "web.mplstyle"))
#
sys.path.append(PATH_PYTOOLS)
import runtool as RT


def plot_illustration_fig():
    plt.figure(figsize=(6, 5))
    ax = plt.axes()

    """
    Set labels
    """
    plt.xlabel(r'x')
    plt.ylabel(r'y')

    """
    Set range
    """
    x_st = -0.4
    x_ed = 1.4
    y_st = -0.3
    y_ed = 1.3

    plt.xlim([x_st, x_ed])
    plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')

    """
    Data part
    """
    box_x      = [0.0, 1.0, 1.0, 0.0, 0.0]
    box_y      = [0.0, 0.0, 1.0, 1.0, 0.0]
    # dash line
    line_x     = [0.0, 1.0]
    line_y     = [0.0, 1.0]
    # dash line 2
    line2_x    = [0.0,       1.0 - (1.0/6.0)]
    line2_y    = [(1.0/6.0), 1.0]
    # dash line 2
    line3_x    = [(1.0/6.0), 1.0]
    line3_y    = [0.0, 1.0 - (1.0/6.0)]

    arrx_start = [x_st, 0.25, 0.25, 0.75, 0.75, x_ed] 
    arr_v      = [0.0,  0.0,  1.0,  1.0 , 0.0,  0.0]
    arrx_end   = [x_st, 1.25, 1.25, 1.75, 1.75, x_ed]    

    plt.plot(box_x  , box_y, linewidth=2.0)

    plt.plot(line_x,   line_y, "--", linewidth=2.0)
    plt.plot(line2_x, line2_y, "--", linewidth=2.0, color = "C1")
    plt.plot(line3_x, line3_y, "--", linewidth=2.0, color = "C1")

    # the 1/6 ticky
    ticky = 1.0/6.0
    plt.plot([-0.05, 0.0], [ticky, ticky], "-", linewidth=2.0, color = "C0")
    plt.text( -0.2, ticky, "1/6", color = "C0", va = "center")
    
    plt.text(-0.4, .08, r'$\sqrt{1 - (6 * y)^2}$', va="center")

    plt.plot([ticky, ticky], [-0.05, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text( ticky, -0.14, "1/6", color = "C0",   ha="center")

    plt.text(-0.3, -.1, r'$\sqrt{1 - (6 * x)^2}$', va="center")

    plt.text(0.25, 1.05, r'$\frac{d \phi}{d y}$ = ' + "%.f" % float(0.0))
    plt.text(0.25,-0.15, r'$\phi$ = ' + "%.1f" % float(0.0))
    plt.text(-0.35, .6,  r'$\phi$ = ' + "%.1f" % float(0.0), va="center")
    plt.text(1.1,  .5,   r'$\frac{d \phi}{d x}$ = ' + "%.f" % float(0.0))

    vv = {"x" : 0.6, "y" : 0.1}
    plt.arrow(vv["x"],        vv["y"], 0.2, 0.0, width=0.01, color = "k")
    plt.text( vv["x"] + 0.06, vv["y"] - 0.07, "u = 1.0")
    plt.arrow(vv["x"],        vv["y"], 0.0, 0.2, width=0.01, color = "k")
    plt.text( vv["x"] - 0.1,  vv["y"] + 0.25, "v = 1.0")
    plt.arrow(vv["x"],        vv["y"], 0.2, 0.2, width=0.01, color = "r")

    plt.text( 0.75, 0.45, r'$\phi=0.0$')
    plt.text( 0.1, 0.75, r'$\phi=0.0$')

    # plot cut
    xcut = 0.7
    plt.arrow(xcut,  1.1,  0.1, 0.0, head_width=0.05, color = "k")
    plt.arrow(xcut,  -0.1, 0.1, 0.0, head_width=0.05, color = "k")
    plt.plot([xcut, xcut], [1.1, 1.05], color = "k")
    plt.plot([xcut, xcut], [-0.1, -.05], color = "k")
    plt.text(xcut + 0.2,  1.1, r'A-A (x = 0.7)', va = "center")
    plt.text(xcut + 0.2, -0.1, r'A-A (x = 0.7)', va = "center")
    # plt.legend(llg, scheme, loc= 'upper right')

    # plt.grid(True)
    ax.set_aspect('equal')
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/illustration.png")
    plt.close()
    # plt.show()


def main():
    plot_illustration_fig()
    scheme = ["fou", "Superbee", "Minmod"]
    for s in scheme:
        prefix = s + "_phi"
        RT.make_gif(prefix, s + "_iter")
        RT.rename_last_png(prefix)
        RT.delete_png_series(prefix)
    
    

if __name__ == '__main__':
    main()


