import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import pandas as pd

presion = pd.read_csv('presion.dat', header=None, comment='#')

presion = presion.rename(columns={0: 'Paso', 1: 'Izd', 2 : 'Der'})

print(presion)

fig, ax = plt.subplots()

ax.plot(presion['Paso'], presion['Der'], color='tab:blue')
ax.scatter(presion['Paso'], presion['Der'], color='tab:blue', label='Lado derecho')

ax.plot(presion['Paso'], presion['Izd'], color='tab:red')
ax.scatter(presion['Paso'], presion['Izd'], color='tab:red', label='Lado izquierdo')

ax.grid(True)
ax.set_xlabel('Paso')
ax.set_ylabel('Presión')

ax.xaxis.set_major_locator(ticker.MultipleLocator(50000))
ax.xaxis.set_minor_locator(ticker.MultipleLocator(25000))

ax.yaxis.set_major_locator(ticker.MultipleLocator(200))
ax.yaxis.set_minor_locator(ticker.MultipleLocator(100))

plt.legend(loc='center')
plt.show()