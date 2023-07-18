#include <stdio.h>
#include <pthread.h>
#define NUM_THREAD 100
long long num = 0;
pthread_mutex_t mutex;

void* thread_inc(void* arg) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 50000000; ++i) {
        num += 1;
    }
    pthread_mutex_unlock(&mutex);
}

void* thread_des(void* arg) {
    for (int i = 0; i < 50000000; ++i) {
        pthread_mutex_lock(&mutex);
        num -= 1;
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t thread_id[NUM_THREAD];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < NUM_THREAD; ++i) {
        if (i % 2) {
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        } else {
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
        }
    }
    for (int i = 0; i < NUM_THREAD; ++i) {
        pthread_join(thread_id[i], NULL);
    }
    printf("result: %lld\n", num);
    pthread_mutex_destroy(&mutex);
}