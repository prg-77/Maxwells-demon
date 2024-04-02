#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define N1 100 //Número de celdas horizontales
#define N2 100 //Número de celdas verticales
#define num N1*N2/500 //Número de partículas   (La división es entera)

using namespace std;

int main()
{
    int posiciones[num][2];
    bool diferente;

    srand(time(NULL));

    for (int i = 0; i < num; i++)
    {
        diferente = false;
        while (diferente == false)
        {
            diferente = true;
            posiciones[i][0] = round(N1*rand()/RAND_MAX);
            posiciones[i][1] = round(N2*rand()/RAND_MAX);
            for (int j = 0; j < i; j++)
            {
                if (posiciones[i][0]==posiciones[j][0] && posiciones[i][1]==posiciones[j][1])
                {
                    diferente = false;
                    break;
                }
            }
        }
        cout << posiciones[i][0] << " " << posiciones[i][1] << endl;
    }

    return 0;


}