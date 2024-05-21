import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import pandas as pd

#entropia = pd.read_csv('entropia.dat', header=None, comment='#') # Con demonio
entropia = pd.read_csv('entropia_sin_demonio.dat', header=None, comment='#')   # Sin demonio

entropia = entropia.rename(columns={0: 'Paso', 1: 'nA', 2 : 'nB', 3 : 'Entropia'})

print(entropia)

fig, ax = plt.subplots()

ax.plot(entropia['Paso'], entropia['Entropia'], color='tab:blue')
ax.scatter(entropia['Paso'], entropia['Entropia'], color='tab:blue', label=r'$S=ln(\Omega)$')

ax.grid(True)
ax.set_xlabel('Step')
ax.set_ylabel('Entropy')

ax.xaxis.set_major_locator(ticker.MultipleLocator(entropia['Paso'].max()/10))
ax.xaxis.set_minor_locator(ticker.MultipleLocator(entropia['Paso'].max()/20))

ax.yaxis.set_major_locator(ticker.MultipleLocator((entropia['Entropia'].max() - entropia['Entropia'].min()) / 10))
ax.yaxis.set_minor_locator(ticker.MultipleLocator((entropia['Entropia'].max() - entropia['Entropia'].min()) / 20))

plt.legend()
plt.savefig('entropia_sin_demonio.png')
plt.show()
