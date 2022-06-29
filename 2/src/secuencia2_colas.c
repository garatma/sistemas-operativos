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

int recibir_mensaje(int id_cola, struct mensaje * m, long tipo, int flag) {
    int leidos = msgrcv(id_cola,m,sizeof(struct mensaje)-sizeof(long),tipo,flag);
    if ( flag != IPC_NOWAIT && leidos == -1 ) { 
            printf("Error al recibir un mensaje de la cola.\n");
            exit(error_receive);
    }
    return leidos;
}

void A(int id_cola) {
    struct mensaje * m = (struct mensaje *) malloc(sizeof(struct mensaje));
    if(m==NULL) {
        printf("Error en malloc.\n");
        exit(error_malloc);
    }
    int leidos = 0;
    while (1) {
        leidos = recibir_mensaje(id_cola,m,EABC,IPC_NOWAIT); 
        if ( leidos != -1 ) {
            printf("A\n");
            m->tipo = ABCABC;
            enviar_mensaje(id_cola,m);
        }
        leidos = recibir_mensaje(id_cola,m,ABCABC,IPC_NOWAIT); 
        if ( leidos != -1 ) {
            printf("A\n");
            m->tipo = ABCD;
            enviar_mensaje(id_cola,m);
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
    int leidos = 0;
    while (1) {
        leidos = recibir_mensaje(id_cola,m,EABC,IPC_NOWAIT); 
        if ( leidos != -1 ) {
            printf("B\n");
            m->tipo = ABCABC;
            enviar_mensaje(id_cola,m);
        }
        leidos = recibir_mensaje(id_cola,m,ABCABC,IPC_NOWAIT); 
        if ( leidos != -1 ) {
            printf("B\n");
            m->tipo = ABCD;
            enviar_mensaje(id_cola,m);
        }   
    }
    free(m);
}
void C(int id_cola) {
    struct mensaje * m = (struct mensaje *) malloc(sizeof(struct mensaje));
    if(m==NULL) {
        printf("Error en malloc.\n");
        exit(error_malloc);
    }
    int leidos = 0;
    while (1) {
        leidos = recibir_mensaje(id_cola,m,EABC,IPC_NOWAIT); 
        if ( leidos != -1 ) {
            printf("C\n");
            m->tipo = ABCABC;
            enviar_mensaje(id_cola,m);
        }
        leidos = recibir_mensaje(id_cola,m,ABCABC,IPC_NOWAIT); 
        if ( leidos != -1 ) {
            printf("C\n");
            m->tipo = ABCD;
            enviar_mensaje(id_cola,m);
        }   
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
        recibir_mensaje(id_cola,m,ABCD,0);
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
    while (1) {
        recibir_mensaje(id_cola,m,DE,0);
        printf("E\n");
        m->tipo = EABC;
        enviar_mensaje(id_cola,m);
    }
    free(m);
}

void inicializar_cola(int id_cola) {
    struct mensaje * m = (struct mensaje *) malloc(sizeof(struct mensaje));
    if(m==NULL) {
        printf("Error en malloc.\n");
        exit(error_malloc);
    }
    m->tipo = EABC;
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
