#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "constantes.h"

struct mensaje{
    long tipo;
    int dato;
};

void enviar_mensaje(int id_cola, struct mensaje * m) {
    int error = msgsnd(id_cola,m,sizeof(struct mensaje)-sizeof(long),0);
    if ( error == -1 ) { 
            printf("Error al enviar un mensaje a la cola.\n");
            exit(error_send);
    }
}

void recibir_mensaje(int id_cola, struct mensaje * m, long tipo) {
    int error = msgrcv(id_cola,m,sizeof(struct mensaje)-sizeof(long),tipo,0);
    if ( error == -1 ) { 
            printf("Error al recibir un mensaje de la cola.\n");
            exit(error_receive);
    }
}

void A(int id_cola) {
    struct mensaje * m = (struct mensaje *) malloc(sizeof(struct mensaje));
    if(m==NULL) {
        printf("Error en malloc.\n");
        exit(error_malloc);
    }
    int envio = 0;
    while (1) {
        recibir_mensaje(id_cola,m,EA); 
        printf("A\n");
        if ( !envio ) {
            m->tipo = AC;
            enviar_mensaje(id_cola,m);
            envio = 1;
        }
        else { 
            m->tipo = AB;
            enviar_mensaje(id_cola,m);
            envio = 0;
        }
    }
    free(m);
}

void B(int id_cola) {
    struct mensaje * m = (struct mensaje *) malloc(sizeof(struct mensaje));
    if(m==NULL) {
        printf("Error en malloc.\n");
        exit(error_malloc);
    }
    int envio = 0;
    while (1) {
        printf("B\n");
        if ( !envio ) {
            recibir_mensaje(id_cola,m,EB);
            envio = 1;
        }
        else {
            recibir_mensaje(id_cola,m,AB);
            envio = 0;
        }
        m->tipo = BC;
        enviar_mensaje(id_cola,m);
    }
    free(m);
}
void C(int id_cola) {
    struct mensaje * m = (struct mensaje *) malloc(sizeof(struct mensaje));
    if(m==NULL) {
        printf("Error en malloc.\n");
        exit(error_malloc);
    }
    int envio = 0;
    while (1) {
        printf("C\n");
        if ( !envio ) {
            recibir_mensaje(id_cola,m,AC);
            envio = 1;
        }
        else {
            recibir_mensaje(id_cola,m,BC);
            if ( envio == 1 ) envio++;
            else envio = 0;
        }
        m->tipo = CD;
        enviar_mensaje(id_cola,m);
    }
    free(m);
}

void D(int id_cola) {
    struct mensaje * m = (struct mensaje *) malloc(sizeof(struct mensaje));
    if(m==NULL) {
        printf("Error en malloc.\n");
        exit(error_malloc);
    }
    while (1) {
        recibir_mensaje(id_cola,m,CD);
        printf("D\n");
        m->tipo = DE;
        enviar_mensaje(id_cola,m);
    }
    free(m);
}

void E(int id_cola) {
    struct mensaje * m = (struct mensaje *) malloc(sizeof(struct mensaje));
    if(m==NULL) {
        printf("Error en malloc.\n");
        exit(error_malloc);
    }
    int envio = 0;
    while (1) {
        recibir_mensaje(id_cola,m,DE);
        printf("E\n");
        if ( !envio ) {
            m->tipo = EB;
            enviar_mensaje(id_cola,m);
            envio = 1;
        }
        else { 
            m->tipo = EA;
            enviar_mensaje(id_cola,m);
            if ( envio == 1 ) envio++;
            else envio = 0;
        }
    }
    free(m);
}

void inicializar_cola(int id_cola) {
    struct mensaje * m = (struct mensaje *) malloc(sizeof(struct mensaje));
    if(m==NULL) {
        printf("Error en malloc.\n");
        exit(error_malloc);
    }
    m->tipo = EA;
    m->dato = 0;
    enviar_mensaje(id_cola,m);
    free(m);
}

int main() {
    int id_cola = msgget((key_t) 1234,IPC_CREAT|0666);
    if ( id_cola == -1 ) {
        printf("Error al crear la cola de mensajes.\n");
        exit(error_cola);
    }
    inicializar_cola(id_cola);
    int pid;

    pid = fork();
    if ( !pid ) { //hijo
        id_cola = msgget((key_t) 1234, 0666);
        if ( id_cola == -1 ) {
            printf("Error al crear la cola de mensajes.\n");
            exit(error_cola);
        }
        A(id_cola);
        exit(0);
    }
    else if (pid < 0) {
        printf("Error en la creación de procesos.\n");
        exit(error_fork);
    }

    pid = fork();
    if ( !pid ) { //hijo
        id_cola = msgget((key_t) 1234, 0666);
        if ( id_cola == -1 ) {
            printf("Error al crear la cola de mensajes.\n");
            exit(error_cola);
        }
        B(id_cola);
        exit(0);
    }
    else if (pid < 0) {
        printf("Error en la creación de procesos.\n");
        exit(error_fork);
    }

    pid = fork();
    if ( !pid ) { //hijo
        id_cola = msgget((key_t) 1234, 0666);
        if ( id_cola == -1 ) {
            printf("Error al crear la cola de mensajes.\n");
            exit(error_cola);
        }
        C(id_cola);
        exit(0);
    }
    else if (pid < 0) {
        printf("Error en la creación de procesos.\n");
        exit(error_fork);
    }

    pid = fork();
    if ( !pid ) { //hijo
        id_cola = msgget((key_t) 1234, 0666);
        if ( id_cola == -1 ) {
            printf("Error al crear la cola de mensajes.\n");
            exit(error_cola);
        }
        D(id_cola);
        exit(0);
    }
    else if (pid < 0) {
        printf("Error en la creación de procesos.\n");
        exit(error_fork);
    }

    pid = fork();
    if ( !pid ) { //hijo
        id_cola = msgget((key_t) 1234, 0666);
        if ( id_cola == -1 ) {
            printf("Error al crear la cola de mensajes.\n");
            exit(error_cola);
        }
        E(id_cola);
        exit(0);
    }
    else if (pid < 0) {
        printf("Error en la creación de procesos.\n");
        exit(error_fork);
    }

    for ( int i = 0; i < 5; i++) wait(NULL);
	return 0;
}
