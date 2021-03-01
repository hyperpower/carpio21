import plotly.graph_objects as go

fig = go.Figure()

fig.add_trace(go.Scatter3D(
    x    = [0, 1, 2],
    y    = [1, 1, 1],
    mode = "lines+markers+text",
    name = "Lines, Markers and Text",
    text = ["Text A", "Text B", "Text C"],
    textposition ="top center"
))

fig.show()