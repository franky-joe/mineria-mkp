import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from sklearn.manifold import MDS
from scipy.spatial.distance import pdist, squareform

# Cargar el dataset
file_path = "/home/roko/cursos/mkp/pet4-todo.csv"
df = pd.read_csv(file_path)

# Aplicar la regla: si f == 0, entonces rel = 0
df.loc[df['f'] == 0, 'rel'] = 0

# Reducir el número de datos con una muestra aleatoria (máx. 1,000 filas para rendimiento)
num_muestra = min(10000, len(df))
df_muestra = df.sample(n=num_muestra, random_state=42)

# Seleccionar las dimensiones del problema (x1, x2, ..., x6)
X_muestra = df_muestra.iloc[:, 6:].values  

# Calcular la matriz de distancias euclidianas
D = squareform(pdist(X_muestra, metric='euclidean'))  

# Aplicar MDS con matriz de distancias precomputada
mds = MDS(n_components=2, random_state=42, dissimilarity='precomputed')
X_transformed = mds.fit_transform(D)

# Agregar las dimensiones transformadas al DataFrame
df_muestra['MDS1'] = X_transformed[:, 0]
df_muestra['MDS2'] = X_transformed[:, 1]

# Configurar la visualización en tiempo real con Matplotlib
fig, ax = plt.subplots()
sc = ax.scatter([], [], c=[], cmap='viridis', edgecolor='k')
ax.set_xlim(X_transformed[:, 0].min() - 1, X_transformed[:, 0].max() + 1)
ax.set_ylim(X_transformed[:, 1].min() - 1, X_transformed[:, 1].max() + 1)
ax.set_title("Escalado Multidimensional (MDS) en tiempo real")
ax.set_xlabel("MDS1")
ax.set_ylabel("MDS2")
fig.colorbar(sc, label="rel")

# Datos que se irán agregando progresivamente
x_data, y_data, colors = [], [], []

def update(frame):
    """Función que agrega puntos progresivamente"""
    if frame < len(df_muestra):
        x_data.append(df_muestra.iloc[frame]['MDS1'])
        y_data.append(df_muestra.iloc[frame]['MDS2'])
        colors.append(df_muestra.iloc[frame]['rel'])
        
        sc.set_offsets(np.c_[x_data, y_data])
        sc.set_array(np.array(colors))

    return sc,

# Animación: va agregando puntos uno a uno
ani = animation.FuncAnimation(fig, update, frames=len(df_muestra), interval=5, repeat=False)

# Mostrar la ventana con la animación
plt.show()
