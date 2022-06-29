#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#define error_dir -3
#define error_arg -4

//Crea un directorio nuevo con el nombre especificado por parámetro.
int main(int argc, char* argv[]){
	if ( argc > 1 ) {
        int resultado = mkdir(argv[1],0777);
        if ( resultado == -1 ) {
            printf("Error: no se pudo crear el directorio.\n");
            exit(error_dir);
        }
    }
    else {
		printf("Error: no se especificó ningún archivo.\n");
		exit(error_arg);
    }
    return 0;
}
