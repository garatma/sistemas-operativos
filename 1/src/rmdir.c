#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define error_dir -3
#define error_arg -4
#define max 200

// como la función de librería rmdir elimina solo directorios vacios, primero se elimina su contenido.
void eliminar_contenido(char * ruta) {
    DIR * dir;
    struct dirent * dp;
    struct stat statbuf;
    dir = opendir(ruta);
    if ( dir != NULL ) {
        while ( (dp = readdir(dir)) != NULL ) { //obtener contenido hasta que no haya nada más que leer
            //eliminar los directorios . y .. ya que no se necesitan para la recursión
            if (  strcmp(dp->d_name,".") && strcmp(dp->d_name,"..") ) {
                //preparar la ruta abosluta del elemento leido para poder obtener información de él
                char ruta_copia2[max];
                strcpy(ruta_copia2,ruta);
                strcat(ruta_copia2,"/");
                strcat(ruta_copia2,dp->d_name);
                stat(ruta_copia2, &statbuf);
                if ( S_ISDIR(statbuf.st_mode) == 1 ) { // es un directorio
                    //preparar la ruta nueva para la recursión
                    char ruta_nueva[max] = "";
                    char ruta_copia[max] = "";
                    strcpy(ruta_copia,ruta);
                    strcat(ruta_copia,"/");
                    strcat(ruta_nueva,dp->d_name);
                    strcat(ruta_copia,ruta_nueva);
                    eliminar_contenido(ruta_copia);
                    // el directorio está vacío
                    rmdir(ruta_copia);
                }
                else remove(ruta_copia2); // es un archivo
            }
        }
        closedir(dir);
    }
    else {
        printf("Error: no se encontró el directorio.\n");
        exit(error_dir);
    }
}

int main (int argc, char * argv[]) {
    if ( argc > 1 ) {
        eliminar_contenido(argv[1]);
        // el contenido del directorio fue eliminado
        rmdir(argv[1]);
    }
    else {
		printf("Error: no se especificó ningún directorio.\n");
		exit(error_arg);
    }
    return 0;
}
