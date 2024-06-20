#include <iostream>
#include <numeric>
#include <string>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void inicializar_posiciones(int posiciones[][2], int N[2], int num[], int tipos, ofstream* datos_posiciones, bool video, int rep);
void actualizar_posiciones(int posiciones[][2], int posiciones_viejas[][2], int N[2], int num[], int v[], int tipos, ofstream* datos_posiciones, bool barrera_abierta, double pos_barrera, bool video, int rep);
bool rodeado(int posiciones[][2], int N[2], int num[], int i, int v, int tipos, bool barrera_abierta, double pos_barrera);
int suma_array(int array[], int n);
double posicion_barrera(int N);
bool barrera(int posiciones[][2], int N[2], int num[], double pos_barrera, int v[], int izqda, bool hay_demonio); // Función hecha solo para tipos = 2.
bool demonio(int posicion_vieja, int posicion_nueva, double pos_barrera); // Función hecha solo para tipos = 2. 
double log_prod(int N, int M);
double log_factorial(int n);
double entropy(int numA, int numB, int nA, int nB, int N[2]);
void particulas_izquierda(int posiciones[][2], int N[2], int num[], double pos_barrera, int n_izq[]);
void calculo_presion(int posiciones[][2], int posiciones_viejas[][2], int num[2], int N[2], double pos_barrera, double presion[2]);

