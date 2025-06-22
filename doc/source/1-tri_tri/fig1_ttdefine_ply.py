#!/usr/bin/env python
# coding: utf-8

import plotly.graph_objects as go
from os.path import abspath, join, dirname
import os, sys
import importlib.util
import numpy as np

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
_PATH_THIS_FIG_ = abspath(join(_PATH_THIS_, "fig"))

if not os.path.exists(_PATH_THIS_FIG_):
    os.makedirs(_PATH_THIS_FIG_)

sys.path.append(_PATH_SCRIPTS_)

import plot_helper as ph
import plotly_helper as plh
# sys.path.append(p)


# two triangles in 3D space
# t1 is the first triangle, t2 is the second triangle
t1 = [
    [0.5,   0.5,   2.5],
    [0.5,   4.5,   2.5],
    [3, 3.5,   0],
]

t2 = [
    [1, 1, 1],
    [1, 4, 1],
    [4, 1.5, 1],
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

camera = dict(
    up    = {"x": 0,"y": 0,"z": 1 },
    center= {"x": 0.06555170705854932,
"y": 0.09590046995309226,
"z": 0.005677779257753759},
    eye   = {"x": 0.9845245216169527,
"y": -0.4438988657389967,
"z": 0.49982457626685406}
)

def make_triangle_trace(triangle, _name, _color):
    tri_xyz = ph.transpose(triangle)
    traces={}

    traces[_name] = go.Scatter3d(x = tri_xyz[0], 
                            y = tri_xyz[1],
                            z = tri_xyz[2],
                            name= _name,
                            line=dict(
                                 color= _color,
                                 width=3))

    traces[_name +"_Surface"] = go.Mesh3d(x = tri_xyz[0], 
                                y = tri_xyz[1],
                                z = tri_xyz[2],
                                opacity=0.5,
                                color  = _color)
    return traces

def make_tri_annotation(t1, _name, _color):
    anno = []
    anno.append(plh.annote_label(t1[0][0], t1[0][1], t1[0][2], r'$' + _name + r'_0$', _color))
    anno.append(plh.annote_label(t1[1][0], t1[1][1], t1[1][2], r'$' + _name + r'_1$', _color))
    anno.append(plh.annote_label(t1[2][0], t1[2][1], t1[2][2], r'$' + _name + r'_2$', _color))
    return anno

def fig_triangle_define(figname, t1, t2):
    trace1 = make_triangle_trace(t1, "U", "#4285F4")
    trace2 = make_triangle_trace(t2, "V", "#FBBD0C")

    data=list(trace1.values())
    data.extend(list(trace2.values()))
    data.extend(list(plh.coordinate().values()))

    # build and plot figure
    fig=go.Figure(data)

    anno = make_tri_annotation(t1, "P", "#4285F4")
    anno.extend(make_tri_annotation(t2, "Q", "#FBBD0C"))
    anno.extend(plh.annote_coordinate_label())

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
            annotations=anno
        ),
    )
    # output figure
    html_path = abspath(join(_PATH_THIS_FIG_, figname + ".html"))
    fig.write_html(html_path, include_plotlyjs='cdn', include_mathjax='cdn') 
    plh.append_js_to_show_camera_info(html_path)
    fig.write_html(abspath(join(_PATH_THIS_FIG_, figname + ".div" )), full_html=False, include_plotlyjs='cdn')


def fig_t1_plane2(figname, t1, t2):
    trace1 = make_triangle_trace(t1, "U", "#4285F4")
    trace2 = make_triangle_trace(t2, "V", "#FBBD0C")
    

    # plane 2
    p2 = ph.plane_equation(t2[0], t2[1], t2[2])
    pc = ph.avg_coordinate(t2)
    n =  ph.normalize_vector(p2[:3])
    rect = ph.rectangle_from_center_and_normal(pc, n, 2.8)
    trace_rect = plh.rectangle_surface("Plane2", pc, n, 5.0,"#FBBD0C")    

    data=list(trace1.values())
    data.extend(list(trace2.values()))
    data.extend(list(trace_rect.values()))  # add rectangle surface
    # data.extend(list(plh.arrow(pc, ph.add(pc, n), "#4285F4").values()))
    data.extend(list(plh.coordinate().values()))
    
    fig=go.Figure(data)

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
            annotations= 
                make_tri_annotation(t1, "P", ph.Colors["black"]) 
                + make_tri_annotation(t2, "Q", ph.Colors["black"])
                + plh.annote_coordinate_label()
                + [plh.annote_label(rect[0][0], rect[0][1], rect[0][2], r'$\pi_2$', ph.Colors["black"])]
        ),
    )

    html_path = abspath(join(_PATH_THIS_FIG_, figname + ".html"))
    fig.write_html(html_path, include_plotlyjs='cdn', include_mathjax='cdn') 
    # plh.append_js_to_show_camera_info(html_path)
    fig.write_html(abspath(join(_PATH_THIS_FIG_, figname + ".div" )), full_html=False, include_plotlyjs='cdn')

