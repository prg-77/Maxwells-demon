import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import pandas as pd

entropia = pd.read_csv('entropia.dat', header=None, comment='#')

entropia = entropia.rename(columns={0: 'Paso', 1: 'nA', 2 : 'nB', 3 : 'Entropia'})

print(entropia)

fig, ax = plt.subplots()

ax.plot(entropia['Paso'], entropia['Entropia'], color='tab:blue')
ax.scatter(entropia['Paso'], entropia['Entropia'], color='tab:blue', label=r'$S=ln(\Omega)$')

ax.grid(True)
ax.set_xlabel('Paso')
ax.set_ylabel('Entropía')

ax.xaxis.set_major_locator(ticker.MultipleLocator(50000))
ax.xaxis.set_minor_locator(ticker.MultipleLocator(25000))

ax.yaxis.set_major_locator(ticker.MultipleLocator(2))
ax.yaxis.set_minor_locator(ticker.MultipleLocator(1))

plt.legend()
plt.show()