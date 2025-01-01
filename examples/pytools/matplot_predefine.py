import matplotlib.pyplot as plt
import numpy as np
import os
from env_para import *

plt.style.use(os.path.join(DIR_PYTOOLS, "web.mplstyle"))

def plot_test_fig():
    fig, ax = plt.subplots()

    L = 6
    x = np.linspace(0, L)
    ncolors = len(plt.rcParams['axes.prop_cycle'])
    shift = np.linspace(0, L, ncolors, endpoint=False)
    for s in shift:
        ax.plot(x, np.sin(x + s), '-')

    plt.text(1.0, 0.0, 
            r"$\frac{W_c}{W_e} (\frac{T_9}{T_6})^{0.5}=0.25$")
    plt.text(1.0, 0.5, 
            r"$\text{web.mplstyle}$")
    ax.set_xlabel('x-axis')
    ax.set_ylabel('y-axis')
    ax.set_title("web.mplstyle style sheet")

    plt.grid()

    plt.savefig(os.path.join(DIR_FIG, "test_style"))


if __name__ == '__main__':
    plot_test_fig()