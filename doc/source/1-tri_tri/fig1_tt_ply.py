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
                            name='T1',
                            line=dict(
                                 color="#4285F4",
                                 width=3))

arrow0  = plh.arrow([0,0,0], t1[0], cs["blue"] )
arrow1  = plh.arrow([0,0,0], t1[1], cs["blue"] )
arrow2  = plh.arrow([0,0,0], t1[2], cs["blue"] )

arrowt0 = plh.arrow([0,0,0], t2[0], cs["yellow"] )
arrowt1 = plh.arrow([0,0,0], t2[1], cs["yellow"] )
arrowt2 = plh.arrow([0,0,0], t2[2], cs["yellow"] )


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

traces["Line"]  = go.Scatter3d(
    x=[2,2], y=[0,5], z=[1,1],
    mode='lines', name='Intersect Line',
    line=dict(color=cs["green"], width=5)
)

# Triangle 1 intersection point on Plane 2
# p0 -> p2
pp02, pd02 = ph.line_equation_3d(t1[0], t1[2])
pi = ph.line_plane_intersection(p2, pp02, pd02)
traces["pi"]  = go.Scatter3d(
    x=[pi[0]], y=[pi[1]], z=[pi[2]],
    name="i", mode='markers',
    marker =dict(size=5, color=cs["blue"])
)
pp12, pd12 = ph.line_equation_3d(t1[1], t1[2])
pj = ph.line_plane_intersection(p2, pp12, pd12)
traces["pj"]  = go.Scatter3d(
    x=[pj[0]], y=[pj[1]], z=[pj[2]],
    name="j", mode='markers',
    marker =dict(size=5, color=cs["blue"])
)
# Triangle 2 intersection point on Plane 1
qp02, qd02 = ph.line_equation_3d(t2[0], t2[2])
pk = ph.line_plane_intersection(p1, qp02, qd02)
traces["pk"]  = go.Scatter3d(
    x=[pk[0]], y=[pk[1]], z=[pk[2]],
    name="k", mode='markers',
    marker =dict(size=5, color=cs["yellow"])
)
pp12, pd12 = ph.line_equation_3d(t2[1], t2[2])
ql = ph.line_plane_intersection(p1, pp12, pd12)
traces["ql"]  = go.Scatter3d(
    x=[ql[0]], y=[ql[1]], z=[ql[2]],
    name="l", mode='markers',
    marker =dict(size=5, color=cs["yellow"])
)

data=list(traces.values())

data.extend(list(arrow0.values()))
data.extend(list(arrow1.values()))
data.extend(list(arrow2.values()))

data.extend(list(arrowt0.values()))
data.extend(list(arrowt1.values()))
data.extend(list(arrowt2.values()))

data.extend(list(plh.coordinate().values()))

# build and plot figure
fig=go.Figure(data)

camera = dict(
    up    = {"x": 0,"y": 0,"z": 1 },
    center= {"x": 0.0835682267608771,"y": -0.16778116467798726,"z": -0.10649313647557461},
    eye   = {"x": 1.0530442890234777,"y": -0.544530617157829,"z": 0.6340155268971976}
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
         plh.annote_label(1, 0, 0, "x", "green"),
         plh.annote_label(0, 1, 0, "y", "red"),
         plh.annote_label(0, 0, 1, "z", "blue"),
         plh.annote_label(2, 4, 1, r'$L$', cs["green"]),
         plh.annote_label(pi[0], pi[1], pi[2], r'$i$', cs["red"]),
         plh.annote_label(pj[0], pj[1], pj[2], r'$j$', cs["red"]),
         plh.annote_label(pk[0], pk[1], pk[2], r'$k$', cs["red"]),
         plh.annote_label(ql[0], ql[1], ql[2], r'$l$', cs["red"]),
        ]
    ),
)


if __name__ == "__main__":
    ph.generate_0_svg(_PATH_THIS_)
    html_path = abspath(join(_PATH_THIS_, "fig1_tt_ply.html"))
    fig.write_html(html_path)
    # plh.append_js_to_show_camera_info(html_path)
    fig.write_html(abspath(join(_PATH_THIS_, "fig1_tt_ply.div")), full_html=False, include_plotlyjs='cdn')
# 
