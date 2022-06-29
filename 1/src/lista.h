#include <stdio.h>
#define FALSE                   0
#define TRUE                    1
#define LST_NO_INI              2
#define LST_POS_INV             3
#define LST_VAC                 4
#define TRI_NO_INI              5
#define STR_NO_PER              -1
#define POS_NULA                NULL
#define ELE_NULO                0

//  Definici�n de tipos utilizados por el TDA Lista:

//  Define las TPosiciones conteniendo un TElemento y una referencia a otra celda.
typedef struct celda {
    int elemento;
    struct celda *proxima_celda;
} *TCelda;

//  Modela las posiciones de una lista.
typedef struct celda *TPosicion;

//  Modela una lista simplemente enlazada y sin centinelas a partir de un
//  entero que representa la cantidad de elementos de la lista y una referencia
//  a la primera celda.
typedef struct lista {
    unsigned int cantidad_elementos;
    TCelda primer_celda;
} *TLista;

//  Definici�n de funciones del TDA Lista:

//  Crea y retorna una lista sin elementos.
TLista crear_lista();

//  Inserta en la lista en la posici�n anterior a pos, el elemento
//  pasado por par�metro elem. Si la posici�n es nula (POS_NULA) y no hay elementos,
//  inserta el elemento en la primera posici�n. Retorna verdadero si la inserci�n
//  se realiz� con �xito y falso en caso contrario.
int l_insertar(TLista lista, TPosicion pos, int elem);

//  Elimina de la lista la posici�n pos. Retorna verdadero si se
//  procede con �xito y falso en caso contrario. Si pos no es v�lida
//  retorna LST_POS_INV.
int l_eliminar(TLista lista, TPosicion pos);

//  Retorna la primera posici�n de la lista. Si la lista est� vac�a retorna
//  POS_NULA.
TPosicion l_primera(TLista lista);

//  Retorna la �ltima posici�n de la lista. Si la lista est� vac�a retorna
//  POS_NULA.
TPosicion l_ultima(TLista lista);

//  Retorna la posici�n anterior a pos en la lista. Si la posici�n es la
//  primera, retorna POS_NULA.
TPosicion l_anterior(TLista lista, TPosicion pos);

//  Retorna la posici�n siguiente a pos en la lista. Si la posici�n es la
//  �ltima, retorna POS_NULA.
TPosicion l_siguiente(TLista lista, TPosicion pos);

//  Retorna el elemento correspondiente a la posici�n pos. Si la posici�n es
//  POS_NULA, retorna ELE_NULO.
int l_recuperar(TLista lista, TPosicion pos);

//  Retorna la cantidad de elementos de la lista.
int l_size(TLista lista);

//  En todos los casos anteriores, salvo en crear_lista, si la lista no est�
//  inicializada, la funci�n se aborta con LST_NO_INI como salida.
//  Todas las funciones de la lista asumen que las posiciones pasadas por par�metro
//  son v�lida, es decir, pertenecen a la lista del argumento.

