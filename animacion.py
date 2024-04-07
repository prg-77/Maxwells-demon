import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


#Tengo una sola partícula y genero aleatoriamente la posición
#x = np.random.rand(100)
#y = np.random.rand(100)



datos = 'posiciones.dat'

#Leo el fichero
with open(datos, "r") as f:
    datos_str = f.read()

#Inicializo la lista con las posiciones
posiciones = list()

print(posiciones)

#Leo los datos y los almaceno en una lista, primero se indica que cada frame se separa con un doble salto de línea
for un_frame_str in datos_str.split("\n\n"):
    posicion = list()

    #Cada posición de cada partícula se separa por un salto de línea
    for part_pos_str in un_frame_str.split("\n"):

        # Lee la componente x e y de cada partícula
        part_pos = np.fromstring(part_pos_str, sep=" , ")
        #print(part_pos)
        #x1, y1 = part_pos
        #print(x1)
        #print(y1)

        #Añado la partícula a la lista de posiciones
        if part_pos.size > 0:
            posicion.append(np.fromstring(part_pos_str, sep=" , "))

    # Añade los datos de este fotograma a la lista
    posiciones.append(posicion)

#Obtenemos el número de partículas que tenemos
npart = len(posiciones[0])
#print(x1)
print(posiciones[10])
#print('\n')
#print(npart)
interval = 10

fig, ax = plt.subplots()

ax.set_xlim(-0.5, 9.5)
ax.set_ylim(-0.5, 9.5)

##########################################

puntos_part = list()

x = []
y = []

def initial_data():
    for part_pos in (posiciones[0]):
    
        x, y = part_pos
        #print(part_pos[0])

        punto_part, = ax.plot(x, y, "o", markersize=10, color='tab:blue')
        #ax.add_artist(punto_part)
        puntos_part.append(punto_part)

        animated_plot, = ax.plot(x, y, 'o', markersize=5, color='black')
    
    return animated_plot

animated_plot, = ax.plot(x, y, 'o', markersize=5, color='black')


def update_data (frames):
    #for j, part_pos in enumerate(posiciones[frames]):
    for part_pos in (posiciones[frames]):
        #Hace falta meter un índice para tener en cuenta que hay más de una partícula
        x, y = part_pos
        print(x[0])
        for i in range(npart):
            animated_plot[i].set_data(x, y)
        #animated_plot.set_data(x, y)
        #animated_plot, = ax.plot(x, y, 'o', markersize=5, color='black')
        #puntos_part[j].center = (x, y)
        
    return animated_plot

animation = FuncAnimation(fig=fig,
                          func = update_data,
                          frames=len(posiciones),
                          init_func=initial_data,
                          interval=100)


plt.grid()
plt.show()


'''
# Función que actualiza la posición de los planetas en la animación 
def update(j_frame, posiciones, puntos_part):
    # Actualiza la posición del correspondiente a cada planeta
    for j_part, part_pos in enumerate(posiciones[j_frame]):
        x, y = part_pos
        puntos_part[j_part].center = (x, y)

    return puntos_part

def init_anim():
    return puntos_part

# Calcula el nº de frames
nframes = len(posiciones)
print(nframes)

# Si hay más de un instante de tiempo, genera la animación
if nframes > 1:
    # Info sobre FuncAnimation: https://matplotlib.org/stable/api/animation_api.html
    animation = FuncAnimation(
            fig, update, init_func=init_anim,
            fargs=(posiciones, puntos_part),
            frames=len(posiciones), blit=True, interval=interval)

    plt.show()
'''






'''
    # Muestra por pantalla o guarda según parámetros
    if save_to_file:
        animation.save("{}.mp4".format(file_out), dpi=dpi)
    else:
        plt.show()
# En caso contrario, muestra o guarda una imagen
else:
    # Muestra por pantalla o guarda según parámetros
    if save_to_file:
        fig.savefig("{}.pdf".format(file_out))
    else:
        plt.show()

'''









'''
animated_plot, = ax.plot([], [], 'o', markersize=5, color='black')

def update_data (frame):
    animated_plot.set_data([x[frame]], [y[frame]])

    return animated_plot

animation = FuncAnimation(fig=fig,
                          func = update_data,
                          frames=len(x),
                          interval=100)


plt.grid()
plt.show()

'''