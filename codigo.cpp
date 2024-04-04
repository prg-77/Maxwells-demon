#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define N1 3 //Número de celdas horizontales
#define N2 3 //Número de celdas verticales
#define num N1*N2/9 //Número de partículas   (La división es entera)
#define num_pasos 100 //Número de pasos

using namespace std;

void inicializar_posiciones(int posiciones[][2]);
void actualizar_posiciones(int posiciones[][2]);

int main()
{
    int posiciones[num][2];

    inicializar_posiciones(posiciones);

    for (int i = 0; i < num_pasos; i++)
    {
        cout << endl;
        actualizar_posiciones(posiciones);
    }

    return 0;


}

void inicializar_posiciones(int posiciones[][2])
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
        cout << posiciones[i][0]+1 << " " << posiciones[i][1]+1 << endl;
    } 
}

//Falta implementar que cuando una partícula no tiene a donde moverse, se quede en su lugar
void actualizar_posiciones(int posiciones[][2])
{
    double r;
    bool ocupado;
    double posiciones_aux[2];

    for (int i = 0; i < num; i++)
    {
        ocupado = true;

        while (ocupado == true)
        {
            ocupado = false;

            r = 1.0*rand()/RAND_MAX;
            cout << r << endl;

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
        cout << posiciones[i][0]+1 << " " << posiciones[i][1]+1 << endl;
    }
}