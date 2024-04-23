#include <iostream>
#include <numeric>
#include <string>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void inicializar_posiciones(int posiciones[][2], int N[2], int num[], int tipos, ofstream* datos_posiciones);
void actualizar_posiciones(int posiciones[][2], int N[2], int num[], int v[], int tipos, ofstream* datos_posiciones, bool barrera_abierta, double pos_barrera);
bool rodeado(int posiciones[][2], int N[2], int num[], int i, int v, int tipos, bool barrera_abierta, double pos_barrera);
int suma_array(int array[], int n);
double posicion_barrera(int N);
bool barrera(int posiciones[][2], int N[2], int num[], double pos_barrera, int v[], int izqda); // Función hecha solo para tipos = 2.
bool demonio(int posicion_vieja, int posicion_nueva, double pos_barrera); // Función hecha solo para tipos = 2. 

int main()
{
    int N[2], tipos, num_pasos;
    bool barrera_abierta;

    N[0] = 100; // Número de celdas horizontales
    N[1] = 100; // Número de celdas verticales
    tipos = 2; // Número de tipos de partículas
    num_pasos = 2000; // Número de pasos

    int num[tipos], v[tipos];
    double pos_barrera;

    pos_barrera = posicion_barrera(N[0]); //Posición de la barrera
    cout << pos_barrera << endl;
    cout << endl;

    // Si queremos el mismo número de partículas de cada tipo y velocidades ascendentes
    for (int i = 0; i < tipos; i++)
    {
        num[i] = 20; // Número de partículas de cada tipo [num1, num2, ...]
        v[i] = i+1; // Velocidad de las partículas de cada tipo [v1, v2, ...]
    }
    // Si queremos un número de partículas y velocidades diferentes para cada tipo
    // num[0] = 1;
    // num[1] = 1;
    // v[0] = 1;
    // v[1] = 2;

    int posiciones[suma_array(num,tipos)][2];
    ofstream parametros_iniciales, datos_posiciones[tipos];

    parametros_iniciales.open("parametros_iniciales.dat");

    parametros_iniciales << "#N1 N2 num_pasos";

    for (int i = 0; i < tipos; i++)
    {
        parametros_iniciales << " num" << i+1 << " v" << i+1;
    }

    parametros_iniciales << endl;

    parametros_iniciales << N[0] << " , " << N[1] << " , " << num_pasos;

    for (int i = 0; i < tipos; i++)
    {
        parametros_iniciales << " , " << num[i] << " , " << v[i];
    }

    parametros_iniciales.close();

    for (int i = 0; i < tipos; i++)
    {
        datos_posiciones[i].open("posiciones" + to_string(i+1) + ".dat");
    }

    inicializar_posiciones(posiciones, N, num, tipos, datos_posiciones);

    for (int i = 1; i < num_pasos; i++)
    {
        for (int j = 0; j < tipos; j++)
        {
            datos_posiciones[j] << endl;
        }
        barrera_abierta = barrera(posiciones, N, num, pos_barrera, v, 0);
        cout << barrera_abierta << endl;
        cout << endl;
        actualizar_posiciones(posiciones, N, num, v, tipos, datos_posiciones, barrera_abierta, pos_barrera);
    }

    for (int i = 0; i < tipos; i++)
    {
        datos_posiciones[i].close();
    }
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

void actualizar_posiciones(int posiciones[][2], int N[2], int num[], int v[], int tipos, ofstream* datos_posiciones, bool barrera_abierta, double pos_barrera)
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
    int contador;
    bool izquierda;
    int cont;
    double distancia;

    cont = 0;
    contador = 0;

    for (int j=0; j<2; j++)
    {
        if (j == izqda)
        {
            izquierda = true;
        }
        else
        {
            izquierda = false;
        }
        
        for (int i = cont; i < suma_array(num,j+1); i++)
        {
            
            distancia = posiciones[i][0] + 1 - pos_barrera;

            if(izquierda)
            {
                if( (distancia < v[j]) && (distancia > 0) )
                {
                    contador++;
                }
                else if( (distancia > -v[j]) && (distancia < 0) ) 
                {
                    contador--;
                }
            }
            else
            {
                if( (distancia < v[j]) && (distancia > 0) )
                {
                    contador--;
                }
                else if( (distancia > -v[j]) && (distancia < 0) )
                {
                    contador++;
                }
            }
            //cout << contador << endl;
            cont = suma_array(num,j+1);
            
        }
    }
    //cout << endl;
    if (contador > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
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