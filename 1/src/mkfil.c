#include <stdio.h>
#include <stdlib.h>
#define error_arg -4
#define error_arch -5

// crea un archivo con el nombre especificado por parámetro y lo cierra
int main(int argc, char* argv[]){
	if ( argc > 1 ) {
		// especifica un archivo
        FILE * file = fopen(argv[1],"w");
		if ( file == NULL ) {
			printf("Error: no se pudo abrir el archivo.\n");
			exit(-5);
		}
		else fclose(file);
    }
    else {
		printf("Error: no se especificó ningún archivo.\n");
		exit(error_arg);
    }
    return 0;
}
