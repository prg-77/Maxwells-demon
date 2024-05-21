#include <iostream>
#include <numeric>
#include <string>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void inicializar_posiciones(int posiciones[][2], int N[2], int num[], int tipos, ofstream* datos_posiciones);
void actualizar_posiciones(int posiciones[][2], int posiciones_viejas[][2], int N[2], int num[], int v[], int tipos, ofstream* datos_posiciones, bool barrera_abierta, double pos_barrera);
bool rodeado(int posiciones[][2], int N[2], int num[], int i, int v, int tipos, bool barrera_abierta, double pos_barrera);
int suma_array(int array[], int n);
double posicion_barrera(int N);
bool barrera(int posiciones[][2], int N[2], int num[], double pos_barrera, int v[], int izqda); // Función hecha solo para tipos = 2.
bool demonio(int posicion_vieja, int posicion_nueva, double pos_barrera); // Función hecha solo para tipos = 2. 
double log_prod(int N, int M);
double log_factorial(int n);
double entropy(int numA, int numB, int nA, int nB, int N[2]);
void particulas_izquierda(int posiciones[][2], int N[2], int num[], double pos_barrera, int n_izq[]);
void calculo_presion(int posiciones[][2], int posiciones_viejas[][2], int num[2], int N[2], double pos_barrera, double presion[2]);

int main()
{
    int N[2], tipos, num_pasos;
    bool barrera_abierta;

    N[0] = 100; // Número de celdas horizontales
    N[1] = 100; // Número de celdas verticales
    tipos = 2; // Número de tipos de partículas
    num_pasos = 42000; // Número de pasos

    int num[tipos], v[tipos], n_izq[tipos];
    double pos_barrera, presion[tipos], presion_sumada[tipos];

    pos_barrera = posicion_barrera(N[0]); //Posición de la barrera
    cout << pos_barrera << endl;
    cout << endl;

    presion_sumada[0] = presion_sumada[1] = 0;

    // Si queremos el mismo número de partículas de cada tipo y velocidades ascendentes
    for (int i = 0; i < tipos; i++)
    {
        num[i] = 200; // Número de partículas de cada tipo [num1, num2, ...]
        v[i] = i+1; // Velocidad de las partículas de cada tipo [v1, v2, ...]
    }
    // Si queremos un número de partículas y velocidades diferentes para cada tipo
    // num[0] = 1;
    // num[1] = 1;
    // v[0] = 1;
    // v[1] = 2;

    int posiciones[suma_array(num,tipos)][2], posiciones_viejas[suma_array(num,tipos)][2];
    ofstream parametros_iniciales, datos_posiciones[tipos], barrera_abierta_fichero, entropia_fichero, presion_fichero;

    parametros_iniciales.open("parametros_iniciales.dat");

    parametros_iniciales << "#N1 N2 num_pasos pos_barrera";

    for (int i = 0; i < tipos; i++)
    {
        parametros_iniciales << " num" << i+1 << " v" << i+1;
    }

    parametros_iniciales << endl;

    parametros_iniciales << N[0] << " , " << N[1] << " , " << num_pasos << " , " << pos_barrera;

    for (int i = 0; i < tipos; i++)
    {
        parametros_iniciales << " , " << num[i] << " , " << v[i];
    }

    parametros_iniciales.close();

    for (int i = 0; i < tipos; i++)
    {
        datos_posiciones[i].open("posiciones" + to_string(i+1) + ".dat");
    }

    barrera_abierta_fichero.open("barrera_abierta_sin_demonio.dat");
    entropia_fichero.open("entropia_sin_demonio.dat");
    presion_fichero.open("presion_sin_demonio.dat");

    inicializar_posiciones(posiciones, N, num, tipos, datos_posiciones);

    particulas_izquierda(posiciones, N, num, pos_barrera, n_izq);
    entropia_fichero <<  "# Paso   nA   nB   Entropía" << endl;
    entropia_fichero << 0 << " , " << n_izq[0] << " , " << n_izq[1] << " , " << entropy(num[0], num[1], n_izq[0], n_izq[1], N) << endl;
    presion_fichero << "# Paso   Presión Izq   Presión Dcha" << endl;

    for (int i = 1; i < num_pasos; i++)
    {
        for (int j = 0; j < tipos; j++)
        {
            datos_posiciones[j] << endl;
        }
        barrera_abierta = barrera(posiciones, N, num, pos_barrera, v, 0);
        barrera_abierta_fichero << barrera_abierta << endl;
        actualizar_posiciones(posiciones, posiciones_viejas, N, num, v, tipos, datos_posiciones, barrera_abierta, pos_barrera);
        calculo_presion(posiciones, posiciones_viejas, num, N, pos_barrera, presion);
        presion_sumada[0] += presion[0];    
        presion_sumada[1] += presion[1];
 
        particulas_izquierda(posiciones, N, num, pos_barrera, n_izq);
        if (i%2000 == 0)
        {
            entropia_fichero << i << " , " << n_izq[0] << " , " << n_izq[1] << " , " << entropy(num[0], num[1], n_izq[0], n_izq[1], N) << endl;
            presion_fichero << i << " , " << presion_sumada[0] << " , " << presion_sumada[1] << endl;
            presion_sumada[0] = presion_sumada[1] = 0;
        }

    }

    for (int i = 0; i < tipos; i++)
    {
        datos_posiciones[i].close();
    }

    barrera_abierta_fichero.close();
    entropia_fichero.close();
    presion_fichero.close();

    return 0;
}

