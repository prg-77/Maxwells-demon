from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.patches import Circle
import numpy as np
import matplotlib.ticker as ticker
import os 


#Ruta donde se encuentran los archivos de datos
path = r"."

os.chdir(path)

archivos = []

def read_text_file(file_path): 
    with open(file_path, 'r') as f: 
        return f.read()

filename_inicial = "parametros_iniciales.dat"
filename_barrera = "barrera_abierta.dat"

with open(filename_inicial, 'r') as f:
    f.readline()  # Ignora la primera línea
    line = f.readline()  # Lee la segunda línea
    params = line.split(',')  # Divide la línea en una lista de cadenas

barrera_abierta = []
with open(filename_barrera, 'r') as f:
    for line in f.readlines():
        barrera_abierta.append(int(line))  # Añade la posición de la barrera a la lista

print(barrera_abierta)

N = [0, 0]
num = [0, 0] #Si hay más de dos tipos de partículas hay que cambiarlo
v = [0, 0] #Si hay más de dos tipos de partículas hay que cambiarlo

# Convierte cada cadena a un número y guarda el resultado en una variable
N[0], N[1], num_pasos, pos_barrera, num[0], v[0], num[1], v[1] = [float(param) if i == 3 else int(param) for i, param in enumerate(params)]

filename = []
archivos = []

for i in range(1, len(num)+1):
        filename.append("posiciones" + str(i) + ".dat")
        archivos.append(read_text_file(filename[i-1]))

file_out = "Demon_animation"

x_min = 0.5
x_max = N[0]+0.5
y_min = 0.5
y_max = N[1]+0.5

N_iter = num_pasos

#Tiempo entre fotogramas
interval = 150

#Guardar en archivo de salida
save_to_file = True 

#Calidad
dpi = 150

def leo_archivos (archivo):
    positions_data = []
    for frame_data_str in archivo.split("\n\n"):
        frame_data = []
        #Cada posición de cada partícula se separa por un salto de línea
        for part_pos_str in frame_data_str.split("\n"):
            # Lee la componente x e y de la línea
            part_pos = np.fromstring(part_pos_str, sep=",")
            if part_pos.size > 0:
                frame_data.append(part_pos)
        positions_data.append(frame_data)
    return positions_data

positions_data = []
for i in range(1, len(num)+1):
    positions_data.append(leo_archivos(archivos[i-1]))

print(positions_data[0][0])
print(positions_data[1][0])

nparts = np.sum(num)

radius = [0.2, 0.4]
part_radius = []
colours = ['red', 'blue']
part_colours = []

for i in range(1,len(num)+1):
    for j in range(num[i-1]):
        part_radius.append(radius[i-1])
        part_colours.append(colours[i-1])

print(part_radius)

nframes = len(positions_data[0])

positions = []
for i in range(1, nframes+1):
    position = []
    for j in range(1,len(num)+1):
        for k in range(1,num[j-1]+1):
            position.append(positions_data[j-1][i-1][k-1])
    positions.append(position)

print(positions[0])
         
#Creamos la animación
fig, ax = plt.subplots()

ax.set_aspect('equal', adjustable='box')
ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)

ax.xaxis.set_major_locator(ticker.MultipleLocator(N[0]/10))
ax.yaxis.set_major_locator(ticker.MultipleLocator(N[0]/10))
#ax.vlines(x=N[0]/2, ymin=y_min, ymax=y_max, linewidth=2, colors='black')

#Obtenemos las posiciones iniciales de cada partícula
part_points = []

for part_pos, radius, colour in zip(positions[0], part_radius, part_colours):
    x, y = part_pos
    part_point = Circle((x, y), radius, color=colour)
    ax.add_artist(part_point)
    part_points.append(part_point)

#Función para actualizar las posiciones de cada partícula
def update(j_frame, frames_data, part_points, pos_barrera, nframes, barrera_abierta):
    for j_part, part_pos in enumerate(frames_data[j_frame]):
        x, y = part_pos
        part_points[j_part].center = (x, y)
    if (barrera_abierta[j_frame] == 1):
        linea = ax.vlines(x=pos_barrera, ymin=y_min, ymax=y_max, linewidth=2, colors='white')
        ax.add_artist(linea)
    else:
        linea = ax.vlines(x=pos_barrera, ymin=y_min, ymax=y_max, linewidth=2, colors='black')
        ax.add_artist(linea)
    return part_points + [linea]
    

print(nframes)

def init_anim():
    return part_points

# Si hay más de un instante de tiempo, genera la animación
if nframes > 1:
    animation = FuncAnimation(
            fig, update, init_func=init_anim,
            fargs=(positions, part_points, pos_barrera, nframes, barrera_abierta),
            frames=nframes, blit=True, interval=interval)
    # Muestra por pantalla o guarda según parámetros
    if save_to_file:
        animation.save("{}.mp4".format(file_out), dpi=dpi)
    else:
        plt.grid()
        plt.show()
# En caso contrario, muestra o guarda una imagen
else:
    # Muestra por pantalla o guarda según parámetros
    if save_to_file:
        fig.savefig("{}.pdf".format(file_out))
    else:
        plt.show()   

