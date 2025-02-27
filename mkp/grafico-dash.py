import pandas as pd
import numpy as np
import plotly.express as px
from sklearn.manifold import MDS
from scipy.spatial.distance import pdist, squareform
import dash
from dash import dcc, html
from dash.dependencies import Input, Output

# Cargar el dataset
file_path = "/home/roko/cursos/mineria-mkp-main/mkp/pet4-todo.csv"
df = pd.read_csv(file_path)

# Eliminar filas donde f == 0
df = df[df['f'] != 0]
# Seleccionar los primeros 10000 datos con mejor 'rel'
df = df.sort_values(by='rel', ascending=False).head(1000)

# Reducir la muestra
df_muestra = df.sample(n=min(1000, len(df)), random_state=42)

# Seleccionar las dimensiones relevantes
X_muestra = df_muestra.iloc[:, 6:].values  
D = squareform(pdist(X_muestra, metric='euclidean'))  # Matriz de distancias

# Aplicar MDS en 3D
mds = MDS(n_components=3, random_state=42, dissimilarity='precomputed')
X_transformed = mds.fit_transform(D)

# Agregar dimensiones transformadas
df_muestra['MDS1'] = X_transformed[:, 0]
df_muestra['MDS2'] = X_transformed[:, 1]
df_muestra['MDS3'] = X_transformed[:, 2]

# Inicializar la aplicación Dash
app = dash.Dash(__name__)

# Layout con slider y gráfico
app.layout = html.Div([
    html.H1("Escalado Multidimensional (MDS) en 3D"),
    dcc.RangeSlider(
        id='slider-rel',
        min=df_muestra['rel'].min(),
        max=df_muestra['rel'].max(),
        step=0.01,
        value=[df_muestra['rel'].min(), df_muestra['rel'].max()],
        marks={int(i): str(int(i)) for i in np.linspace(df_muestra['rel'].min(), df_muestra['rel'].max(), 5)}
    ),
    dcc.Graph(id='3d-scatter')
])

# Callback para actualizar el gráfico dinámicamente
@app.callback(
    Output('3d-scatter', 'figure'),
    [Input('slider-rel', 'value')]
)
def update_graph(rel_range):
    df_filtered = df_muestra[(df_muestra['rel'] >= rel_range[0]) & (df_muestra['rel'] <= rel_range[1])]
    fig = px.scatter_3d(df_filtered, x='MDS1', y='MDS2', z='MDS3', color='rel',
                        color_continuous_scale='viridis')
    return fig

# Ejecutar la app
if __name__ == '__main__':
    app.run_server(debug=True)