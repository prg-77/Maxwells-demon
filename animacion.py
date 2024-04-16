from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.patches import Circle
import numpy as np
import os 


#Nombres de los ficheros de entrada y salida de datos
path = r"C:\Users\Raul\Desktop\universidad\Cuarto\2 Cuatrimestre\Complex System\Project\Maxwells-demon"

os.chdir(path)

archivos = []

def read_text_file(file_path): 
    with open(file_path, 'r') as f: 
        #print(f.read())
        #archivos = f.read()
        #print('\n\n\n\n\n')
        #print(archivos)
        return f.read()

#file_data = {}
#file_path = []
# iterate through all file 

for file in os.listdir(): 
    # Check whether file is in text format or not 
    if file.endswith(".dat"): 
        file_path = f"{path}\{file}"
        #posiciones_i, posiciones, posiciones1, posiciones2 = file_path
        # call read text file function 
        archivos.append(read_text_file(file_path))

        #print(posiciones)

#print('Ahora los escribo yo')
#print(archivos[1])

#Obtengo el número de archivos de datos de partículas
n_arch = len(archivos) - 1

#file_in = "posiciones.dat"
file_out = "Demon_animation"

x_min = -0.5
x_max = 5.5
y_min = -0.5
y_max = 5.5

N_iter = 100

#Tiempo entre fotogramas
interval = 500

#Guardar en archivo de salida
save_to_file = False 

#Calidad
dpi = 150 

#Radio de las partículas
#part_radius = 0.1 

#with open(file_in, "r") as f:
#    data_str = f.read()

#positions_data = [[0 for j in range(N_iter)] for k in range(n_arch)]
positions_data = []
aux = []
#Leo los datos y los almaceno en una lista, primero se indica que cada frame se separa con un doble salto de línea
def leo_archivos (archivo):
    #print(archivo)
    for frame_data_str in archivo.split("\n\n"):

        #frame_data = [[0 for j in range(N_iter)] for k in range(n_arch)]
        frame_data = []
    #Cada posición de cada partícula se separa por un salto de línea
        #j=0
        for part_pos_str in frame_data_str.split("\n"):
        
        # Lee la componente x e y de la línea
            part_pos = np.fromstring(part_pos_str, sep=",")
        
            if part_pos.size > 0:
                frame_data.append(np.fromstring(part_pos_str, sep=","))

        positions_data.append(frame_data)

    return positions_data

for i in range(1, 4):
    vari = leo_archivos(archivos[i])
    # Añado a la variable donde se almacenan las posiciones de todas las partículas
    #print(vari)
    aux.append(vari)
        
print('printeo el auxiliar')
#print(aux)
#Obtengo el número de partículas que tengo

positions = []
for i in range(3):
    cont = []
    for j in range(N_iter):
        cont.append(aux[0][j + i*N_iter +1])
    positions.append(cont)

print(positions[1][0])

nparts = []
for i in range(0, 3):
    nparts.append(len(positions[i][0]))

print(nparts)


part_radius = []
for i in range(n_arch):
    radius = []
    for j in range(nparts[i]):
        radius.append(0.1)
    part_radius.append(radius)

#print(part_radius)
#Creamos la animación
fig, ax = plt.subplots()

ax.axis("equal")
ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)

#Si se ha introducido un radio, se dice que todas las partículas tienen ese radio
for i in range(n_arch):
    if not hasattr(part_radius[i], "__iter__"):
        part_radius[i] = part_radius[i]*np.ones(nparts[i])
#Si se ha introducido distintos radios para cada partícula, se comrpueba que se hayan introducido tantos radios como partículas
    else:
        if not nparts[i] == len(part_radius[i]):
            raise ValueError(
                "El número de radios especificados no coincide con el número "
                "de partículas")

#Variable para almacenar las posiciones de cada partícula en un frame


#Obtenemos las posiciones iniciales de cada partícula
aux2 = []

part_points0 = []
for part_pos, radius in zip(positions[0][0], part_radius[0]):
    x, y = part_pos
    #planet_point, = ax.plot(x, y, "o", markersize=10)
    part_point = Circle((x, y), radius, color='black')
    ax.add_artist(part_point)
    part_points0.append(part_point)

part_points1 = []
for part_pos, radius in zip(positions[1][0], part_radius[1]):
    x, y = part_pos
    #planet_point, = ax.plot(x, y, "o", markersize=10)
    part_point = Circle((x, y), radius, color='tab:blue')
    ax.add_artist(part_point)
    part_points1.append(part_point)

part_points2 = []
for part_pos, radius in zip(positions[2][0], part_radius[1]):
    x, y = part_pos
    #planet_point, = ax.plot(x, y, "o", markersize=10)
    part_point = Circle((x, y), radius, color='tab:red')
    ax.add_artist(part_point)
    part_points2.append(part_point)

#Función para actualizar las posiciones de cada partícula
def update0(j_frame, frames_data, part_points):
    for j_part, part_pos in enumerate(frames_data[j_frame]):
        x, y = part_pos
        part_points[j_part].center = (x, y)

    return part_points

nframes = len(positions[0])


def init_anim0():
    return part_points0

def init_anim1():
    return part_points1

def init_anim2():
    return part_points2

# Si hay más de un instante de tiempo, genera la animación
if nframes > 1:
    animation0 = FuncAnimation(
            fig, update0, init_func=init_anim0,
            fargs=(positions[0], part_points0),
            frames=len(positions[0]), blit=True, interval=interval)
    
    animation1 = FuncAnimation(
            fig, update0, init_func=init_anim1,
            fargs=(positions[1], part_points1),
            frames=len(positions[0]), blit=True, interval=interval)
    
    animation2 = FuncAnimation(
            fig, update0, init_func=init_anim2,
            fargs=(positions[2], part_points2),
            frames=len(positions[0]), blit=True, interval=interval)
    
    '''def updateAll (nframes):
        a = update0
        b = update1
        c = update2
        return a+b+c'''
    
    # Muestra por pantalla o guarda según parámetros
    if save_to_file:
        animation0.save("{}.mp4".format(file_out), dpi=dpi)
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