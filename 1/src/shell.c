#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define max 100
#define error_fork -1
#define error_exec -2

void proceso_parametro(char comando[max], char parametro[max]) {
	scanf("%s",parametro);
	int pid = fork();
	if ( pid < 0 ) {
		printf("Error en la creación del proceso que ejecuta el comando.\n");
		exit(error_fork);
	}
	else {
		if ( pid > 0 ) wait(NULL);
		else {
			// proceso hijo
			int estado = execl(comando,comando,parametro,NULL);
			if ( estado == -1 ) {
				printf("Error en la creación del proceso que ejecuta el comando.\n");
				exit(error_exec);
			}
		}
	}
}

void proceso_nada(char comando[max]) {
	int pid = fork();
	if ( pid < 0 ) {
		printf("Error en la creación del proceso que ejecuta el comando.\n");
		exit(error_fork);
	}
	else {
		if ( pid > 0 ) wait(NULL);
		else {
			// proceso hijo
			int estado = execl(comando,comando,NULL);
			if ( estado == -1 ) {
				printf("Error en la creación del proceso que ejecuta el comando.\n");
				exit(error_exec);
			}
		}
	}
}

void limpiar_strings(char * s1, char * s2) {
	for ( int i = 0; i < max; i++ ) {
		s1[i] = '\0';
		s2[i] = '\0';
	}
}

int main(int argc, char * argv[]) {
	char comando[max] = "";
	char parametro[max] = "";
	int terminar = 0;
	while(!terminar){
		printf("Ingrese un comando: ");
		scanf("%s",comando);
			 if ( !strcmp(comando,"exit") ) terminar = 1;
   		else if ( !strcmp(comando,"help") ) proceso_nada(comando);
			 else {
				 if ( !strcmp(comando,"rmdir") || !strcmp(comando,"lsdir") || !strcmp(comando,"mkdir") || !strcmp(comando,"mkfil") || !strcmp(comando,"lsfil") ) proceso_parametro(comando,parametro);
			     else printf("No se reconoce a -%s- como un comando.\n",comando);
			 }
		limpiar_strings(comando,parametro);
	}
	return 0;
}
