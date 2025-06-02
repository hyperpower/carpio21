import plot_helper as ph

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
    veo = ph.sub(p1,p0)
    veo = ph.normalize(veo)
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
                      "size" : 16
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

def append_js_to_show_camera_info(html_name):
    with open(html_name, 'r') as file:
        content = file.read()
    
    js_code = """
    <script>
    document.addEventListener("DOMContentLoaded", function() {
        var gd = document.querySelector('.js-plotly-plot');
        var infoDiv = document.createElement('div');
        infoDiv.style.position = 'fixed';
        infoDiv.style.top = '10px';
        infoDiv.style.right = '10px';
        infoDiv.style.background = 'rgba(255,255,255,0.9)';
        infoDiv.style.border = '1px solid #ccc';
        infoDiv.style.padding = '8px';
        infoDiv.style.zIndex = 1000;
        infoDiv.style.fontSize = '12px';
        infoDiv.style.fontFamily = 'monospace';
        document.body.appendChild(infoDiv);

        function updateCameraInfo(eventdata) {
            if(eventdata && eventdata['scene.camera']) {
                var cam = eventdata['scene.camera'];
                infoDiv.innerText = JSON.stringify(cam, null, 2);
            }
        }
        gd.on('plotly_relayout', updateCameraInfo);
    });
    </script>
    """
    
    content += js_code
    
    with open(html_name, 'w') as file:
        file.write(content)