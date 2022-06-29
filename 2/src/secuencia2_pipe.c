#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "constantes.h"

int pabc_abc[2], pabc_d[2], pd_e[2], pe_abc[2];

void escribir(int pipe, char * caracter, int size) {
	int escritos = write(pipe,caracter,size);
	if ( escritos == -1 ) {
		printf("Error al escribir en el pipe.\n");
		exit(error_escribir_pipe);
	}
}

void A() {
	if ( close(pabc_d[0]) == -1 && close(pe_abc[1]) == -1 ) {
		printf("Error al cerrar el pipe.\n");
		exit(error_close_pipe);
	}
	
	char p;
    int leidos;
	while (1) {
		leidos = read(pe_abc[0],&p,1);
        if ( leidos != -1 ) {
            printf("A\n");
            escribir(pabc_abc[1],"X",1);
        }
        leidos = read(pabc_abc[0],&p,1);
        if ( leidos != -1 ) {
            printf("A\n");
            escribir(pabc_d[1],"D",1);
        }  
	}
}

void B() {
	if ( close(pabc_d[0]) == -1 && close(pe_abc[1]) == -1 ) {
		printf("Error al cerrar el pipe.\n");
		exit(error_close_pipe);
	}

	char p;
    int leidos;
	while (1) {
		leidos = read(pe_abc[0],&p,1);
        if ( leidos != -1 ) {
            printf("B\n");
            escribir(pabc_abc[1],"X",1);
        }
        leidos = read(pabc_abc[0],&p,1);
        if ( leidos != -1 ) {
            printf("B\n");
            escribir(pabc_d[1],"D",1);
        }
	}
}

void C() {
	if ( close(pabc_d[0]) == -1 && close(pe_abc[1]) == -1 ) {
		printf("Error al cerrar el pipe.\n");
		exit(error_close_pipe);
	}
	
	char p;
    int leidos;
	while (1) {
		leidos = read(pe_abc[0],&p,1);
        if ( leidos != -1 ) {
            printf("C\n");
            escribir(pabc_abc[1],"X",1);
        }
        leidos = read(pabc_abc[0],&p,1);
        if ( leidos != -1 ) {
            printf("C\n");
            escribir(pabc_d[1],"D",1);
        }
	}
}

void D() {
	if ( close(pd_e[0]) == -1 && close(pabc_d[1]) == -1 ) {
		printf("Error al cerrar el pipe.\n");
		exit(error_close_pipe);
	}
	
	char p;
	while (1) {
		read(pabc_d[0],&p,1);
		printf("D\n");
		escribir(pd_e[1],"E",1);
	}
}

void E() {
	if ( close(pe_abc[0]) == -1 && close(pd_e[1]) == -1 ) {
		printf("Error al cerrar el pipe.\n");
		exit(error_close_pipe);
	}

	char p;
	while (1) {
		read(pd_e[0],&p,1);
		printf("E\n");
		escribir(pe_abc[1],"X",1);
	}
}

int main() {
	if ( !pipe(pabc_abc) && !pipe(pabc_d) && !pipe(pd_e) && !pipe(pe_abc) ) {
		int pid;
		
        if ( !fcntl(pe_abc[0], F_SETFL, O_NONBLOCK) && !fcntl(pabc_abc[0], F_SETFL, O_NONBLOCK) ) {

            escribir(pe_abc[1],"X",1);
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
            printf("Error al crear los pipes.\n");
            exit(error_pipe);
        }
	}
	else {
		printf("Error al crear los pipes.\n");
		exit(error_pipe);
	}
	return 0;
}
