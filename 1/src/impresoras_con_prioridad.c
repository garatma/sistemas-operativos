#include "lista.c"
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#define n 3

TLista lista; // la lista donde se almacena a cada usuario que quiere utilizar las impresoras
pthread_t thread[n]; // para guardar el id de cada hilo
sem_t mutex_lista, impresoras; // un mutex para el acceso a la lista. el otro semáforo es para el acceso a las impresoras
int prioridades[n]; // las prioridades de cada usuario: el usuario i tiene la prioridad prioridades[i]

// retorna el primer elemento de la lista. se utiliza para determinar el usuario con mayor prioridad
int pri() {
    int menor = lista->primer_celda->elemento;
    TCelda movil = lista->primer_celda;
    while ( movil != POS_NULA ) {
        if ( menor > movil->elemento ) menor = movil->elemento;
        movil = movil->proxima_celda;
    }
    return menor;
}

// retorna el segundo menor elemento de la lista. se utiliza para determinar el segundo usuario con
// mayor prioridad (ya que hay dos impresoras)
int segu(int primero) {
    int segundo = lista->primer_celda->elemento;
    TCelda movil = lista->primer_celda;
    while ( movil != POS_NULA ) {
        if ( segundo == primero ) segundo = movil->elemento;
        if ( segundo > movil->elemento && movil->elemento != primero ) segundo = movil->elemento;
        movil = movil->proxima_celda;
    }
    return segundo;
}

// para eliminar aquel usuario cuya prioridad es la pasada por parámetro una vez que utilizó una impresora.
void eliminar(int prioridad) {
    TCelda movil = lista->primer_celda;
    int encontre = 0;
    while ( !encontre && movil != POS_NULA ) {
        encontre = movil->elemento == prioridad;
        if ( !encontre ) movil = movil->proxima_celda;
    }
    if ( encontre ) l_eliminar(lista,movil);
}

// para mostrar la lista por pantalla y lograr una mejor visualización en la ejecución.
void mostrar_lista() {
    printf("[");
    TCelda movil = lista->primer_celda;
    while ( movil != POS_NULA ) {
        if ( movil->proxima_celda == POS_NULA ) printf("%i",movil->elemento);
        else printf("%i-",movil->elemento);
        movil = movil->proxima_celda;
    }
    printf("]\n");
}

void * usuario(int prioridad) {
    int suficiente_prioridad;
    int primero, segundo;
    while (1) {
        suficiente_prioridad = 0;
        sleep(0.5);
        sem_wait(&mutex_lista);
        sleep(0.5);
        printf("Llega el usuario con prioridad %i. La lista de usuarios es: ",prioridad);
        sleep(0.5);
        // el usuario quiere utilizar una impresora: se lo agrega a la lista.
        l_insertar(lista,POS_NULA,prioridad);
        sleep(0.5);
        mostrar_lista();
        sleep(0.5);
        sem_post(&mutex_lista);
        sleep(0.5);
        while ( !suficiente_prioridad ) {
            sem_wait(&mutex_lista);
            printf("Análisis de prioridad del usuario con prioridad %i:\n",prioridad);
            sleep(2);
            primero = pri();
            sleep(2);
            // puede utilizar la impresora si es el primero de la lista de prioridades.
            suficiente_prioridad = prioridad == primero;
            sleep(2);
            if ( suficiente_prioridad ) printf("    El usuario con prioridad %i está primero. Puede utilizar una impresora.\n",prioridad);
            else {
                // puede utilizar la impresora si es el segundo de la lista de prioridades (porque hay dos impresoras).
                segundo = segu(primero);
                suficiente_prioridad = prioridad == segundo;
                if ( suficiente_prioridad ) printf("    El usuario con prioridad %i está segundo. Puede utilizar una impresora.\n",prioridad);
                else printf("    El usuario con prioridad %i no tiene prioridad para usar las impresoras.\n",prioridad);
            }
            sleep(2);
            sem_post(&mutex_lista);
            sleep(2);
        }
        // si hay una impresora disponible, el usuario prosigue a utilizarla.
        sem_wait(&impresoras);
        sleep(2);
        printf("El usuario con prioridad %i está usando una impresora.\n",prioridad);
        sleep(2);
        sem_wait(&mutex_lista);
        sleep(2);
        // el usuario utilizó la impresora. por lo tanto, se lo elimina de la lista de usuarios que quieren utilizarla.
        eliminar(prioridad);
        sleep(2);
        printf("El usuario con prioridad %i libera una impresora. La lista de usuarios es: ",prioridad);
        sleep(2);
        mostrar_lista();
        sleep(2);
        sem_post(&mutex_lista);
        sleep(2);
        sem_post(&impresoras);
        sleep(2);
    }
    return NULL;
}

// inicializa las prioridades de cada usuario
void setear_prioridades() {
    for (int i = 0; i < n; i++) prioridades[i] = i; // en este caso, la prioridad se define con el número de usuario
}

int main() {
    lista = crear_lista();
    sem_init(&mutex_lista,0,1);
    sem_init(&impresoras,0,2);
    setear_prioridades();
    int i = 0;
    for (i = 0; i < n; i++) pthread_create(&thread[i],NULL,usuario,prioridades[i]); // cada usuario conoce su prioridad
    for ( i = 0; i < n; i++ ) pthread_join(thread[i],NULL);
    return 0;
}
