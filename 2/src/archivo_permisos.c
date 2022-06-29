#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "constantes.h"

mode_t mu = 0, mg = 0, mo = 0;

void asignar_mu(char * argv){
	int i, size = strlen(argv);
	for ( i=0; i<size; i++){
		if (argv[i] == 'r' || argv[i] == 'R') mu |= S_IRUSR;
		else if(argv[i] == 'w' || argv[i] == 'W') mu |= S_IWUSR;
		else if(argv[i] == 'x' || argv[i] == 'X') mu |= S_IXUSR;
		else {
			printf("No existe tal modo de usuario.\n");
			exit(error_modo);
		}
	}
}

void asignar_mg(char * argv){
	int i, size = strlen(argv);
	for ( i=0; i<size; i++){
		if (argv[i] == 'r' || argv[i] == 'R') mg |= S_IRGRP;
		else if(argv[i] == 'w' || argv[i] == 'W') mg |= S_IWGRP;
		else if(argv[i] == 'x' || argv[i] == 'X') mg |= S_IXGRP;
		else {
			printf("No existe tal modo de grupo.\n");
			exit(error_modo);
		}
	}
}
void asignar_mo(char * argv){
	int i, size = strlen(argv);
	for ( i=0; i<size; i++){
		if (argv[i] == 'r' || argv[i] == 'R') mo |= S_IROTH;
		else if(argv[i] == 'w' || argv[i] == 'W') mo |= S_IWOTH;
		else if(argv[i] == 'x' || argv[i] == 'X') mo |= S_IXOTH;
		else {
			printf("No existe tal modo de otros.\n");
			exit(error_modo);
		}
	}
}

int main(int argc, char * argv[]) {
	if ( argc < 5 ) {
		printf("Se deben especificar un archivo y tres permisos.\n");
		exit(error_parametros);
	}
	if ( strcmp(argv[2],"-") ) asignar_mu(argv[2]);
	if ( strcmp(argv[3],"-") ) asignar_mg(argv[3]);
	if ( strcmp(argv[4],"-") ) asignar_mo(argv[4]);
	if ( mu != 0 || mg != 0 || mo != 0 ) { // si no se asignó ningún permiso, no hacer nada
		if ( chmod(argv[1], mu | mg | mo) == -1 ) {
			int errnum = errno;
			if ( errnum == ENOENT ) {
				printf("No se pudo encontrar el archivo.\n");
				exit(error_archivo);
			}
			printf("Error al asignar los privilegios al archivo.\n");
			exit(error_asignacion);
		}
		printf("Permiso asignado correctamente.\n");
	}
	return 0;
}
