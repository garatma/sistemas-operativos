#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define error_dir -3
#define error_arg -4

// lista el directorio pasado por parámetro
int main(int argc, char * argv[]) {
    DIR * directorio;
    if ( argc == 1 ) {
		printf("Error: no se especificó ningún archivo.\n");
		exit(error_arg);
    }
    else directorio = opendir(argv[1]); // abrir el directorio argumento
    if ( directorio != NULL ) {
        struct dirent * elem_dir; // para almacenar los elementos del diretorio
        elem_dir = readdir(directorio);
        int hay_elementos = elem_dir!=NULL;
        while( hay_elementos ) { // leer hasta que no haya más elementos en el directorio
            if ( strcmp(elem_dir->d_name,".") && strcmp(elem_dir->d_name,"..") ) printf("%s\n",elem_dir->d_name); // no incluir . ni ..
            elem_dir = readdir(directorio);
            hay_elementos = elem_dir!=NULL;
        }
        closedir(directorio);
    }
    else {
        printf("Error: no se encontró el directorio.\n");
        exit(error_dir);
    }
    return 0;
}