void inicializar_posiciones(int posiciones[][2], int N[2], int num[], int tipos, ofstream* datos_posiciones)
{
    bool diferente;
    int contador = 0;

    srand(time(NULL));

    for (int i = 0; i < (suma_array(num,tipos)); i++)
    {
        diferente = false;
        while (diferente == false)
        {
            diferente = true;

            posiciones[i][0] = floor(1.0*N[0]*rand()/RAND_MAX);
            posiciones[i][1] = floor(1.0*N[1]*rand()/RAND_MAX);

            for (int j = 0; j < i; j++)
            {
                if (posiciones[i][0]==posiciones[j][0] && posiciones[i][1]==posiciones[j][1])
                {
                    diferente = false;
                    break;
                }
            }
        }
    } 
    for (int j = 0; j < tipos; j++)
    {
        for(int i=contador; i < suma_array(num,j+1); i++)
        {
            datos_posiciones[j] << posiciones[i][0]+1 << " , " << posiciones[i][1]+1 << endl;
        }
        contador = suma_array(num,j+1);
    }
}

void actualizar_posiciones(int posiciones[][2], int posiciones_viejas[][2], int N[2], int num[], int v[], int tipos, ofstream* datos_posiciones, bool barrera_abierta, double pos_barrera)
{
    double r, posiciones_aux[2];
    bool ocupado;
    int contador = 0;

    for (int j=0; j<tipos; j++)
    {
        for (int i = contador; i < suma_array(num,j+1); i++)
        {
            if (rodeado(posiciones, N, num, i, v[j], tipos, barrera_abierta, pos_barrera))
            {
                datos_posiciones[j] << posiciones[i][0]+1 << " , " << posiciones[i][1]+1 << endl;
                continue;
            }
            
            ocupado = true;

            while (ocupado == true)
            {
                ocupado = false;

                r = 1.0*rand()/RAND_MAX;
                //cout << r << endl;

                if (r < 0.25)
                {
                    posiciones_aux[0] = posiciones[i][0] + v[j];
                    posiciones_aux[1] = posiciones[i][1];
                }
                else if (r < 0.5)
                {
                    posiciones_aux[0] = posiciones[i][0] - v[j];
                    posiciones_aux[1] = posiciones[i][1];
                }
                else if (r < 0.75)
                {
                    posiciones_aux[0] = posiciones[i][0];
                    posiciones_aux[1] = posiciones[i][1] + v[j];
                }
                else
                {
                    posiciones_aux[0] = posiciones[i][0];
                    posiciones_aux[1] = posiciones[i][1] - v[j];
                }

                if (barrera_abierta == false)
                {
                    if (demonio(posiciones[i][0], posiciones_aux[0], pos_barrera) == true)
                    {
                        ocupado = true;
                        continue;
                    }
                }

                if (posiciones_aux[0] < 0 || posiciones_aux[0] >= N[0] || posiciones_aux[1] < 0 || posiciones_aux[1] >= N[1])
                {
                    ocupado = true;
                    continue;
                }

                for (int k = 0; k < suma_array(num,tipos); k++)
                {
                    if (posiciones_aux[0] == posiciones[k][0] && posiciones_aux[1] == posiciones[k][1])
                    {
                        ocupado = true;
                        continue;
                    }
                }  
            }
            posiciones_viejas[i][0] = posiciones[i][0];
            posiciones_viejas[i][1] = posiciones[i][1];
            posiciones[i][0] = posiciones_aux[0];
            posiciones[i][1] = posiciones_aux[1];
            datos_posiciones[j] << posiciones[i][0]+1 << " , " << posiciones[i][1]+1 << endl;
        }
        contador = suma_array(num,j+1);
    }
}

