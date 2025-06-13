#!/usr/bin/env python
# coding: utf-8

import plotly.graph_objects as go
from os.path import abspath, join, dirname
import os, sys
import importlib.util

# Known that current script is in "source" folder or subfolder of "source"
_PATH_THIS_ = abspath((dirname(__file__)))
# find parent folder, which is "source"
path = os.path.abspath(_PATH_THIS_)
while True:
    if os.path.basename(path) == "source":
        break
    new_path = os.path.dirname(path)
    if new_path == os.path.abspath("/"):
        raise FileNotFoundError("Could not find 'source' folder in the path hierarchy.")
    path = new_path
_PATH_SOURCE_  = path
_PATH_DOC_     = abspath(join(_PATH_SOURCE_, ".."))
_PATH_PROJECT_ = abspath(join(_PATH_SOURCE_, "..", ".."))
_PATH_SCRIPTS_ = abspath(join(_PATH_PROJECT_, "scripts"))

sys.path.append(_PATH_SCRIPTS_)

import plot_helper as ph
import plotly_helper as plh
# sys.path.append(p)


# two triangles in 3D space
# t1 is the first triangle, t2 is the second triangle
t1 = [
    [1, 1, 1],
    [1, 4, 1],
    [4, 1.5, 1],
]

t2 = [
    [1,   1,   2  ],
    [1,   4,   2],
    [3, 3.5,   0],
]

t1xyz   = ph.transpose(t1)
t2xyz   = ph.transpose(t2)

p1 = ph.plane_equation(t1[0], t1[1], t1[2])
p2 = ph.plane_equation(t2[0], t2[1], t2[2])

pp0, pd = ph.plane_intersection_line(p1, p2)
pp1     = ph.point_at_distance(pp0, pd, 5)

print("Intersection Point: ", pp0)
print("Intersection Direction: ", pd)

print("Plane 1: ", p1)
print("Plane 2: ", p2)

fig = go.Figure()

cs = {
    "yellow" : "#FBBD0C",
    "blue"   : "#4285F4",
    "red"    : "#F35325",
    "green"  : "#81BC06",
}

# make multiple traces
traces={}

traces["T1"] = go.Scatter3d(x = t1xyz[0], 
                            y = t1xyz[1],
                            z = t1xyz[2],
                            name='U',
                            line=dict(
                                 color="#4285F4",
                                 width=3))


traces["T1Surface"] = go.Mesh3d(x = t1xyz[0], 
                                y = t1xyz[1],
                                z = t1xyz[2],
                                opacity=0.5,
                                color  = "#4285F4")

traces["T2"]  = go.Scatter3d(x = t2xyz[0], 
                             y = t2xyz[1],
                             z = t2xyz[2],
                            name='V',
                            line=dict(
                                     color="#FBBD0C",
                                     width=3))

traces["T2Surface"] = go.Mesh3d(x = t2xyz[0], 
                                y = t2xyz[1],
                                z = t2xyz[2],
                                opacity=0.8,
                                color  = "#FBBD0C")



data=list(traces.values())

data.extend(list(plh.coordinate(go).values()))

# build and plot figure
fig=go.Figure(data)

camera = dict(
    up    = {"x": 0,"y": 0,"z": 1 },
    center= {"x": 0.06555170705854932,
"y": 0.09590046995309226,
"z": 0.005677779257753759},
    eye   = {"x": 0.9845245216169527,
"y": -0.4438988657389967,
"z": 0.49982457626685406}
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
         plh.annote_label(t1[0][0], t1[0][1], t1[0][2], r'$P_0$', "#4285F4"),
         plh.annote_label(t1[1][0], t1[1][1], t1[1][2], r'$P_1$', "#4285F4"),
         plh.annote_label(t1[2][0], t1[2][1], t1[2][2], r'$P_2$', "#4285F4"),
         plh.annote_label(t2[0][0], t2[0][1], t2[0][2], r'$Q_0$', "#FBBD0C"),
         plh.annote_label(t2[1][0], t2[1][1], t2[1][2], r'$Q_1$', "#FBBD0C"),
         plh.annote_label(t2[2][0], t2[2][1], t2[2][2], r'$Q_2$', "#FBBD0C"),
         plh.annote_label(0.9, 0, 0, "x", "green"),
         plh.annote_label(0, 0.9, 0, "y", "red"  ),
         plh.annote_label(0, 0, 0.9, "z", "blue" ),
        ]
    ),
)


if __name__ == "__main__":
    ph.generate_0_svg(_PATH_THIS_)
    html_path = abspath(join(_PATH_THIS_, "fig1_ttdefine_ply.html"))
    fig.write_html(html_path)
    plh.append_js_to_show_camera_info(html_path)
    fig.write_html(abspath(join(_PATH_THIS_, "fig1_ttdefine_ply.div")), full_html=False, include_plotlyjs='cdn')
# 
