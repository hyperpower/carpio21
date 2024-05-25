import matplotlib.pyplot as plt
import numpy as np
import os

# pytools dir
_RUNTOOL_DIR_   = os.path.abspath(os.path.join(__file__, "../"))

plt.style.use('dark_background')

def plot_test_fig():
    fig, ax = plt.subplots()

    L = 6
    x = np.linspace(0, L)
    ncolors = len(plt.rcParams['axes.prop_cycle'])
    shift = np.linspace(0, L, ncolors, endpoint=False)
    for s in shift:
        ax.plot(x, np.sin(x + s), 'o-')
    ax.set_xlabel('x-axis')
    ax.set_ylabel('y-axis')
    ax.set_title("'dark_background' style sheet")

    plt.savefig(os.path.join(_RUNTOOL_DIR_, "test_style"))


if __name__ == '__main__':
    plot_test_fig()