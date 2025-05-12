#include <stdio.h>
#include <pthread.h>

int sum = 0;
pthread_mutex_t mutex; // 互斥锁

void *thread(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&mutex); // 加锁
        sum += 1;
        pthread_mutex_unlock(&mutex); // 解锁
    }
    return NULL;
}

int main() {
    pthread_t tid1, tid2;
    pthread_mutex_init(&mutex, NULL); // 初始化互斥锁
    
    pthread_create(&tid1, NULL, thread, NULL);
    pthread_create(&tid2, NULL, thread, NULL);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    printf("Result: 1000000 + 1000000 = %d (Expected: 2000000)\n", sum);
    pthread_mutex_destroy(&mutex); // 销毁锁
    return 0;
}
