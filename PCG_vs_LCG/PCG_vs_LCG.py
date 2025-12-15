import plotly.graph_objects as go
import numpy as np

# --- Parameters ---
num_points_3d = 5000
seed = 42
a_bad = 65539
c_bad = 0
m = 2**32

# --- LCG generator ---
def lcg(seed, a, c, m, size):
    x = seed
    for _ in range(size):
        x = (a * x + c) % m
        yield x

lcg_values_bad = np.fromiter(lcg(seed, a_bad, c_bad, m, num_points_3d), dtype=np.uint32, count=num_points_3d)

# --- PCG64 values using numpy random ---
rng = np.random.default_rng(seed)
pcg_values = rng.integers(0, m, size=num_points_3d, dtype=np.uint32)

# --- Prepare consecutive triples for 3D scatter ---
def prepare_triples(values):
    return values[:-2] / m, values[1:-1] / m, values[2:] / m

X_lcg, Y_lcg, Z_lcg = prepare_triples(lcg_values_bad)
X_pcg, Y_pcg, Z_pcg = prepare_triples(pcg_values)

# --- Create Plotly figure with two subplots ---
from plotly.subplots import make_subplots

fig = make_subplots(
    rows=1, cols=2,
    specs=[[{'type':'scatter3d'}, {'type':'scatter3d'}]],
    subplot_titles=("LCG (Red): Lattice Structure", "PCG64 (Blue): Truly Scattered")
)

# LCG plot
fig.add_trace(
    go.Scatter3d(x=X_lcg, y=Y_lcg, z=Z_lcg, mode='markers',
                 marker=dict(size=2, color='red', opacity=0.8)),
    row=1, col=1
)

# PCG plot
fig.add_trace(
    go.Scatter3d(x=X_pcg, y=Y_pcg, z=Z_pcg, mode='markers',
                 marker=dict(size=2, color='blue', opacity=0.8)),
    row=1, col=2
)

# Update layout
fig.update_layout(
    width=1400, height=700,
    scene=dict(
        xaxis_title='X_n',
        yaxis_title='X_{n+1}',
        zaxis_title='X_{n+2}'
    ),
    scene2=dict(
        xaxis_title='X_n',
        yaxis_title='X_{n+1}',
        zaxis_title='X_{n+2}'
    )
)

fig.show()

