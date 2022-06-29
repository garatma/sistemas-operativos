#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#define error_fork -1
#define error_archivo_incorrecto -2

int sudoku[9][9];

//  Verifica que el archivo cumpla con las condiciones de formato.
//  NOTA: Un formato válido asume nueve líneas de números intercalados con comas
int leerDeArchivo(int sudoku[9][9], FILE * archivo) {
	int fila = 0, columna = 0, sudokuValido = 1, columnaDeInsercion = 0;
	int hay_para_leer = 1;
	char linea[18];
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

void chequearFila(int fila, FILE * resultados) {
	// chequea que la fila del sudoku tenga todos los dígitos del 1 al 9.
    int columna, sudokuValido=1; int filaAux[9] = {0,0,0,0,0,0,0,0,0}; //el arreglo guarda los dígitos de la fila hasta encontrar alguno repetido.
    while (columna<9 && sudokuValido ) { // si algún hilo encuentra un error, terminal la ejecución
		sudokuValido = filaAux[sudoku[fila][columna]-1]==0; // se indexa el arreglo con el número leído menos 1 ( arreglo desde 0 a 8 )
		if (sudokuValido) filaAux[sudoku[fila][columna]-1] = 1; // no está repetido, entonces se marca como leído
		columna++;
    }
    fprintf(resultados,"%c",sudokuValido+'0');
}

void chequearColumna(int columna, FILE * resultados) {
	// chequea que la columna del sudoku tenga todos los dígitos del 1 al 9.
    int fila=0; int sudokuValido=1; int columnaAux[9] = {0,0,0,0,0,0,0,0,0}; //el arreglo guarda los dígitos de la columna hasta encontrar alguno repetido.
    while(fila<9 && sudokuValido) { // si algún hilo encuentra un error, terminal la ejecución
		sudokuValido = columnaAux[sudoku[fila][columna]-1]==0; // se indexa el arreglo con el número leído menos 1 ( arreglo desde 0 a 8 )
		if (sudokuValido) columnaAux[sudoku[fila][columna]-1] = 1; // no está repetido, entonces se marca como leído
		fila++;
    }
	fprintf(resultados,"%c",sudokuValido+'0');
}

void chequearSubGrilla(int numero_subgrilla,FILE * resultados) {
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
	int sudokuValido = 1;
	while (	fila < filaMax && sudokuValido) {
		while ( columna < columnaMax && sudokuValido) {
			sudokuValido = leidos[sudoku[fila][columna]-1]==0; // se indexa el arreglo con el número leído menos 1 ( arreglo desde 0 a 8 )
			if (sudokuValido) leidos[sudoku[fila][columna]-1] = 1; // no está repetido, entonces se marca como leído
			columna++;
		}
		columna = columnaMin;
		fila++;
	}
	fprintf(resultados,"%c",sudokuValido+'0');
}

void eliminar_archivos_creados(int cant_archivos) {
	int i = 0;
	while ( i < cant_archivos ) {
		char nombre_archivo[2] = "";
		sprintf(nombre_archivo,"%i",i);
		remove(nombre_archivo);
		i++;
	}
}

int main() {  //  para escribir y despues leer de archivo: Abrir para escritura cerrar y abrir para lectura.
	FILE * archivo = fopen("sudoku.txt","r");
//	FILE * resultados = fopen("resultados.txt","w+"); //archivo donde se comparten los resultados entre los procesos
	int sudokuValido = 0;
	if(archivo == NULL) printf("No se encontró el archivo.");
	else {
		sudokuValido = leerDeArchivo(sudoku,archivo);
		fclose(archivo);
		int j = 0;
		if ( sudokuValido ) {
			int i, pid;
			for (i=0; i<9; i++) {
				char nombre_archivo[2] = "";
				sprintf(nombre_archivo,"%i",j);
				FILE * archivo = fopen(nombre_archivo,"w");
				j++;
				pid = fork();
				if (pid<0) {
					fclose(archivo);
					eliminar_archivos_creados(j-1);
					printf("error en fork");
					exit(error_fork);
				}
				else
					if (pid==0) {
						chequearFila(i,archivo);
						fclose(archivo);
						exit(0);
					}
			}
			for (i=0; i<9; i++) {
				char nombre_archivo[2] = "";
				sprintf(nombre_archivo,"%i",j);
				FILE * archivo = fopen(nombre_archivo,"w");
				j++;
				pid = fork();
				if (pid<0) {
					fclose(archivo);
					eliminar_archivos_creados(j-1);
					printf("error en fork");
					exit(error_fork);
				}
				else
					if (pid==0) {
						chequearColumna(i,archivo);
						fclose(archivo);
						exit(0);
					}
			}
			for (i=0; i<9; i++) {
				char nombre_archivo[2] = "";
				sprintf(nombre_archivo,"%i",j);
				FILE * archivo = fopen(nombre_archivo,"w");
				j++;
				pid = fork();
				if (pid<0) {
					fclose(archivo);
					eliminar_archivos_creados(j-1);
					printf("error en fork");
					exit(error_fork);
				}
				else
					if (pid==0) {
						chequearSubGrilla(i,archivo);
						fclose(archivo);
						exit(0);
					}
			}
			for (i = 0; i < 27; i++) wait(NULL);
			i = 0; char caracter;
			while ( i < 27 ) {
				char nombre_archivo[2] = "";
				sprintf(nombre_archivo,"%i",i);
				FILE * archivo = fopen(nombre_archivo,"r");
				if ( sudokuValido ) {
					caracter = fgetc(archivo);
					sudokuValido = caracter == '1';
				}
				fclose(archivo);
				remove(nombre_archivo);
				i++;
			}
			if ( sudokuValido ) printf("El sudoku es válido\n");
			else printf("El sudoku no es válido\n");
		}
		else {
			printf("Formato de archivo incorrecto.");
			exit(error_archivo_incorrecto);
		}
	}
    return 0;
}
