import pandas as pd
import numpy as np
import plotly.express as px
from sklearn.manifold import MDS

# Cargar el dataset
file_path = "/home/roko/cursos/mineria-mkp-main/mkp/pb5-todo.csv"
df = pd.read_csv(file_path)

# Eliminar filas donde f == 0
df = df[df['f'] != 0]
# Seleccionar los primeros 4000 datos con mejor 'rel' (mayor valor)
df = df.sort_values(by='rel', ascending=False).head(10000)

# Reducir el número de datos con una muestra aleatoria (máx. 1000 filas)
num_muestra = min(10000, len(df))
df_muestra = df.sample(n=num_muestra, random_state=42)

# Seleccionar las dimensiones del problema (x1, x2, ..., x6)
X_muestra = df_muestra.iloc[:, 6:].values  


# Reducir el número de datos con una muestra aleatoria (máx. 10,000 filas)
num_muestra = min(1000, len(df))
df_muestra = df.sample(n=num_muestra, random_state=42)
X_muestra = df_muestra.iloc[:, 6:].values  

# Aplicar MDS en la muestra
mds = MDS(n_components=2, random_state=42, dissimilarity='euclidean')
X_transformed = mds.fit_transform(X_muestra)

# Agregar las dimensiones transformadas al DataFrame
df_muestra['MDS1'] = X_transformed[:, 0]
df_muestra['MDS2'] = X_transformed[:, 1]

# Crear gráfico interactivo con plotly
fig = px.scatter(df_muestra, x='MDS1', y='MDS2', color='rel',
                 color_continuous_scale='viridis',
                 title="Escalado Multidimensional (MDS) en 2D")

fig.show()
