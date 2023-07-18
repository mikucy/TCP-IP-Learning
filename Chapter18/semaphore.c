#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

static sem_t sem_one;
static sem_t sem_two;
static int num;

void* read(void* arg) {
    for (int i = 0; i < 5; ++i) {
        fputs("Input num: ", stdout);
        sem_wait(&sem_two);
        scanf("%d", &num);
        sem_post(&sem_one);
    }
}

void* write(void* arg) {
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        sem_wait(&sem_one);
        sum += num;
        sem_post(&sem_two);
    }
    printf("Sum is %d\n", sum);
}

int main() {
    pthread_t id1, id2;
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);
    pthread_create(&id1, NULL, read, NULL);
    pthread_create(&id2, NULL, write, NULL);
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
}