#!/usr/bin/env python
# coding: utf-8

import plotly.graph_objects as go
from os.path import abspath, join, dirname
import os

# import add_path
# cwd = os.getcwd()
# print(cwd)
# from plotly_helper import *


# data part
t1 = [
    [3, 1, 1],
    [1, 4, 1],
    [1, 1, 1],
]

t2 = [
    [2,   2.5, 2  ],
    [-2,  2,   1.3],
    [1.5, 2.5, -1 ],
]

t1xyz = to_xyz(t1[0],t1[1],t1[2])
t2xyz = to_xyz(t2[0],t2[1],t2[2])

fig = go.Figure()

cs = {
    "blue" : '#4285F4' 
}

fig.add_trace(go.Scatter3d(
    x = t1xyz[0], 
    y = t1xyz[1],
    z = t1xyz[2],
    name='T1',
    line=dict(
        color= cs["blue"],
        width= 3)
))

fig.update_layout(
    margin_t=0,
    margin_b=0,
    height  = 300 )

fig.show()