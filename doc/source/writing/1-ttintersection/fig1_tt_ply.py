#!/usr/bin/env python
# coding: utf-8

import plotly.graph_objects as go
from os.path import abspath, join, dirname
import os, sys

p = join(abspath(os.getcwd()), "./source/writing/script")
# print(p)
if not os.path.exists(p):
    p = join(abspath(dirname(dirname(__file__))), "script")
    sys.path.append(p)
else:
    sys.path.append(p)

from plotly_helper import *
# sys.path.append(p)

_PATH_THIS_ = abspath((dirname(__file__)))

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
    "yellow" : "#FBBD0C",
    "blue"   : "#4285F4" 
}

# make multiple traces
traces={}

traces["T1"] = go.Scatter3d(x = t1xyz[0], 
                            y = t1xyz[1],
                            z = t1xyz[2],
                            name='T1',
                            line=dict(
                                 color="#4285F4",
                                 width=3))

arrow0 = arrow(go, [0,0,0], t1[0], "#4285F4" )
arrow1 = arrow(go, [0,0,0], t1[1], "#4285F4" )
arrow2 = arrow(go, [0,0,0], t1[2], "#4285F4" )

arrowt0 = arrow(go, [0,0,0], t2[0], "#FBBD0C" )
arrowt1 = arrow(go, [0,0,0], t2[1], "#FBBD0C" )
arrowt2 = arrow(go, [0,0,0], t2[2], "#FBBD0C" )


traces["T1Surface"] = go.Mesh3d(x = t1xyz[0], 
                                y = t1xyz[1],
                                z = t1xyz[2],
                                opacity=0.5,
                                color  = "#4285F4")

traces["T2"]  = go.Scatter3d(x = t2xyz[0], 
                             y = t2xyz[1],
                             z = t2xyz[2],
                            name='T2',
                            line=dict(
                                     color="#FBBD0C",
                                     width=3))

traces["T2Surface"] = go.Mesh3d(x = t2xyz[0], 
                                y = t2xyz[1],
                                z = t2xyz[2],
                                opacity=0.8,
                                color  = "#FBBD0C")

# print(type(traces["T2Surface"].x))

data=list(traces.values())

data.extend(list(arrow0.values()))
data.extend(list(arrow1.values()))
data.extend(list(arrow2.values()))

data.extend(list(arrowt0.values()))
data.extend(list(arrowt1.values()))
data.extend(list(arrowt2.values()))

data.extend(list(coordinate(go).values()))

# build and plot figure
fig=go.Figure(data)

camera = dict(
    up    =dict(x=0, y=0, z=1),
    center=dict(x=0, y=-0.2, z=0.1),
    eye   =dict(x=0.8, y=-0.9, z=0.3)
)


fig.update_layout(
    scene_camera = camera,
    height       = 300,
    margin_t     = 0,
    margin_b     = 0,
    margin_r     = 10,
    margin_l     = 10,
    scene=dict(
        xaxis=dict(visible=False),
        yaxis=dict(visible=False),
        zaxis=dict(visible=False),
        annotations=[
         annote_label(t1[0][0], t1[0][1], t1[0][2], "P0", "#4285F4"),
         annote_label(t1[1][0], t1[1][1], t1[1][2], "P1", "#4285F4"),
         annote_label(t1[2][0], t1[2][1], t1[2][2], "P2", "#4285F4"),
         annote_label(t2[0][0], t2[0][1], t2[0][2], "Q0", "#FBBD0C"),
         annote_label(t2[1][0], t2[1][1], t2[1][2], "Q1", "#FBBD0C"),
         annote_label(t2[2][0], t2[2][1], t2[2][2], "Q2", "#FBBD0C"),
         annote_label(1, 0, 0, "x", "green"),
         annote_label(0, 1, 0, "y", "red"),
         annote_label(0, 0, 1, "z", "blue"),
        ]
    ),
)


# fig.write_image("fig1.png")
fig.write_html(join(_PATH_THIS_, "fig1_tt.div"),full_html=False)
# fig.show()