import matplotlib.pyplot as plt
import os

PATH_CASES    = os.path.abspath(os.path.join(__file__, "../.."))
PATH_THIS     = os.path.abspath(os.path.join(__file__, "../"))
PATH_DATA     = os.path.abspath(os.path.join(PATH_THIS, "data"))
PATH_FIG      = os.path.abspath(os.path.join(PATH_THIS, "fig"))
PATH_PROJECT  = os.path.abspath(os.path.join(PATH_CASES, "../"))
PATH_PYTOOLS  = os.path.abspath(os.path.join(PATH_CASES, "pytools"))


plt.style.use(os.path.join(PATH_PYTOOLS, "web.mplstyle"))

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
    x_st = -0.3
    x_ed = 1.3
    y_st = -0.2
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
    line_x     = [0.0, -0.0]
    line_y     = [0.0, -0.2]
    # dash line 2
    line2_x    = [1.0, 1.0]
    line2_y    = [0.0, -0.2]

    plt.plot(box_x  , box_y, linewidth=2.0)

    plt.plot(line_x,  line_y, "--", linewidth=2.0)
    plt.plot(line2_x, line2_y, "--", linewidth=2.0, color = "C1")
    plt.plot([-0.3,0.0], [0.0,0.0] , "--", linewidth=2.0, color = "C1")
    plt.plot([-0.3,0.0], [1.0,1.0] , "--", linewidth=2.0, color = "C1")

    plt.text(0.5,  1.05, r'$\phi = 1$', ha="center", va="bottom")
    plt.text(-0.05, 0.5, r'$\phi = 0$', ha="right", va="center")
    plt.text( 1.05, 0.5,  r'$\phi = 0$', ha="left", va="center")
    plt.text( 0.5, -0.05,  r'$\phi = 0$', ha="center", va="top")
    
    plt.text( 0.5, 0.5,  r'$\Delta \phi = 0$', ha="center", va="center")

    ax.set_aspect('equal')
    plt.tight_layout()
    plt.savefig(PATH_FIG + "/illustration.png")
    plt.close()

    
def make_gif(fn_prefix, gifname):
    # make gif
    print("making gif ...")
    cmd = "{} {} {}".format(
            "ffmpeg -i ./fig/{}_%03d.png".format(fn_prefix),
            "-vf \"fps=10,scale=800:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\"",
            "-y -loop 0 ./fig/%s.gif" % gifname
        )
    os.system(cmd)
    # os.system("ffmpeg -i ./fig/lb_%02d.png -vf \"fps=10,scale=800:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\" -loop 0 ./fig/%s.gif" % (fn_prefix, filename))

    # delete files
    for file_name in os.listdir("./fig"):
        if file_name.endswith('.png') and file_name.startswith("%s_" % fn_prefix):
            # print("./fig/" + file_name)
            os.remove("./fig/" + file_name)

def main():
    plot_illustration_fig()
    make_gif("implicit_phi", "implicit_iter")

if __name__ == '__main__':
    main()
