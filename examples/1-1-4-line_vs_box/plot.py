import matplotlib
import matplotlib.pyplot as plt
import os, sys
import numpy as np

import multiprocessing
from multiprocessing import Pool

# matplotlib.rcParams['text.usetex']        = True
# matplotlib.rcParams['font.family']        = 'serif'
# matplotlib.rcParams['font.size']          = 14

PATH_CASES    = os.path.abspath(os.path.join(__file__, "../.."))
PATH_THIS     = os.path.abspath(os.path.join(__file__, "../"))
PATH_DATA     = os.path.abspath(os.path.join(PATH_THIS, "data"))
PATH_FIG      = os.path.abspath(os.path.join(PATH_THIS, "fig"))
PATH_PROJECT  = os.path.abspath(os.path.join(PATH_CASES, "../"))
PATH_PYTOOLS  = os.path.abspath(os.path.join(PATH_CASES, "pytools"))

plt.style.use(os.path.join(PATH_PYTOOLS, "web.mplstyle"))
# sys.path.append(PATH_PYTOOLS)
# import filetool as FT

# plot figure for illustration
def figure_1():
    # plt.figure(figsize=(6,6))
    frame = plt.gca()
    ax = plt.axes()
    ax.set_aspect("equal")
    plot_box_line(ax, box, line)

    plot_annotation(plt)

    plt.axis('off')
    plt.savefig(PATH_FIG + "/" +"illustration")


box = np.array([[0.0, 0.0],
                [1.0, 0.0],
                [1.0, 1.0],
                [0.0, 1.0],
                [0.0, 0.0]])

line = np.array([[0.2,1.2],[0.8,-0.3]])


def plot_box_line(ax, box, line):
    ax.plot(box[:,0], box[:,1],"-")
    ax.plot(line[:,0], line[:,1])
    
def plot_annotation(plt):
    arrowprops = dict(arrowstyle = "->",
                      connectionstyle = "arc3")
    plt.annotate("Box", xy        =(1.0, 0.9), 
                        xytext    =(1.2, 1.0),
                        va="center", 
                        ha="center",
                        arrowprops=arrowprops)

    plt.annotate("Line", xy       =(0.4, 0.7), 
                        xytext    =(0.5, 0.8),
                        va="center", 
                        ha="center",
                        arrowprops=arrowprops)

def make_gif(fn_prefix, filename):
    # make gif
    print("making gif ...")
    cmd = "{} {} {}".format(
            "ffmpeg -i ./fig/{}_%02d.png".format(fn_prefix),
            "-vf \"fps=10,scale=800:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\"",
            "-y -loop 0 ./fig/%s.gif" % filename
        )
    os.system(cmd)
    # os.system("ffmpeg -i ./fig/lb_%02d.png -vf \"fps=10,scale=800:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\" -loop 0 ./fig/%s.gif" % (fn_prefix, filename))

    # delete files
    for file_name in os.listdir("./fig"):
        if file_name.endswith('.png') and file_name.startswith("%s_" % fn_prefix):
            # print("./fig/" + file_name)
            os.remove("./fig/" + file_name)
    # files = FT.select_files1(PATH_FIG, fn_prefix)
    # for file in files:
    #     os.system("rm " + PATH_FIG + "/" + file)

def main():
    figure_1()
    make_gif("lb", "lb")

if __name__ == '__main__':
    main()