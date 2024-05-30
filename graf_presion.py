import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import pandas as pd

presion = pd.read_csv('presion.dat', header=None, comment='#')
#presion = pd.read_csv('presion_sin_demonio.dat', header=None, comment='#')

presion = presion.rename(columns={0: 'Paso', 1: 'Izd', 2 : 'Der', 3 : 'ErrorIzd', 4 : 'ErrorDer'})

print(presion)

fig, ax = plt.subplots()

ax.errorbar(presion['Paso'], presion['Der'], yerr=presion['ErrorDer'], color='tab:blue', label='Right box', capsize=5, capthick=2, fmt='o-')
#ax.plot(presion['Paso'], presion['Der'], color='tab:blue')
#ax.scatter(presion['Paso'], presion['Der'], color='tab:blue', label='Right box')

ax.errorbar(presion['Paso'], presion['Izd'], yerr=presion['ErrorIzd'], color='tab:red', label='Left box', capsize=5, capthick=2, fmt='o-')
#ax.plot(presion['Paso'], presion['Izd'], color='tab:red')
#ax.scatter(presion['Paso'], presion['Izd'], color='tab:red', label='Left box')

ax.grid(True)
ax.set_xlabel('Step')
ax.set_ylabel('Pressure')

ax.xaxis.set_major_locator(ticker.MultipleLocator(presion['Paso'].max()/10))
ax.xaxis.set_minor_locator(ticker.MultipleLocator(presion['Paso'].max()/20))

ax.yaxis.set_major_locator(ticker.MultipleLocator((presion['Der'].max() - presion['Izd'].min()) / 10))
ax.yaxis.set_minor_locator(ticker.MultipleLocator((presion['Der'].max() - presion['Izd'].min()) / 20))

plt.legend(loc='upper left')
plt.savefig('presion_sin_demonio.png')
plt.show()