bool rodeado(int posiciones[][2], int N[2], int num[], int i, int v, int tipos, bool barrera_abierta, double pos_barrera)
{
    bool rodeado[4] = {false, false, false, false};
    int pos_proximas[4][2] = {{posiciones[i][0]+v, posiciones[i][1]}, {posiciones[i][0]-v, posiciones[i][1]}, {posiciones[i][0], posiciones[i][1]+v}, {posiciones[i][0], posiciones[i][1]-v}};

    for (int j = 0; j < 4; j++)
    {
        if (pos_proximas[j][0] < 0 || pos_proximas[j][0] >= N[0] || pos_proximas[j][1] < 0 || pos_proximas[j][1] >= N[1])
        {
            rodeado[j] = true;
            continue;
        }
        else
        {
            for (int k = 0; k < suma_array(num,tipos); k++)
            {
                if (pos_proximas[j][0] == posiciones[k][0] && pos_proximas[j][1] == posiciones[k][1])
                {
                    rodeado[j] = true;
                    break;
                }
            }
        }
    }

    if (barrera_abierta == false)
    {
        if (posiciones[i][0]+1 < pos_barrera && pos_proximas[0][0]+1 > pos_barrera)
        {
            rodeado[0] = true;
        }
        else if (posiciones[i][0]+1 > pos_barrera && pos_proximas[0][0]+1 < pos_barrera)
        {
            rodeado[0] = true;
        }
        if (posiciones[i][0]+1 < pos_barrera && pos_proximas[1][0]+1 > pos_barrera)
        {
            rodeado[1] = true;
        }
        else if (posiciones[i][0]+1 > pos_barrera && pos_proximas[1][0]+1 < pos_barrera)
        {
            rodeado[1] = true;
        }
    }

    if (rodeado[0] && rodeado[1] && rodeado[2] && rodeado[3])
    {
        return true;
    }
    else
    {
        return false;
    }
}

int suma_array(int array[], int n)
{
    int suma = 0;

    for (int i = 0; i < n; i++)
    {
        suma += array[i];
    }

    return suma;
}

double posicion_barrera(int N)
{
    if (N%2 == 0)
    {
        return (N/2)+0.5;
    }
    else
    {
        return (1.0*N/2);
    }
}

// Función hecha solo para tipos = 2.
bool barrera(int posiciones[][2], int N[2], int num[], double pos_barrera, int v[], int izqda)
{
    return true;
}

// Función hecha solo para tipos = 2.
bool demonio(int posicion_vieja, int posicion_nueva, double pos_barrera)
{
    if( (posicion_vieja+1 < pos_barrera) && (posicion_nueva+1 > pos_barrera) )
    {
        return true;
    }
    else if( (posicion_vieja+1 > pos_barrera) && (posicion_nueva+1 < pos_barrera) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Función que calcula el logaritmo del producto (N*(N-1)*...*(N-M+1))

double log_prod(int N, int M)
{
    double log_prod = 0;

    for (int i = N; i > (N-M); i--)
    {
        log_prod += log(i);
    }

    return log_prod;
}

//Función que calcula el logaritmo de un factorial
double log_factorial(int n)
{
    double log_fact = 0;

    for (int i = 1; i <= n; i++)
    {
        log_fact += log(i);
    }

    return log_fact;
}

// nA y nB son el número de partículas de cada tipo en la parte izquierda de la barrera
double entropy(int numA, int numB, int nA, int nB, int N[2]) 
{
    double entropia;

    entropia = log_prod(N[0]*N[1]/2, nA+nB) + log_prod(N[0]*N[1]/2, numA+numB-nA-nB) - log_factorial(nA) - log_factorial(nB) - log_factorial(numA-nA) - log_factorial(numB-nB);

    return entropia;
}

// Función que calcula el número de partículas de cada tipo en la parte izquierda de la barrera
void particulas_izquierda(int posiciones[][2], int N[2], int num[2], double pos_barrera, int n_izq[2])
{
    for (int i = 0; i < 2; i++)
    {
        n_izq[i] = 0;
    }

    for (int i = 0; i < 2; i++)
    {
        for (int cont=suma_array(num,i); cont < suma_array(num,i+1); cont++)
        {
            if (posiciones[cont][0]+1 < pos_barrera)
            {
                n_izq[i] += 1;
            }
        }
    }

    return;
}

// Función que calcula la presión 

void calculo_presion(int posiciones[][2], int posiciones_viejas[][2], int num[2], int N[2], double pos_barrera, double presion[2])
{
    presion[0] = 0;
    presion[1] = 0;

    for (int i=0; i < num[0]+num[1]; i++)
    {
        if(posiciones[i][0] == 0)
        {
            if (posiciones_viejas[i][0] != 0)
            {
                presion[0] += 1;
            }
        }
        else if(posiciones[i][0] == N[0]-1)
        {
            if (posiciones_viejas[i][0] != N[0]-1)
            {
                presion[1] += 1;
            }
        }
        else if(posiciones[i][1] == 0)
        {
            if (posiciones_viejas[i][1] != 0)
            {
                if (posiciones[i][0]+1 < pos_barrera)
                {
                    presion[0] += 1;
                }
                else
                {
                    presion[1] += 1;
                }
            }
        }
        else if(posiciones[i][1] == N[1]-1)
        {
            if (posiciones_viejas[i][1] != N[1]-1)
            {
                if (posiciones[i][0]+1 < pos_barrera)
                {
                    presion[0] += 1;
                }
                else
                {
                    presion[1] += 1;
                }
            }
        }
        else if(posiciones[i][0]+1 == floor(pos_barrera))
        {
            if (posiciones_viejas[i][0] < posiciones[i][0])
            {
                presion[0] += 1;
            }
        }
        else if(posiciones[i][0]+1 == ceil(pos_barrera))
        {
            if (posiciones_viejas[i][0] > posiciones[i][0])
            {
                presion[1] += 1;
            }
        }
    }
}

