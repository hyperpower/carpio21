import os, sys
import numpy as np
import string
import math
import operator


CURRENT = os.path.dirname(os.path.abspath(__file__))
FIG = os.path.join(CURRENT, "fig")


def main():
    # fn      = "man"
    res = []
    files = [f for f in os.listdir(FIG) if os.path.isdir(os.path.join(FIG, f))]
    for f in files:
        namedir = os.path.join(FIG, f)
        os.system("magick " + namedir + "/*.png -delay 10 -loop 0 -layers Optimize " + os.path.join(FIG, f + ".gif"))


if __name__ == '__main__':
    main()