#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int sum = 0;
sem_t sem; // 信号量

void *thread(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        sem_wait(&sem); // P操作（信号量-1）
        sum += 1;
        sem_post(&sem); // V操作（信号量+1）
    }
    return NULL;
}

int main() {
    pthread_t tid1, tid2;
    sem_init(&sem, 0, 1); // 初始化信号量（初始值为1）
    
    pthread_create(&tid1, NULL, thread, NULL);
    pthread_create(&tid2, NULL, thread, NULL);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    printf("Result: 1000000 + 1000000 = %d (Expected: 2000000)\n", sum);
    sem_destroy(&sem); // 销毁信号量
    return 0;
}
