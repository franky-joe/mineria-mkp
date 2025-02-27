##################################
#### Carga de Librerías ##########
##################################
if (!require("pROC")) install.packages("pROC")
if (!require("e1071")) install.packages("e1071")
if (!require("ggplot2")) install.packages("ggplot2")
if (!require("viridis")) install.packages("viridis")
if (!require("caret")) install.packages("caret")

library(e1071)
library(ggplot2)
library(pROC)
library(caret)

##################################
#### Lectura y Preprocesamiento ##
##################################

# Lectura del dataset
setwd("/home/roko/cursos/mkp/")
df_2_clases <- "pet1_11-14-full.csv"
df_2_clases <- "/home/roko/cursos/mkp/pet4-todo.csv"

df_pet1 <- read.csv(df_2_clases)
df_pet1

# Eliminación de variables segun la literatura
df_pet1$x <- NULL
df_pet1$y <- NULL
df_pet1$obj <- NULL
df_pet1$n <- NULL

# Selección del tamaño de muestra
set.seed(101)
tamaño_muestra <- 0.01  # Proporción de datos para entrenamiento
datos_muestra <- createDataPartition(df_pet1$f, p = tamaño_muestra, list = FALSE)
df_pet1 <- df_pet1[datos_muestra, ]

# Convertir la variable objetivo a factor
df_pet1$f <- as.factor(df_pet1$f)

##################################
#### SVM Kernel Lineal ###########
##################################

# Entrenamiento con SVM lineal y ajuste de parámetros
set.seed(101)
svm_lineal <- tune(
  svm,
  f ~ ., 
  data = df_pet1, 
  kernel = "linear",
  ranges = list(cost = 2^(-5:5)),
  tunecontrol = tune.control(sampling = "cross", cross = 5)
)

# Mejor modelo lineal
best_model <- svm_lineal$best.model
x <- subset(df_pet1, select = -f)
pred <- predict(best_model, x)

# Info parametros encontrados 
summary(svm_lineal)

# Matriz de confusión y métricas
conf_matrix <- confusionMatrix(pred, df_pet1$f)
print("Matriz de confusión (Kernel Lineal):")
print(conf_matrix)

##################################
#### Reducción Dimensional (MDS) #
##################################

# Calcular la matriz de distancias y aplicar MDS
dist_matrix <- dist(df_pet1[, -ncol(df_pet1)])
mds_result <- cmdscale(dist_matrix, k = 2)

# Crear un dataframe con las coordenadas reducidas
mds_df <- data.frame(
  X1 = mds_result[, 1], 
  X2 = mds_result[, 2], 
  f = df_pet1$f
)

##################################
#### Visualización MDS Lineal ####
##################################

# Entrenar el modelo SVM con kernel lineal en el espacio MDS
best_model_mds_linear <- svm(
  f ~ ., 
  data = mds_df, 
  kernel = "linear", 
  cost = 0.5
)

# Crear una malla de puntos y predecir regiones de decisión
x_seq <- seq(min(mds_df$X1) - 1, max(mds_df$X1) + 1, length.out = 200)
y_seq <- seq(min(mds_df$X2) - 1, max(mds_df$X2) + 1, length.out = 200)
grid <- expand.grid(X1 = x_seq, X2 = y_seq)
grid$Predicted <- predict(best_model_mds_linear, grid)

# Graficar las regiones de decisión
ggplot() +
  geom_tile(data = grid, aes(x = X1, y = X2, fill = Predicted), alpha = 0.3) +
  geom_point(data = mds_df, aes(x = X1, y = X2, color = f), size = 3) +
  labs(title = "Visualización del SVM con Kernel Lineal en el Espacio MDS",
       x = "Dimensión 1 (MDS)", y = "Dimensión 2 (MDS)") +
  theme_minimal() +
  scale_fill_viridis_d(name = "Regiones (Predicted)") +
  scale_color_viridis_d(name = "Clases Reales")