def fig_t1_plane2_distance(figname, t1, t2):
    trace1 = make_triangle_trace(t1, "U", "#4285F4")
    trace2 = make_triangle_trace(t2, "V", "#FBBD0C")

    # plane 2
    p2  = ph.plane_equation(t2[0], t2[1], t2[2])
    pc2 = ph.avg_coordinate(t2)
    n   = ph.normalize_vector(p2[:3])
    rect = ph.rectangle_from_center_and_normal(pc2, n, 2.8)
    trace_rect = plh.rectangle_surface("Plane2", pc2, n, 5.0,"#FBBD0C")    

    pj0 = ph.point_projection_to_plane(t1[0], p2)
    pj1 = ph.point_projection_to_plane(t1[1], p2)
    pj2 = ph.point_projection_to_plane(t1[2], p2)

    pj0m = ph.mid(pj0, t1[0])
    pj1m = ph.mid(pj1, t1[1])
    pj2m = ph.mid(pj2, t1[2])

    data=list(trace1.values())
    data.extend(list(trace2.values()))
    data.extend(list(trace_rect.values()))  # add rectangle surface
    # data.extend(list(plh.arrow(pc, ph.add(pc, n), "#4285F4").values()))
    data.extend(list(plh.coordinate().values()))
    data.extend(plh.arrow(t1[0], pj0, ph.Colors["blue"]).values())
    data.extend(plh.arrow(t1[1], pj1, ph.Colors["blue"]).values())
    data.extend(plh.arrow(t1[2], pj2, ph.Colors["blue"]).values())
    data.extend(plh.arrow(pc2, pc2 + np.array(n), ph.Colors["black"]).values())


    fig=go.Figure(data)

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
            annotations= 
                make_tri_annotation(t1, "P", ph.Colors["black"]) 
                + make_tri_annotation(t2, "Q", ph.Colors["black"])
                + plh.annote_coordinate_label()
                + [plh.annote_label(rect[0][0], rect[0][1], rect[0][2], r'$\pi_2$', ph.Colors["black"])]
                + [plh.annote_label(pj0m[0], pj0m[1], pj0m[2], r'$D_0$', ph.Colors["black"])]
                + [plh.annote_label(pj1m[0], pj1m[1], pj1m[2], r'$D_1$', ph.Colors["black"])]
                + [plh.annote_label(pj2m[0], pj2m[1], pj2m[2], r'$D_2$', ph.Colors["black"])]
                + [plh.annote_label((pc2 + np.array(n))[0],
                                    (pc2 + np.array(n))[1],
                                    (pc2 + np.array(n))[2], r'$n_2$', ph.Colors["black"])]
        ),
    )

    html_path = abspath(join(_PATH_THIS_FIG_, figname + ".html"))
    fig.write_html(html_path, include_plotlyjs='cdn', include_mathjax='cdn') 
    # plh.append_js_to_show_camera_info(html_path)
    fig.write_html(abspath(join(_PATH_THIS_FIG_, figname + ".div" )), full_html=False, include_plotlyjs='cdn')

