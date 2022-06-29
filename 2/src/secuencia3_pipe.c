#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "constantes.h"

int pa_c[2], pc_d[2], pd_e[2], pe_b[2], pb_c[2], pe_a[2], pa_b[2];

void leer(int pipe, char * caracter, int size) {
	int leidos = read(pipe,caracter,size);
	if ( leidos == -1 ) {
		printf("Error al leer del pipe.\n");
		exit(error_leer_pipe);
	}
}

void escribir(int pipe, char * caracter, int size) {
	int escritos = write(pipe,caracter,size);
	if ( escritos == -1 ) {
		printf("Error al escribir en el pipe.\n");
		exit(error_escribir_pipe);
	}
}

void A() {
	if ( close(pa_b[0]) == -1 && close(pe_a[1]) == -1 ) {
		printf("Error al cerrar el pipe.\n");
		exit(error_close_pipe);
	}
	
	char p;
	while (1) {
		leer(pe_a[0],&p,1);
		printf("A\n");
		escribir(pa_c[1],"C",1);
		leer(pe_a[0],&p,1);
		printf("A\n");
		escribir(pa_b[1],"B",1);
	}
}

void B() {
	if ( close(pb_c[0]) == -1 && close(pa_b[1]) == -1 ) {
		printf("Error al cerrar el pipe.\n");
		exit(error_close_pipe);
	}

	char p;
	while (1) {
		leer(pe_b[0],&p,1);
		printf("B\n");
		escribir(pb_c[1],"C",1);
		leer(pa_b[0],&p,1);
		printf("B\n");
		escribir(pb_c[1],"C",1);
	}
}

void C() {
	if ( close(pc_d[0]) == -1 && close(pb_c[1]) == -1 ) {
		printf("Error al cerrar el pipe.\n");
		exit(error_close_pipe);
	}
	
	char p;
    int i;
	while (1) {
		leer(pa_c[0],&p,1);
		printf("C\n");
		escribir(pc_d[1],"D",1); 
		for ( i = 0; i < 2; i++) { 
            leer(pb_c[0],&p,1);
            printf("C\n");
            escribir(pc_d[1],"D",1);
        }
	}
}

void D() {
	if ( close(pd_e[0]) == -1 && close(pc_d[1]) == -1 ) {
		printf("Error al cerrar el pipe.\n");
		exit(error_close_pipe);
	}
	
	char p;
	while (1) {
		leer(pc_d[0],&p,1);
		printf("D\n");
		escribir(pd_e[1],"E",1);
	}
}

void E() {
	if ( close(pe_a[0]) == -1 && close(pd_e[1]) == -1 ) {
		printf("Error al cerrar el pipe.\n");
		exit(error_close_pipe);
	}
    
	char p;
    int i;
	while (1) {
		leer(pd_e[0],&p,1);
		printf("E\n");
        escribir(pe_b[1],"B",1); 
        for(i=0; i<2; i++){
            leer(pd_e[0],&p,1);
            printf("E\n");
            escribir(pe_a[1],"A",1);
        }
	}
}

int main() {
	if ( !pipe(pa_c) && !pipe(pc_d) && !pipe(pd_e) && !pipe(pe_b) && !pipe(pb_c) && !pipe(pe_a) && !pipe(pa_b) ) {
		int pid;

	    escribir(pe_a[1],"A",1);
		pid = fork();
		if ( !pid ) { //hijo
			A();
			exit(0);
		}
		else if (pid < 0) {
			printf("Error en la creación de procesos.\n");
			exit(error_fork);
		}

		pid = fork();
		if ( !pid ) { //hijo
			B();
			exit(0);
		}
		else if (pid < 0) {
			printf("Error en la creación de procesos.\n");
			exit(error_fork);
		}

		pid = fork();
		if ( !pid ) { //hijo
			C();
			exit(0);
		}
		else if (pid < 0) {
			printf("Error en la creación de procesos.\n");
			exit(error_fork);
		}

		pid = fork();
		if ( !pid ) { //hijo
			D();
			exit(0);
		}
		else if (pid < 0) {
			printf("Error en la creación de procesos.\n");
			exit(error_fork);
		}

		pid = fork();
		if ( !pid ) { //hijo
			E();
			exit(0);
		}
		else if (pid < 0) {
			printf("Error en la creación de procesos.\n");
			exit(error_fork);
		}

		for ( int i = 0; i < 5; i++) wait(NULL);
	}
	else {
		printf("Error al crear el pipe.\n");
		exit(error_pipe);
	}
	return 0;
}
