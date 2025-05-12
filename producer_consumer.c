#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h> 
#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty;    // 空槽数量
sem_t full;     // 满槽数量
pthread_mutex_t mutex;  // 互斥锁

void* producer(void* arg) {
    int item;
    for (int i = 0; i < 10; i++) {
        item = rand() % 100; // 生产一个随机数
        
        sem_wait(&empty);    // 等待空槽
        pthread_mutex_lock(&mutex);
        
        buffer[in] = item;
        printf("生产者放入 %d，位置：%d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        
        pthread_mutex_unlock(&mutex);
        sem_post(&full);    // 增加满槽
    }
    return NULL;
}

void* consumer(void* arg) {
    int item;
    for (int i = 0; i < 10; i++) {
        sem_wait(&full);     // 等待满槽
        pthread_mutex_lock(&mutex);
        
        item = buffer[out];
        printf("消费者取出 %d，位置：%d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);    // 增加空槽
    }
    return NULL;
}

int main() {
    pthread_t prod_tid, cons_tid;
    
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cons_tid, NULL, consumer, NULL);
    
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);
    
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