int main()
{
    int N[2], tipos, num_pasos, rep, intervalo, numero_puntos;
    bool barrera_abierta, video, hay_demonio;

    N[0] = 15; // Número de celdas horizontales
    N[1] = 15; // Número de celdas verticales
    tipos = 2; // Número de tipos de partículas
    num_pasos = 20000; // Número de pasos
    rep = 100; // Número de repeticiones
    video = false; // si es true, se guardan los datos de la primera simulación para hacer un video, si es false, no se guardan
    intervalo = 1000; // Intervalo de tiempo para calcular la entropía y la presión
    numero_puntos = num_pasos/intervalo; // Número de puntos para calcular la entropía
    hay_demonio = true; // Si es true, hay demonio, si es false, no hay demonio

    int num[tipos], v[tipos], n_izq[tipos];
    double pos_barrera, presion[tipos], presion_sumada[tipos];
    double entropia[rep][numero_puntos], presion_iesima[rep][numero_puntos][2];

    pos_barrera = posicion_barrera(N[0]); //Posición de la barrera
    cout << pos_barrera << endl;
    cout << endl;

    presion_sumada[0] = presion_sumada[1] = 0;

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

    int posiciones[suma_array(num,tipos)][2], posiciones_viejas[suma_array(num,tipos)][2];
    ofstream parametros_iniciales, datos_posiciones[tipos], barrera_abierta_fichero, entropia_fichero, presion_fichero;

    parametros_iniciales.open("parametros_iniciales.dat");

    parametros_iniciales << "#N1 N2 num_pasos repeticiones pos_barrera";

    for (int i = 0; i < tipos; i++)
    {
        parametros_iniciales << " num" << i+1 << " v" << i+1;
    }

    parametros_iniciales << endl;

    parametros_iniciales << N[0] << " , " << N[1] << " , " << num_pasos << " , " << rep << " , " << pos_barrera;

    for (int i = 0; i < tipos; i++)
    {
        parametros_iniciales << " , " << num[i] << " , " << v[i];
    }

    parametros_iniciales.close();

    for (int i = 0; i < tipos; i++)
    {
        datos_posiciones[i].open("posiciones" + to_string(i+1) + ".dat");
    }

    if(hay_demonio == true)
    {
        barrera_abierta_fichero.open("barrera_abierta.dat");
        entropia_fichero.open("entropia.dat");
        presion_fichero.open("presion.dat");
    }
    else
    {
        barrera_abierta_fichero.open("barrera_abierta_sin_demonio.dat");
        entropia_fichero.open("entropia_sin_demonio.dat");
        presion_fichero.open("presion_sin_demonio.dat");
    }

    entropia_fichero <<  "# Paso   Entropía   Error" << endl;
    presion_fichero << "# Paso   Presión Izq   Presión Dcha   Error Izq   Error Dcha" << endl;


    srand(time(NULL)); // Semilla para el generador de números aleatorios

    for (int k = 1; k <= rep; k++)
    {
        cout << k << endl;
        inicializar_posiciones(posiciones, N, num, tipos, datos_posiciones, video, k);

        particulas_izquierda(posiciones, N, num, pos_barrera, n_izq);
        entropia[k-1][0] = entropy(num[0], num[1], n_izq[0], n_izq[1], N);

        for (int i = 1; i < num_pasos; i++)
        {
            if (video == true && k == 1)
            {
                for (int j = 0; j < tipos; j++)
                {
                    datos_posiciones[j] << endl;
                }
            }
                
            barrera_abierta = barrera(posiciones, N, num, pos_barrera, v, 0, hay_demonio);
            if (video == true && k == 1)
            {
                barrera_abierta_fichero << barrera_abierta << endl;
            }
            actualizar_posiciones(posiciones, posiciones_viejas, N, num, v, tipos, datos_posiciones, barrera_abierta, pos_barrera, video, k);
            calculo_presion(posiciones, posiciones_viejas, num, N, pos_barrera, presion);
            presion_sumada[0] += presion[0];    
            presion_sumada[1] += presion[1];
    
            if (i%intervalo == 0)
            {
                particulas_izquierda(posiciones, N, num, pos_barrera, n_izq);
                entropia[k-1][i/intervalo] = entropy(num[0], num[1], n_izq[0], n_izq[1], N);
                presion_iesima[k-1][(i/intervalo)-1][0] = presion[0];
                presion_iesima[k-1][(i/intervalo)-1][1] = presion[1];
                presion_sumada[0] = presion_sumada[1] = 0;
            }
        }
    }

    for (int i = 0; i < tipos; i++)
    {
        datos_posiciones[i].close();
    }

    double entropia_media, presion_media[2];
    double error_entropia, error_presion[2];

    for (int i = 0; i < num_pasos; i=i+intervalo)
    {
        entropia_media = 0;
        presion_media[0] = 0;
        presion_media[1] = 0;
        error_entropia = 0;
        error_presion[0] = 0;
        error_presion[1] = 0;

        for (int j = 0; j < rep; j++)
        {
            entropia_media += entropia[j][i/intervalo];
            presion_media[0] += presion_iesima[j][i/intervalo][0];
            presion_media[1] += presion_iesima[j][i/intervalo][1];
        }

        entropia_media = entropia_media/rep;
        presion_media[0] = presion_media[0]/rep;
        presion_media[1] = presion_media[1]/rep;

        for (int j = 0; j < rep; j++)
        {
            error_entropia += pow(entropia[j][i/intervalo] - entropia_media, 2);
            error_presion[0] += pow(presion_iesima[j][i/intervalo][0] - presion_media[0], 2);
            error_presion[1] += pow(presion_iesima[j][i/intervalo][1] - presion_media[1], 2);
        }

        error_entropia = sqrt(error_entropia/(rep-1));  // Desviación estándar
        error_presion[0] = sqrt(error_presion[0]/(rep-1));
        error_presion[1] = sqrt(error_presion[1]/(rep-1));

        error_entropia = error_entropia/sqrt(rep); // Error
        error_presion[0] = error_presion[0]/sqrt(rep);
        error_presion[1] = error_presion[1]/sqrt(rep);

        entropia_fichero << i << " , " << entropia_media << " , " << error_entropia << endl;
        if (i+intervalo<num_pasos)
        {
            presion_fichero << i+intervalo << " , " << presion_media[0] << " , " << presion_media[1] << "  , " << error_presion[0] << " , " << error_presion[1] << endl;
        }
    }

    barrera_abierta_fichero.close();
    entropia_fichero.close();
    presion_fichero.close();

    return 0;
}

