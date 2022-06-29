#include <stdio.h>
#include <stdlib.h>
#define error_arg -4
#define error_arch -5

//Muestra en la pantalla el contenido del archivo especificado por parámetro.
int main (int argc, char * argv[]){
	if ( argc > 1 ) {
		FILE * file = fopen(argv[1], "r");
		if ( file == NULL ) {
			printf("Error: no se pudo abrir el archivo.\n");
			exit(error_arch);
		}
		else {
			char c;
			int finArchivo = fscanf(file, "%c", &c);
			while ( finArchivo != EOF ) {
				printf("%c",c);
				finArchivo = fscanf(file, "%c", &c);
			}
		    fclose(file);
			printf("\n");
		}
	}
	else {
		printf("Error: no se especificó ningún archivo.\n");
		exit(error_arg);
	}
    return 0;
}
