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
    int porciones_restantes;
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

void oso(int id_cola) {
    struct mensaje * m = (struct mensaje *) malloc(sizeof(struct mensaje));
    if(m==NULL) {
        printf("Error en malloc.\n");
        exit(error_malloc);
    }
    while (1) {
        recibir_mensaje(id_cola,m,2);
        printf("El oso se despierta y empieza a comer\n");
        m->porciones_restantes = capacidad_tarro;
        printf("El tarro está lleno\n");
        m->tipo = 1;
        enviar_mensaje(id_cola,m);
    }
    free(m);
}

void producir_miel(int i) {
    sleep(1);
    printf("La abeja %i produjo una porción de miel.\n",i);
}

void abeja(int i, int id_cola) {
    struct mensaje * m = (struct mensaje *) malloc(sizeof(struct mensaje));
    if(m==NULL) {
        printf("Error en malloc.\n");
        exit(error_malloc);
    }
    while (1) {
        producir_miel(i);
        recibir_mensaje(id_cola,m,1);
        if ( !m->porciones_restantes ) {
            printf("La abeja %i no puede insertar su porción y despierta al oso.\n",i);
            m->tipo = 2;
            enviar_mensaje(id_cola,m);
        }
        else {
            m->porciones_restantes--;
            printf("La abeja %i insertó una porción. Quedan %i.\n",i,m->porciones_restantes);
            m->tipo = 1;
            enviar_mensaje(id_cola,m);
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
    m->tipo = 1;
    m->porciones_restantes= capacidad_tarro;
    printf("las porciones iniciales son %i\n",m->porciones_restantes);
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

    int i, pid;
    pid = fork();
    if ( !pid ) {
        id_cola = msgget((key_t) 1234, 0666);
        if ( id_cola == -1 ) {
            printf("Error al crear la cola de mensajes.\n");
            exit(error_cola);
        }
        oso(id_cola);
        exit(0);
    }
    else if ( pid == -1 ) {
        printf("Error en fork.\n");
        exit(error_fork);
    }

    for ( i = 0; i < cantidad_abejas; i++ ) {
        pid = fork();
        if ( !pid ) {
            id_cola = msgget((key_t) 1234, 0666);
            if ( id_cola == -1 ) {
                printf("Error al crear la cola de mensajes.\n");
                exit(error_cola);
            }
            abeja(i,id_cola);
            exit(0);
        }
        else if ( pid == -1 ) {
            printf("Error en fork.\n");
            exit(error_fork);
        }
    }

    for ( i = 0; i < cantidad_abejas+1; i++ ) wait(NULL);
    return 0;
}