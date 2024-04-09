from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.patches import Circle
import numpy as np

#Nombres de los ficheros de entrada y salida de datos
file_in = "posiciones.dat"
file_out = "Demon_animation"

# Límites de los ejes X e Y
x_min = -0.5
x_max = 4.5
y_min = -0.5
y_max = 4.5

#Tiempo entre fotogramas
interval = 100

#Guardar en archivo de salida
save_to_file = False 

#Calidad
dpi = 150 

#Radio de las partículas
part_radius = 0.1 

with open(file_in, "r") as f:
    data_str = f.read()

positions_data = list()

#Leo los datos y los almaceno en una lista, primero se indica que cada frame se separa con un doble salto de línea
for frame_data_str in data_str.split("\n\n"):

    frame_data = list()
    #Cada posición de cada partícula se separa por un salto de línea
    for part_pos_str in frame_data_str.split("\n"):
        
        # Lee la componente x e y de la línea
        part_pos = np.fromstring(part_pos_str, sep=",")
        
        if part_pos.size > 0:
            frame_data.append(np.fromstring(part_pos_str, sep=","))

    # Añado a la variable donde se almacenan las posiciones de todas las partículas
    positions_data.append(frame_data)

#Obtengo el número de partículas que tengo
nparts = len(positions_data[0])
print(nparts)

#Creamos la animación
fig, ax = plt.subplots()

ax.axis("equal")
ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)

#Si se ha introducido un radio, se dice que todas las partículas tienen ese radio
if not hasattr(part_radius, "__iter__"):
    part_radius = part_radius*np.ones(nparts)
#Si se ha introducido distintos radios para cada partícula, se comrpueba que se hayan introducido tantos radios como partículas
else:
    if not nparts == len(part_radius):
        raise ValueError(
                "El número de radios especificados no coincide con el número "
                "de partículas")

#Variable para almacenar las posiciones de cada partícula en un frame
part_points = list()

#Obtenemos las posiciones iniciales de cada partícula
for part_pos, radius in zip(positions_data[0], part_radius):
    x, y = part_pos
    #planet_point, = ax.plot(x, y, "o", markersize=10)
    part_point = Circle((x, y), radius, color='black')
    ax.add_artist(part_point)
    part_points.append(part_point)

    
#Función para actualizar las posiciones de cada partícula
def update(j_frame, frames_data, part_points):
    for j_planet, part_pos in enumerate(frames_data[j_frame]):
        x, y = part_pos
        part_points[j_planet].center = (x, y)

    return part_points

#Función para inicializar la animación a las posiciones iniciales obtenidas anteriormente
def init_anim():
    return part_points

nframes = len(positions_data)

# Si hay más de un instante de tiempo, genera la animación
if nframes > 1:
    animation = FuncAnimation(
            fig, update, init_func=init_anim,
            fargs=(positions_data, part_points),
            frames=len(positions_data), blit=True, interval=interval)

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