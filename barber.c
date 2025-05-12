#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 3         // 等待椅子的数量
#define CUSTOMERS 5 // 顾客数量

sem_t customers;    // 等待的顾客数
sem_t barber;       // 理发师状态
sem_t mutex;        // 保护空闲椅子数
int free_chairs = N; // 可用椅子数
int served_customers = 0; // 已服务顾客数

void* barber_thread(void* arg) {
    while (served_customers < CUSTOMERS) {
        printf("理发师正在睡觉...\n");
        sem_wait(&customers); // 等待顾客
        
        sem_wait(&mutex);
        free_chairs++; // 顾客从等待椅起身
        printf("理发师被唤醒，开始理发 (剩余等待椅子: %d)\n", free_chairs);
        sem_post(&mutex);
        
        sleep(1);     // 理发时间
        sem_post(&barber); // 理发完成
        served_customers++;
    }
    printf("理发师: 今日服务已完成\n");
    return NULL;
}

void* customer_thread(void* arg) {
    int id = *(int*)arg;
    sem_wait(&mutex);
    if (free_chairs > 0) {
        free_chairs--;
        printf("顾客%d坐下等待 (剩余椅子: %d)\n", id, free_chairs);
        sem_post(&customers); // 通知理发师
        sem_post(&mutex);
        
        sem_wait(&barber);    // 等待理发
        printf("顾客%d理发完成\n", id);
    } else {
        sem_post(&mutex);
        printf("顾客%d: 没有空椅子，离开\n", id);
    }
    return NULL;
}

int main() {
    pthread_t barber_tid;
    int customer_ids[CUSTOMERS];
    
    sem_init(&customers, 0, 0);
    sem_init(&barber, 0, 0);
    sem_init(&mutex, 0, 1);
    
    pthread_create(&barber_tid, NULL, barber_thread, NULL);
    
    // 模拟顾客陆续到达
    for (int i = 0; i < CUSTOMERS; i++) {
        customer_ids[i] = i + 1;
        pthread_t cust_tid;
        pthread_create(&cust_tid, NULL, customer_thread, &customer_ids[i]);
        sleep(1);  // 顾客间隔到达
    }
    
    // 等待所有顾客线程完成
    for (int i = 0; i < CUSTOMERS; i++) {
        pthread_join(customer_ids[i], NULL);
    }
    
    pthread_join(barber_tid, NULL);
    
    sem_destroy(&customers);
    sem_destroy(&barber);
    sem_destroy(&mutex);
    
    return 0;
}
