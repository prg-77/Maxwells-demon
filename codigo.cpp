#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define N1 3 //Número de celdas horizontales
#define N2 3 //Número de celdas verticales
#define num N1*N2/3 //Número de partículas   (La división es entera)
#define num_pasos 100 //Número de pasos

using namespace std;

void inicializar_posiciones(int posiciones[][2], ofstream& datos_posiciones);
void actualizar_posiciones(int posiciones[][2], ofstream& datos_posiciones);
bool rodeado(int posiciones[][2], int i);

int main()
{
    int posiciones[num][2];
    ofstream parametros_iniciales, datos_posiciones;

    parametros_iniciales.open("parametros_iniciales.dat");
    parametros_iniciales << "#N1 N2 num num_pasos" << endl;
    parametros_iniciales << N1 << " , " << N2 << " , " << num << " , " << num_pasos << endl;
    parametros_iniciales.close();

    datos_posiciones.open("posiciones.dat");
    inicializar_posiciones(posiciones, datos_posiciones);

    for (int i = 0; i < num_pasos; i++)
    {
        datos_posiciones << endl;
        actualizar_posiciones(posiciones, datos_posiciones);
    }

    datos_posiciones.close();
    return 0;
}

void inicializar_posiciones(int posiciones[][2], ofstream& datos_posiciones)
{
    bool diferente;

    srand(time(NULL));

    for (int i = 0; i < num; i++)
    {
        diferente = false;
        while (diferente == false)
        {
            diferente = true;

            posiciones[i][0] = floor(1.0*N1*rand()/RAND_MAX);
            posiciones[i][1] = floor(1.0*N2*rand()/RAND_MAX);

            for (int j = 0; j < i; j++)
            {
                if (posiciones[i][0]==posiciones[j][0] && posiciones[i][1]==posiciones[j][1])
                {
                    diferente = false;
                    break;
                }
            }
        }
        datos_posiciones << posiciones[i][0]+1 << " , " << posiciones[i][1]+1 << endl;
    } 
}

void actualizar_posiciones(int posiciones[][2], ofstream& datos_posiciones)
{
    double r;
    bool ocupado;
    double posiciones_aux[2];

    for (int i = 0; i < num; i++)
    {
        if (rodeado(posiciones, i))
        {
            datos_posiciones << posiciones[i][0]+1 << " , " << posiciones[i][1]+1 << endl;
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
                posiciones_aux[0] = posiciones[i][0] + 1;
                posiciones_aux[1] = posiciones[i][1];
            }
            else if (r < 0.5)
            {
                posiciones_aux[0] = posiciones[i][0] - 1;
                posiciones_aux[1] = posiciones[i][1];
            }
            else if (r < 0.75)
            {
                posiciones_aux[0] = posiciones[i][0];
                posiciones_aux[1] = posiciones[i][1] + 1;
            }
            else
            {
                posiciones_aux[0] = posiciones[i][0];
                posiciones_aux[1] = posiciones[i][1] - 1;
            }

            if (posiciones_aux[0] < 0 || posiciones_aux[0] >= N1 || posiciones_aux[1] < 0 || posiciones_aux[1] >= N2)
            {
                ocupado = true;
                continue;
            }

            for (int j = 0; j < num; j++)
            {
                if (posiciones_aux[0] == posiciones[j][0] && posiciones_aux[1] == posiciones[j][1])
                {
                    ocupado = true;
                    continue;
                }
            }  
        }
        posiciones[i][0] = posiciones_aux[0];
        posiciones[i][1] = posiciones_aux[1];
        datos_posiciones << posiciones[i][0]+1 << " , " << posiciones[i][1]+1 << endl;
    }
}

bool rodeado(int posiciones[][2], int i)
{
    bool rodeado[4] = {false, false, false, false};
    int pos_proximas[4][2] = {{posiciones[i][0]+1, posiciones[i][1]}, {posiciones[i][0]-1, posiciones[i][1]}, {posiciones[i][0], posiciones[i][1]+1}, {posiciones[i][0], posiciones[i][1]-1}};

    for (int j = 0; j < 4; j++)
    {
        if (pos_proximas[j][0] < 0 || pos_proximas[j][0] >= N1 || pos_proximas[j][1] < 0 || pos_proximas[j][1] >= N2)
        {
            rodeado[j] = true;
            continue;
        }
        else
        {
            for (int k = 0; k < num; k++)
            {
                if (pos_proximas[j][0] == posiciones[k][0] && pos_proximas[j][1] == posiciones[k][1])
                {
                    rodeado[j] = true;
                    break;
                }
            }
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