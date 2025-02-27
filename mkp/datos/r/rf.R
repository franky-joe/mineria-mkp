##################################
#### Carga de Librerías ##########
##################################
if (!require("randomForest")) install.packages("randomForest")
if (!require("MASS")) install.packages("MASS")
if (!require("dplyr")) install.packages("dplyr")
if (!require("ggplot2")) install.packages("ggplot2")

library(randomForest)
library(MASS)
library(dplyr)
library(ggplot2)

##################################
#### Lectura y Preprocesamiento ##
##################################

# Lectura del dataset
setwd("/home/roko/cursos/mkp/")
df_2_clases <- "pet1_11-14-full.csv"
df_pet1 <- read.csv(df_2_clases)

# Eliminación de variables irrelevantes
df_pet1$x <- NULL
df_pet1$y <- NULL
df_pet1$obj <- NULL
df_pet1$n <- NULL

# Selección del tamaño de muestra
if (!require("caret")) install.packages("caret")
library(caret)
set.seed(101)
tamaño_muestra <- 0.7  # Proporción de datos para entrenamiento
datos_muestra <- createDataPartition(df_pet1$f, p = tamaño_muestra, list = FALSE)
df_pet1 <- df_pet1[datos_muestra, ]

# Convertir la variable objetivo a factor
df_pet1$f <- as.factor(df_pet1$f)

##################################
#### Random Forest ###############
##################################

# Entrenamiento del modelo Random Forest
set.seed(101)
rf_model <- randomForest(
  formula = f ~ ., 
  data = df_pet1, 
  ntree = 500, 
  mtry = 2, 
  importance = TRUE, 
  proximity = TRUE
)

# Muestra los resultados del modelo
print(rf_model)

# Visualizar la importancia de las variables
importance(rf_model)
varImpPlot(rf_model)

##################################
#### Análisis de Proximidad ######
##################################

# Escalamiento clásico multidimensional usando los valores de proximidad
rf_mds <- cmdscale(1 - rf_model$proximity, eig = TRUE, k = 2)

# Crear un dataframe con las coordenadas MDS
df_mds <- data.frame(
  X1 = rf_mds$points[, 1],
  X2 = rf_mds$points[, 2],
  Clase = df_pet1$f
)

##################################
#### Visualización del MDS #######
##################################

# Crear una malla de puntos y predecir regiones de decisión
x_seq <- seq(min(df_mds$X1) - 1, max(df_mds$X1) + 1, length.out = 200)
y_seq <- seq(min(df_mds$X2) - 1, max(df_mds$X2) + 1, length.out = 200)
grid <- expand.grid(X1 = x_seq, X2 = y_seq)
grid$Predicted <- predict(rf_model, newdata = grid, type = "response")

# Graficar las regiones de decisión
p <- ggplot() +
  geom_tile(data = grid, aes(x = X1, y = X2, fill = Predicted), alpha = 0.3) +
  geom_point(data = df_mds, aes(x = X1, y = X2, color = Clase), size = 3) +
  labs(
    title = "Visualización del Random Forest en el Espacio MDS",
    x = "Dimensión 1 (MDS)",
    y = "Dimensión 2 (MDS)"
  ) +
  theme_minimal() +
  scale_fill_viridis_d(name = "Regiones (Predicted)") +
  scale_color_viridis_d(name = "Clases Reales")

print(p)

##################################
#### Fin del código #############
##################################

