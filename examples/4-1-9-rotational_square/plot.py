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
    return (-(y-40)*2*math.pi/360.0, (x-40)*2*math.pi/360.0)

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
    x_st = -15
    x_ed = 90
    y_st = -10
    y_ed = 90

    plt.xlim([x_st, x_ed])
    plt.ylim([y_st, y_ed])
    #plt.xscale('log')
    #plt.yscale('log')

    """
    Data part
    """
    box_x      = [ 0.0, 80.0, 80.0, 0.0 , 0.0]
    box_y      = [ 0.0, 0.0,  80.0, 80.0, 0.0]

    cx = 20
    cy = 40
    hl = 4  

    ib_x      = [ cx - hl, cx + hl, cx + hl, cx - hl , cx - hl]
    ib_y      = [ cy - hl, cy - hl, cy + hl, cy + hl , cy - hl]

    plt.plot(ib_x, ib_y, linewidth=2.0, color = "k")

    arrx_start = [x_st, 0.25, 0.25, 0.75, 0.75, x_ed] 
    arr_v      = [0.0,  0.0,  1.0,  1.0 , 0.0,  0.0]
    arrx_end   = [x_st, 1.25, 1.25, 1.75, 1.75, x_ed]    

    plt.plot(box_x  , box_y, linewidth=2.0)

    add_stream_line(ax, 20.0, 40.0, 100)
    # add_stream_line(ax, -0.6, 0.0, 100)
    # add_stream_line(ax, -0.4, 0.0, 100)
    # add_stream_line(ax, -0.2, 0.0, 100)

    # the 1/6 ticky
    ticky = 20
    plt.plot([ticky, ticky],[-2, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text(ticky, -5.0, "%.1f" % ticky, color = "C0", ha="center",va = "center")
    ticky = 16 
    plt.plot([ticky, ticky],[2, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text(ticky, 5.0, "%.f" % ticky, color = "C0", ha="center",va = "center")
    ticky = 24 
    plt.plot([ticky, ticky],[2, 0.0], "-", linewidth=2.0, color = "C0")
    plt.text(ticky, 5.0, "%.f" % ticky, color = "C0", ha="center",va = "center")

    ticky = 36
    plt.plot([2, 0.0], [ticky, ticky], "-", linewidth=2.0, color = "C0")
    plt.text( 8.0, ticky, "%.1f" % ticky, color = "C0", ha="center",va = "center")
    ticky = 44 
    plt.plot([2, 0.0], [ticky, ticky], "-", linewidth=2.0, color = "C0")
    plt.text( 8.0, ticky, "%.1f" % ticky, color = "C0", ha="center",va = "center")

    ticky = 40
    plt.plot([-2, 0.0], [ticky, ticky], "-", linewidth=2.0, color = "C0")
    plt.text( -8.0, ticky, "%.1f" % ticky, color = "C0", ha="center",va = "center")

    vv = {"x" : 40, "y" : 70}
    plt.text( vv["x"], vv["y"], r'$u=-(y-40)\frac{2 \pi}{360}$', ha="center")
    vv = {"x" : 40, "y" : 60}
    plt.text( vv["x"], vv["y"], r'$v=(x-40)\frac{2 \pi}{360}$', ha="center")
    

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