def fig_t1_plane2_similar(figname, t1, t2):
    trace1 = make_triangle_trace(t1, "U", "#4285F4")
    trace2 = make_triangle_trace(t2, "V", "#FBBD0C")

    

    # plane 2
    p2  = ph.plane_equation(t2[0], t2[1], t2[2])
    pc2 = ph.avg_coordinate(t2)
    n   = ph.normalize_vector(p2[:3])
    rect = ph.rectangle_from_center_and_normal(pc2, n, 2.8)
    trace_rect = plh.rectangle_surface("Plane2", pc2, n, 5.0,"#FBBD0C")    

    pj0 = ph.point_projection_to_plane(t1[0], p2)
    pj1 = ph.point_projection_to_plane(t1[1], p2)
    pj2 = ph.point_projection_to_plane(t1[2], p2)

    # plane 1
    p1  = ph.plane_equation(t1[0], t1[1], t1[2])
    pc1 = ph.avg_coordinate(t1)
    n1  = ph.normalize_vector(p1[:3])

    #intersection point
    e2 = np.array(t1[2]) - np.array(t1[0])
    ip = ph.intersection_line_plane(t1[0], e2, p2)
    t  = np.cross(n, n1)

    pjl0 = ph.point_projection_to_line(t1[0], ip, t)
    pjl2 = ph.point_projection_to_line(t1[2], ip, t)
     
    

    pj0m = ph.mid(pj0, t1[0])
    pj1m = ph.mid(pj1, t1[1])
    pj2m = ph.mid(pj2, t1[2])

    data=list(trace1.values())
    data.extend(list(trace2.values()))
    data.extend(list(trace_rect.values()))  # add rectangle surface
    # data.extend(list(plh.arrow(pc, ph.add(pc, n), "#4285F4").values()))
    data.extend(list(plh.coordinate().values()))
    data.extend(plh.arrow(t1[0], pj0, ph.Colors["blue"]).values())
    data.extend(plh.arrow(t1[1], pj1, ph.Colors["blue"]).values())
    data.extend(plh.arrow(t1[2], pj2, ph.Colors["blue"]).values())
    data.extend(plh.arrow(pc2, pc2 + np.array(n), ph.Colors["black"]).values())
    data.extend(plh.dash_segment(pj0, pjl0, ph.Colors["blue"]).values())
    data.extend(plh.dash_segment(pj2, pjl2, ph.Colors["blue"]).values())
    data.extend(plh.dash_segment(pjl2, ip, ph.Colors["blue"]).values())
    data.extend(plh.dash_segment(pjl0, ip, ph.Colors["blue"]).values())
    data.extend(plh.dash_segment(pj0, ip, ph.Colors["blue"]).values())
    data.extend(plh.dash_segment(pj2, ip, ph.Colors["blue"]).values())
    data.extend(plh.line(ip, t, 3, ph.Colors["red"]).values())


    fig=go.Figure(data)

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
            annotations= 
                make_tri_annotation(t1, "P", ph.Colors["black"]) 
                + make_tri_annotation(t2, "Q", ph.Colors["black"])
                + plh.annote_coordinate_label()
                + [plh.annote_label(rect[0][0], rect[0][1], rect[0][2], r'$\pi_2$', ph.Colors["black"])]
                + [plh.annote_label(pj0m[0], pj0m[1], pj0m[2], r'$D_0$', ph.Colors["black"])]
                + [plh.annote_label(pj1m[0], pj1m[1], pj1m[2], r'$D_1$', ph.Colors["black"])]
                + [plh.annote_label(pj2m[0], pj2m[1], pj2m[2], r'$D_2$', ph.Colors["black"])]
                + [plh.annote_label((pc2 + np.array(n))[0],
                                    (pc2 + np.array(n))[1],
                                    (pc2 + np.array(n))[2], r'$n_2$', ph.Colors["black"])]
                + [plh.annote_label(ip[0],
                                    ip[1],
                                    ip[2], r'$p_{02}$', ph.Colors["black"])] 
        ),
    )

    html_path = abspath(join(_PATH_THIS_FIG_, figname + ".html"))
    fig.write_html(html_path, include_plotlyjs='cdn', include_mathjax='cdn') 
    # plh.append_js_to_show_camera_info(html_path)
    fig.write_html(abspath(join(_PATH_THIS_FIG_, figname + ".div" )), full_html=False, include_plotlyjs='cdn')


if __name__ == "__main__":
    ph.generate_0_svg(_PATH_THIS_FIG_)
    fig_triangle_define("fig1_tt_define", t1, t2)
    fig_t1_plane2("fig2_t1_plane2", t1, t2)
    fig_t1_plane2_distance("fig3_t1_plane2_distance", t1, t2)
    fig_t1_plane2_similar("fig3_t1_plane2_similar", t1, t2)
