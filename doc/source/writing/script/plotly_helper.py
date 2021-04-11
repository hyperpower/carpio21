import math
import plotly.graph_objects as go

def sub(p1, p0):
    p = [p1[0]-p0[0],
         p1[1]-p0[1],
         p1[2]-p0[2]]
    return p

def mid(p1, p0):
    p = [(p1[0]+p0[0])*0.5,
         (p1[1]+p0[1])*0.5,
         (p1[2]+p0[2])*0.5]
    return p

def to_xyz(p0, p1, p2):
    res = [
      [p0[0], p1[0], p2[0]], #x
      [p0[1], p1[1], p2[1]], #y
      [p0[2], p1[2], p2[2]], #z
    ]
    return res

def length(p):
    return math.sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2])
    
def normalize(p):
    l = length(p)
    s3 = math.sqrt(3.0)
    return [p[0]/l*s3, p[1]/l*s3, p[2]/l*s3]

def arrow(go, p0, p1, c):
    traces = {}
    traces["arrow_line1"] = go.Scatter3d(
                               x = [p0[0],p1[0]], 
                               y = [p0[1],p1[1]],
                               z = [p0[2],p1[2]],
                               showlegend=False,
                               mode='lines',
                               line=dict(
                                 color=c, dash="longdash",
                                 width=5))
    veo = sub(p1,p0)
    veo = normalize(veo)
    traces["Cone"] = go.Cone(
                         x=[p1[0]], y=[p1[1]], z=[p1[2]],
                         u=[veo[0]], v=[veo[1]], w=[veo[2]],
                         sizemode   = 'scaled',
                         sizeref    = 0.25,
                         showscale  = False,
                         anchor     = "tip",
                         colorscale =[[0, c], [1, c]])
    return traces

def coordinate(go):
    traces = {}
    traces["coordinate_x"] = go.Scatter3d(
                               x = [0,1], 
                               y = [0,0],
                               z = [0,0],
                               showlegend=False,
                               mode='lines',
                               line=dict(
                                 color="green",
                                 width=5))
    traces["coordinate_y"] = go.Scatter3d(
                               x = [0,0], 
                               y = [0,1],
                               z = [0,0],
                               showlegend=False,
                               mode='lines',
                               line=dict(
                                 color="red",
                                 width=5))
    traces["coordinate_z"] = go.Scatter3d(
                               x = [0,0], 
                               y = [0,0],
                               z = [0,1],
                               showlegend=False,
                               mode='lines',
                               line=dict(
                                 color="blue",
                                 width=5))
    return traces


def annote_label(cx, cy, cz, name, c):
    res = {
        "showarrow" : False,
        "x"         : cx,
        "y"         : cy,
        "z"         : cz,
        "xshift"    : 5,
        "text"      : name,
        "ax"        : 50,
        "ay"        : 0,
        "font"      :{"color": c,
                      "size" : 14
                      },
        "xanchor"   : "left",
        "yanchor"   : "bottom"
        }
    return res

def annote_point_label(p, name, c):
    res = {
        "showarrow" : False,
        "x"         : p[0],
        "y"         : p[1],
        "z"         : p[2],
        "xshift"    : 5,
        "text"      : name,
        "ax"        : 50,
        "ay"        : 0,
        "font"      :{"color": c,
                      "size" : 14
                      },
        "xanchor"   : "left",
        "yanchor"   : "bottom"
        }
    return res