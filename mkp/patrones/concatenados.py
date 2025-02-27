import pandas as pd

def contar_zeros(input_csv, output_csv):
    # Lee el archivo CSV
    df = pd.read_csv(input_csv)
    
    # Lista para almacenar los resultados
    results = []

    # Variables para rastrear los 0 consecutivos
    consecutive_zeros = 0
    start_index = None

    # Iterar sobre las filas del DataFrame
    for index, row in df.iterrows():
        if row['f'] == 0:
            if consecutive_zeros == 0:
                start_index = index + 1  # +1 para que coincida con la fila en el CSV
            consecutive_zeros += 1
        else:
            if consecutive_zeros > 0:
                results.append([start_index, consecutive_zeros])
                consecutive_zeros = 0
                start_index = None

    # Captura el último grupo de 0's si termina con 0
    if consecutive_zeros > 0:
        results.append([start_index, consecutive_zeros])

    # Crea un DataFrame para los resultados
    results_df = pd.DataFrame(results, columns=['fila', 'cantidad_0s'])

    # Guarda los resultados en un nuevo archivo CSV
    results_df.to_csv(output_csv, index=False)

    # Imprime los resultados
    print(results_df)

# Uso de la función
input_csv = '/home/roko/cursos/mkp/resultados/flei/flei.csv'
output_csv = 'resultado.csv'
contar_zeros(input_csv, output_csv)
