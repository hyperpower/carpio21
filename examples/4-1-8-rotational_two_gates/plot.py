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


def velocity(x, y):
    return (y, -x)

def add_stream_line(ax, xst, yst, step=10):
    dt   = 0.01
    arrx = [xst]
    arry = [yst]

    for i in range(0,step):
        x   = arrx[i]
        y   = arry[i]
        u,v = velocity(x, y)
        nx  = x + u * dt 
        ny  = y + v * dt 
        arrx.append(nx)
        arry.append(ny)

    ax.plot(arrx, arry, linewidth=2.0, color = "C1")


def plot_illustration_fig():
    plt.figure(figsize=(8, 5))
    ax = plt.axes()

    """
    Set labels
    """
    plt.xlabel(r'x')
    plt.ylabel(r'y')

    """
    Set range
    """
    x_st = -1.4
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
    box_x      = [-1.0, 1.0, 1.0, -1. ,-1.0]
    box_y      = [ 0.0, 0.0, 1.0,  1.0, 0.0]

    arrx_start = [x_st, 0.25, 0.25, 0.75, 0.75, x_ed] 
    arr_v      = [0.0,  0.0,  1.0,  1.0 , 0.0,  0.0]
    arrx_end   = [x_st, 1.25, 1.25, 1.75, 1.75, x_ed]    

    plt.plot(box_x  , box_y, linewidth=2.0)

    arrx = np.linspace(-0.9,0.9,10)
    arry = np.linspace( 0.1,0.9,10)
    add_stream_line(ax, -0.8, 0.0, 100)
    add_stream_line(ax, -0.6, 0.0, 100)
    add_stream_line(ax, -0.4, 0.0, 100)
    add_stream_line(ax, -0.2, 0.0, 100)
    # add_quiver(ax, arrx, arry)

    # the 1/6 ticky
    ticky = -0.8
    plt.plot([ticky, ticky],[-0.05, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text(ticky, -0.12, "%.1f" % ticky, color = "C0", ha="center",va = "center")

    ticky = -0.6
    plt.plot([ticky, ticky],[-0.05, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text(ticky, -0.12, "%.1f" % ticky, color = "C0", ha="center",va = "center")
    # plt.plot([ticky, ticky], [-0.05, 0.0], "-", linewidth=2.0, color = "C0")
    # plt.text( ticky, -0.14, "1/6", color = "C0",   ha="center")
    ticky = -0.4
    plt.plot([ticky, ticky],[-0.05, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text(ticky, -0.12, "%.1f" % ticky, color = "C0", ha="center",va = "center")

    ticky = -0.2
    plt.plot([ticky, ticky],[-0.05, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text(ticky, -0.12, "%.1f" % ticky, color = "C0", ha="center",va = "center")

    ticky = 0.0
    plt.plot([ticky, ticky],[-0.08, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text(ticky, -0.13, "%.1f" % ticky, color = "C0", ha="center",va = "center")


    plt.text(0.,   1.05, r'$\frac{d \phi}{d y}$ = ' + "%.f" % float(0.0), ha="center")
    plt.text(0.5, -0.15, r'$\frac{d \phi}{d y}$ = ' + "%.1f" % float(0.0), ha="center")
    plt.text(-0.9,-0.25, r'$\phi$=' + "%.f" % float(0.0), ha="center")
    plt.text(-0.7,-0.25, r'$\phi$=' + "%.f" % float(1.0), ha="center")
    plt.text(-0.5,-0.25, r'$\phi$=' + "%.f" % float(0.0), ha="center")
    plt.text(-0.3,-0.25, r'$\phi$=' + "%.f" % float(1.0), ha="center")
    plt.text(-0.1,-0.25, r'$\phi$=' + "%.f" % float(0.0), ha="center")
    plt.text(-1.35,.5,   r'$\frac{d \phi}{d x}$ = ' + "%.1f" % float(0.0), va="center")
    plt.text( 1.05,.5,   r'$\frac{d \phi}{d x}$ = ' + "%.f" % float(0.0))

    vv = {"x" : 0.0, "y" : 0.7}
    plt.text( vv["x"], vv["y"], "u=y, v=-x", fontsize = 'large', ha="center")
    

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


