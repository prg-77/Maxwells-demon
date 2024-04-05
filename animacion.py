import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

#Tengo una sola partícula y genero aleatoriamente la posición
x = np.random.rand(100)
y = np.random.rand(100)


fig, ax = plt.subplots()

ax.set_xlim(-0.5, 1.5)
ax.set_ylim(-0.5, 1.5)

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