void inicializar_posiciones(int posiciones[][2], int N[2], int num[], int tipos, ofstream* datos_posiciones, bool video, int rep)
{
    bool diferente;
    int contador = 0;

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
    if (video == true && rep == 1)
    {
        for (int j = 0; j < tipos; j++)
        {
            for(int i=contador; i < suma_array(num,j+1); i++)
            {
                datos_posiciones[j] << posiciones[i][0]+1 << " , " << posiciones[i][1]+1 << endl;
            }
            contador = suma_array(num,j+1);
        }
    }
}

void actualizar_posiciones(int posiciones[][2], int posiciones_viejas[][2], int N[2], int num[], int v[], int tipos, ofstream* datos_posiciones, bool barrera_abierta, double pos_barrera, bool video, int rep)
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
                if(video == true && rep == 1)
                {
                    datos_posiciones[j] << posiciones[i][0]+1 << " , " << posiciones[i][1]+1 << endl;
                }
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

                if(posiciones_aux[0] < 0 && j==1)
                {
                    posiciones_aux[0] ++;
                }
                if(posiciones_aux[0] >= N[0] && j==1)
                {
                    posiciones_aux[0] --;
                }
                if(posiciones_aux[1] < 0 && j==1)
                {
                    posiciones_aux[1] ++;
                }
                if(posiciones_aux[1] >= N[1] && j==1)
                {
                    posiciones_aux[1] --;
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
            if (video == true && rep == 1)
            {
                datos_posiciones[j] << posiciones[i][0]+1 << " , " << posiciones[i][1]+1 << endl;
            }
        }
        contador = suma_array(num,j+1);
    }
}

bool rodeado(int posiciones[][2], int N[2], int num[], int i, int v, int tipos, bool barrera_abierta, double pos_barrera)
{
    bool esta_rodeado[4] = {false, false, false, false};
    int pos_proximas[4][2] = {{posiciones[i][0]+v, posiciones[i][1]}, {posiciones[i][0]-v, posiciones[i][1]}, {posiciones[i][0], posiciones[i][1]+v}, {posiciones[i][0], posiciones[i][1]-v}};

    for (int j = 0; j < 4; j++)
    {
        if (pos_proximas[j][0] < 0 || pos_proximas[j][0] >= N[0] || pos_proximas[j][1] < 0 || pos_proximas[j][1] >= N[1])
        {
            esta_rodeado[j] = true;
            continue;
        }
        else
        {
            for (int k = 0; k < suma_array(num,tipos); k++)
            {
                if (pos_proximas[j][0] == posiciones[k][0] && pos_proximas[j][1] == posiciones[k][1])
                {
                    esta_rodeado[j] = true;
                    break;
                }
            }
        }
    }

    if (barrera_abierta == false)
    {
        if (posiciones[i][0]+1 < pos_barrera && pos_proximas[0][0]+1 > pos_barrera)
        {
            esta_rodeado[0] = true;
        }
        else if (posiciones[i][0]+1 > pos_barrera && pos_proximas[0][0]+1 < pos_barrera)
        {
            esta_rodeado[0] = true;
        }
        if (posiciones[i][0]+1 < pos_barrera && pos_proximas[1][0]+1 > pos_barrera)
        {
            esta_rodeado[1] = true;
        }
        else if (posiciones[i][0]+1 > pos_barrera && pos_proximas[1][0]+1 < pos_barrera)
        {
            esta_rodeado[1] = true;
        }
    }

    
    if (esta_rodeado[0] && esta_rodeado[1] && esta_rodeado[2] && esta_rodeado[3])
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
bool barrera(int posiciones[][2], int N[2], int num[], double pos_barrera, int v[], int izqda, bool hay_demonio)
{
    int contador;
    bool izquierda;
    int cont;
    double distancia;

    cont = 0;
    contador = 0;

    if(hay_demonio == false)
    {
        return true;
    }

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
        return true; // Barrera abierta
    }
    else
    {
        return false; // Barrera cerrada
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

