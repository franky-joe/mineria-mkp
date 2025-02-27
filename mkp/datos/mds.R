# Cargar librerías
if (!require("ggplot2")) install.packages("ggplot2")
if (!require("viridis")) install.packages("viridis")
library(ggplot2)

# Leer el dataset
file_path <- "/home/roko/cursos/mkp/pet4-todo.csv"
df <- read.csv(file_path)

# Seleccionar solo las dimensiones del problema (x1, x2, ..., x6)
X <- df[, 7:ncol(df)]  # Desde la columna 7 en adelante

# Tomar una muestra aleatoria de 5000 filas (ajustar si es necesario)
set.seed(4)  # Para reproducibilidad
num_muestra <- min(10000, nrow(df))
df_muestra <- df[sample(nrow(df), num_muestra), ]

# Seleccionar la parte de la muestra para MDS
X_muestra <- df_muestra[, 7:ncol(df)]

# Calcular la matriz de distancias euclidianas
dist_matrix <- dist(X_muestra, method = "euclidean")

# Aplicar MDS para reducir a 2D
mds_result <- cmdscale(dist_matrix, k = 2)

# Agregar las coordenadas MDS al dataframe
df_muestra$MDS1 <- mds_result[, 1]
df_muestra$MDS2 <- mds_result[, 2]

# Graficar con ggplot2 y usar el valor de "rel" como color
ggplot(df_muestra, aes(x = MDS1, y = MDS2, color = rel)) +
  geom_point() +
  scale_color_viridis_c() +
  labs(title = "Escalado Multidimensional (MDS) en 2D") +
  theme_minimal()

