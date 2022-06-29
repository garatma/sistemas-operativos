#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include "constantes.h"

typedef struct memoria_compartida {
    sem_t mutex_tarro, despertar, vacio;
    int porciones_ocupadas;
} * memoria_compartida;

void producir_miel(int i) {
    sleep(1);
    printf("La abeja %i produjo una porción de miel.\n",i);
}

void oso(memoria_compartida mem) {
    while (1) {
        sem_wait(&mem->despertar);
        printf("El oso se despierta.\n");
        for ( int i = 0; i < capacidad_tarro; i++ ) {
            mem->porciones_ocupadas--;
            printf("El oso come una porción. Quedan %i porciones por comer.\n", mem->porciones_ocupadas);
            sem_post(&mem->vacio);
        }
        sem_post(&mem->mutex_tarro);
    }
}

void abeja(int i, memoria_compartida mem) {
    while (1) {
        producir_miel(i);
        sem_wait(&mem->mutex_tarro);
        if ( !sem_trywait(&mem->vacio) ) {
			mem->porciones_ocupadas++;
			printf("La abeja %i insertó una porción en el tarro. Quedan %i porciones libres.\n",i,capacidad_tarro-mem->porciones_ocupadas);
            sem_post(&mem->mutex_tarro);
        }
        else sem_post(&mem->despertar);
    }
}

void inicializar_memoria() {
    int id_shm = shmget((key_t) 1234,sizeof(struct memoria_compartida),IPC_CREAT | 0666);
    if ( id_shm == -1 ) {
        printf("Error al crear la memoria compartida.\n");
        exit(error_creacion_memoria);
    }

    void * p = shmat(id_shm,NULL,0);
    if ( p == (void*) -1) {
        printf("Error al insertar un dato en la memoria compartida.\n");
        exit(error_attach);
    }
    memoria_compartida mem = (memoria_compartida) p;
    sem_init(&mem->despertar,1,0);
    sem_init(&mem->mutex_tarro,1,1);
    sem_init(&mem->vacio,1,capacidad_tarro);
    mem->porciones_ocupadas = 0;
    if ( shmdt(p) ) {
        printf("Error en la desvinculación de la memoria.\n");
        exit(error_detach);
    }
}

void inicializar_oso() {
    int pid = fork();
    if ( !pid ) {
        int id_shm = shmget((key_t) 1234,sizeof(struct memoria_compartida),0666);
        if ( id_shm == -1 ) {
            printf("Error al vincularse a la memoria compartida.\n");
            exit(error_vinculacion_memoria);
        }
        void * p = shmat(id_shm,NULL,0);
        if ( p == (void*) -1) {
            printf("Error al insertar un dato en la memoria compartida.\n");
            exit(error_attach);
        }
        memoria_compartida mem = (memoria_compartida) p;
        oso(mem);
        exit(0);
    }
    else if ( pid == -1 ) {
        printf("Error en fork.\n");
        exit(error_fork);
    }
}

void inicializar_abejas() {
    for ( int  i = 0; i < cantidad_abejas; i++ ) {
        int pid = fork();
        int id_shm = shmget((key_t) 1234,sizeof(struct memoria_compartida),0666);
        if ( id_shm == -1 ) {
            printf("Error al vincularse a la memoria compartida.\n");
            exit(error_vinculacion_memoria);
        }
        void * p = shmat(id_shm,NULL,0);
        if ( p == (void*) -1) {
            printf("Error al insertar un dato en la memoria compartida.\n");
            exit(error_attach);
        }
        memoria_compartida mem = (memoria_compartida) p;
        if ( !pid ) {
            abeja(i,mem);
            exit(0);
        }
        else if ( pid == -1 ) {
            printf("Error en fork.\n");
            exit(error_fork);
        }
    }
}

int main() {
    inicializar_memoria();
    inicializar_oso();
    inicializar_abejas();
    for ( int i = 0; i < cantidad_abejas+1; i++ ) wait(NULL);
    return 0;
}
