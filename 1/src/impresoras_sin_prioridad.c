#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#define n 15

sem_t impresora;
pthread_t threads[n];

void * usuario(int i) {
	while (1) {
		printf("Llega el usuario %i.\n",i);
		sleep(2);
		sem_wait(&impresora); // requerir impresora
		printf("El usuario %i está utilizando una impresora.\n",i);
		sleep(2);
		printf("El usuario %i libera una impresora.\n",i);
		sem_post(&impresora); // liberar impresora
		sleep(2);
	}
	return 0;
}

int main() {
	sem_init(&impresora,0,2);
	int i;
	for ( i = 0; i < n; i++ ) pthread_create(&threads[i],NULL,usuario,i);
	for ( i = 0; i < n; i++ ) pthread_join(threads[i],NULL);
	return 0;
}
