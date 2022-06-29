#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#define error_archivo_incorrecto -2
#define error_hilo -3

int sudokuValido=1;
sem_t mutex;
int sudoku[9][9];
pthread_t thread[27];

//  Verifica que el archivo cumpla con las condiciones de formato.
//  NOTA: Un formato válido asume nueve líneas de números intercalados con comas
int leerDeArchivo( FILE * archivo) {
	int fila = 0, columna = 0, columnaDeInsercion = 0;
	int hay_para_leer = 1;
	char linea[20];
	hay_para_leer = fscanf(archivo, "%s",linea);
	while ( hay_para_leer && fila < 9 && sudokuValido && strlen(linea) == 17 ) {
		while (columna < 17) {
			// entre la columna 0 y 17 se describe el sudoku
			if ( columna%2 == 0 ) {
				// la posición es par -> tiene que ser un dígito
				sudokuValido = '0' < linea[columna] && linea[columna] <= '9';
				sudoku[fila][columnaDeInsercion++] = linea[columna] - '0';
			}
			else sudokuValido = linea[columna] == ','; // la posición es impar -> tiene que ser una coma
			columna++;
		}
		if ( sudokuValido ) {
			fila++;
			columna = 0;
			columnaDeInsercion = 0;
			for ( int i = 0; i < 18; i++ ) linea[i] = '\0';
			fscanf(archivo,"%s",linea);
		}
	}
	return sudokuValido;
}

void * chequearFila(int fila) {
	// chequea que la fila del sudoku tenga todos los dígitos del 1 al 9.
    int columna, sv=1; int filaAux[9] = {0,0,0,0,0,0,0,0,0}; //el arreglo guarda los dígitos de la fila hasta encontrar alguno repetido.
    while (columna<9 && sv && sudokuValido) { // si algún hilo encuentra un error, terminal la ejecución
		sv = filaAux[sudoku[fila][columna]-1]==0; // se indexa el arreglo con el número leído menos 1 ( arreglo desde 0 a 8 )
		if (sv) filaAux[sudoku[fila][columna]-1] = 1; // no está repetido, entonces se marca como leído
		columna++;
    }
	sem_wait(&mutex);
	if (!sv) sudokuValido = sv;
	sem_post(&mutex);
    return NULL;
}

void * chequearColumna(int columna) {
	// chequea que la columna del sudoku tenga todos los dígitos del 1 al 9.
    int fila=0; int sv=1; int columnaAux[9] = {0,0,0,0,0,0,0,0,0}; //el arreglo guarda los dígitos de la columna hasta encontrar alguno repetido.
    while(fila<9 && sv && sudokuValido) { // si algún hilo encuentra un error, terminal la ejecución
		sv = columnaAux[sudoku[fila][columna]-1]==0; // se indexa el arreglo con el número leído menos 1 ( arreglo desde 0 a 8 )
		if (sv) columnaAux[sudoku[fila][columna]-1] = 1; // no está repetido, entonces se marca como leído
		fila++;
    }
	sem_wait(&mutex);
	if (!sv) sudokuValido = sv;
	sem_post(&mutex);
    return NULL;
}

void * chequearSubGrilla(int numero_subgrilla) {
	//Verifica que las subgrilla de 3x3 contiene todos los dígitos del 1 al 9.
	int fila, filaMax, columna, columnaMin, columnaMax;
	int leidos [9] = {0,0,0,0,0,0,0,0,0};
	switch(numero_subgrilla){
		case 0:
			fila = 0;
			columna = 0;
			break;
		case 1:
			fila = 0;
			columna = 3;
			break;
		case 2:
			fila = 0;
			columna = 6;
			break;
		case 3:
			fila = 3;
			columna = 0;
			break;
		case 4:
			fila = 3;
			columna = 3;
			break;
		case 5:
			fila = 3;
			columna = 6;
			break;
		case 6:
			fila = 6;
			columna = 0;
			break;
		case 7:
			fila = 6;
			columna = 3;
			break;
		case 8:
			fila = 6;
			columna = 6;
			break;
	}
	filaMax = fila + 3; columnaMax = columna + 3; // subgrillas de 3x3
	columnaMin = columna; // para resetear la columna en el while
	int sv = 1;
	while (	fila < filaMax && sv && sudokuValido ) {
		while ( columna < columnaMax && sv && sudokuValido ) {
			sv = leidos[sudoku[fila][columna]-1]==0; // se indexa el arreglo con el número leído menos 1 ( arreglo desde 0 a 8 )
			if (sv) leidos[sudoku[fila][columna]-1] = 1; // no está repetido, entonces se marca como leído
			columna++;
		}
		columna = columnaMin;
		fila++;
	}
	sem_wait(&mutex);
	if (!sv) sudokuValido = sv;
	sem_post(&mutex);
	return NULL;
}

int main() {  //  para escribir y despues leer de archivo: Abrir para escritura cerrar y abrir para lectura.
	FILE * archivo = fopen("sudoku.txt","r");
	if(archivo == NULL) printf("No se encontró el archivo.");
	else {
		sudokuValido = leerDeArchivo(archivo);
		fclose(archivo);
		if ( sudokuValido ) {
			FILE * resultados = fopen("resultados.txt","w+"); //archivo donde se comparten los resultados entre los procesos. formato: fila/columna/subgrilla
			fwrite("000",1,3,resultados);
			rewind(resultados);
			sem_init(&mutex,0,1);
			int i = 0,error = 0, j=0;
			while(i<9 && !error){
				error = pthread_create(&thread[j++],NULL,chequearFila,i);
				if ( !error ) error = pthread_create(&thread[j++],NULL,chequearColumna,i);
				if ( !error ) error = pthread_create(&thread[j++],NULL,chequearSubGrilla,i);
				i++;
			}
			if ( error ) {
				printf("Error en la creación de hilos de verificación del sudoku.\n");
				exit(error_hilo);
			}
			for (i =0; i<27; i++) pthread_join(thread[i],NULL);
			if ( sudokuValido ) printf("El sudoku es válido.");
			else printf("El sudoku no es válido.");
			remove("resultados.txt");
		}
		else {
			printf("Formato de archivo incorrecto.");
			exit(error_archivo_incorrecto);
		}
	}
    return 0;
}
