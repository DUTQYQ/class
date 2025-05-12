#include <stdio.h>
#include <pthread.h>

int sum = 0; // 共享变量

void *thread(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        sum += 1; // 无同步操作
    }
    return NULL;
}

int main() {
    pthread_t tid1, tid2;
    
    pthread_create(&tid1, NULL, thread, NULL);
    pthread_create(&tid2, NULL, thread, NULL);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    printf("Result: 1000000 + 1000000 = %d (Expected: 2000000)\n", sum);
    return 0;
}
