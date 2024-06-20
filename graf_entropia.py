import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import pandas as pd
from scipy.optimize import curve_fit

entropia = pd.read_csv('entropia.dat', header=None, comment='#') # Con demonio
entropia_sin_demonio = pd.read_csv('entropia_sin_demonio.dat', header=None, comment='#')   # Sin demonio

entropia = entropia.rename(columns={0: 'Paso', 1: 'Entropia', 2 : 'Error'})
entropia_sin_demonio = entropia_sin_demonio.rename(columns={0: 'Paso', 1: 'Entropia', 2 : 'Error'})

#print(entropia)

fig, ax = plt.subplots()

ax.errorbar(entropia['Paso'], entropia['Entropia'], yerr=entropia['Error'], color='tab:blue', label='With demon', capsize=5, capthick=2, fmt='o-')
#ax.plot(entropia['Paso'], entropia['Entropia'], color='tab:blue')
#ax.scatter(entropia['Paso'], entropia['Entropia'], color='tab:blue', label= 'With demon')

ax.errorbar(entropia_sin_demonio['Paso'], entropia_sin_demonio['Entropia'], yerr=entropia_sin_demonio['Error'], color='tab:red', label='Without demon', capsize=5, capthick=2, fmt='o-')
#ax.plot(entropia_sin_demonio['Paso'], entropia_sin_demonio['Entropia'], color='tab:red')
#ax.scatter(entropia_sin_demonio['Paso'], entropia_sin_demonio['Entropia'], color='tab:red', label=' Without demon')

'''
def exponential_decay(x, a, b, c):
    return a * np.exp(-b * x) + c

popt, pcov = curve_fit(exponential_decay, entropia['Paso'],entropia['Entropia'] , bounds=(0, [300, 1, 255]))
a, b, c = popt
print(f"Ajuste de parámetros: a={a}, b={b}, c={c}")

plt.plot(range(1,20000), exponential_decay(range(1,20000), *popt), color='green', label="Ajuste ") # Dibujar ajuste
'''

size = 10

ax.grid(True)
ax.set_xlabel('Step', fontsize=size)
ax.set_ylabel('Entropy', fontsize=size)
ax.tick_params(axis='both', labelsize=size)


ax.xaxis.set_major_locator(ticker.MultipleLocator(2500))      
#ax.xaxis.set_major_locator(ticker.MultipleLocator(entropia['Paso'].max()/10))

#ax.xaxis.set_minor_locator(ticker.MultipleLocator(entropia['Paso'].max()/20))


# Locator a partir de 255 de 5 en 5
ax.yaxis.set_major_locator(ticker.MultipleLocator(2.5))
#ax.yaxis.set_major_locator(ticker.MultipleLocator((entropia['Entropia'].max() - entropia['Entropia'].min()) / 10))
#ax.yaxis.set_minor_locator(ticker.MultipleLocator((entropia['Entropia'].max() - entropia['Entropia'].min()) / 20))

plt.legend()
plt.savefig('entropia_ambas.png')
plt.show()
