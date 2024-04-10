#include <iostream>

using namespace std;

void funcion(int* p);
void function(int* b);
void funcion3(int c[]);

int main()
{
    int a=5;
    int b[2] = {1,2};
    int c[2] = {3,4};
    int* p; //puntero 
    
    p = &a; //p apunta a la dirección de a

    cout << p << endl; //imprime la dirección de a
    cout << *p << endl; //imprime el valor de a

    funcion(p);

    cout << p << endl;
    cout << *p << endl; 

    function(b);

    cout << b[0] << " " << b[1] << " " << b[2] << " " << b[3] << endl;

    funcion3(c);

    cout << c[0] << " " << c[1] << " " << c[2] << " " << c[3] << endl;

    //Con los ofstream por alguna razon que desconozco, para poder modificarlos es necesario pasarlos como punteros, no funciona el paso por referencia implícito de C++ ni el explícito.
}

void funcion(int* p)
{
    *p = 10;
}

void function(int* b)
{
    b[0] = 10;
    b[2] = 30;
    b[3] = 20;
}

void funcion3(int c[])
{
    c[0] = 10;
    c[2] = 30;
    c[3] = 20;
}