import pandas as pd
import numpy as np
import plotly.express as px
from sklearn.manifold import MDS

# Cargar el dataset
file_path = "/home/roko/cursos/mineria-mkp-main/mkp/pet3-todo.csv"
df = pd.read_csv(file_path)

# Filtrar solo datos donde f == 1
df = df[df['f'] == 1]

# Seleccionar los primeros 4000 datos con mejor 'rel' (mayor valor)
df = df.sort_values(by='rel', ascending=False).head(10000)

# Seleccionar las dimensiones del problema (x1, x2, ..., x6)
X = df.iloc[:, 6:].values  

# Aplicar MDS en la muestra
mds = MDS(n_components=2, random_state=42, metric=True)
X_transformed = mds.fit_transform(X)

# Agregar las dimensiones transformadas al DataFrame
df['MDS1'] = X_transformed[:, 0]
df['MDS2'] = X_transformed[:, 1]

# Crear gráfico interactivo con Plotly
fig = px.scatter(df, x='MDS1', y='MDS2', color='rel',
                 color_continuous_scale='viridis',
                 title="pet3 en 2D - Mejores 10000 Datos factibles(f=1)")
fig.write_html("/home/roko/cursos/mineria-mkp-main/pet3-mkpgrafico_mejores2d-10000.html")

fig.show()

