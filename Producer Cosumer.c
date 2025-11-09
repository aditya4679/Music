// producer_consumer_input.c  
#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <semaphore.h>  
#include <unistd.h>

#define BUFFER_SIZE 5
#define PRODUCE_COUNT 5   // number of items to produce/consume

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t emptyCount;        // counts empty slots
sem_t fillCount;         // counts filled slots
pthread_mutex_t mutexBuffer;

void *producer(void *arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        int item;
        printf("Producer: Enter item %d: ", i+1);
        scanf("%d", &item);

        sem_wait(&emptyCount);            // wait for empty slot
        pthread_mutex_lock(&mutexBuffer); // lock buffer

        buffer[in] = item;
        printf("Producer produced : %d at index %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutexBuffer); // unlock buffer
        sem_post(&fillCount);               // signal filled slot

        // optional sleep
        sleep(1);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        sem_wait(&fillCount);             // wait for filled slot
        pthread_mutex_lock(&mutexBuffer); // lock buffer

        int item = buffer[out];
        printf("Consumer consumed : %d from index %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutexBuffer); // unlock buffer
        sem_post(&emptyCount);              // signal empty slot

        // optional sleep
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(void) {
    pthread_t prodThread, consThread;

    sem_init(&emptyCount, 0, BUFFER_SIZE);
    sem_init(&fillCount, 0, 0);
    pthread_mutex_init(&mutexBuffer, NULL);

    pthread_create(&prodThread, NULL, producer, NULL);
    pthread_create(&consThread, NULL, consumer, NULL);

    pthread_join(prodThread, NULL);
    pthread_join(consThread, NULL);

    pthread_mutex_destroy(&mutexBuffer);
    sem_destroy(&emptyCount);
    sem_destroy(&fillCount);

    printf("Main: All threads finished. Exiting.\n");
    return EXIT_SUCCESS;
}
