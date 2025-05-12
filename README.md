# 多线程编程实验报告

## 任务1：示例代码验证
- **nosync-ex.c 结果**：输出值小于2000000（数据竞争导致）
- **mutex-ex.c 结果**：正确输出2000000（互斥锁保证原子性）
- **sem-ex.c 结果**：正确输出2000000（信号量控制并发）

## 任务2：生产者消费者问题
```
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h> #include <stdio.h>
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
```
- **同步机制**：信号量`empty`和`full`控制缓冲区状态，互斥锁保护共享变量
- **测试结果**：生产者和消费者交替操作缓冲区，无数据竞争
## 任务3代码理解
#### 3.1 pthread-ex01

    功能：主线程创建子线程，子线程通过 pthread_exit 返回 42，主线程通过 pthread_join 获取返回值并打印。
    输出：42

#### 3.2 pthread-ex02

    功能：子线程调用 exit(42) 终止整个进程。
    输出：无输出，进程直接以状态码 42 退出。

#### 3.3 pthread-ex03

    功能：两个线程分别操作共享变量 i：

        thread 返回 i 的初始值 42。

        thread2 将 i 设为 0，并返回 31。

    执行顺序：

        thread 可能先读取 i=42，主线程通过 pthread_join 将 i 设为 42。

        thread2 随后将 i 修改为 0。

    输出：0

#### 3.4 pthread-ex-O4

    功能：子线程调用 pthread_detach 后返回 42，主线程尝试 pthread_join

    输出：不确定（可能是 0 或随机值）。

#### 3.5 pthread-ex-05

    功能：两个线程操作全局变量 i：

        thread 打印 i。

        thread2 将 i 设为 31。

    问题：

        线程执行顺序不确定，输出可能是 42 或 31。

        thread 未显式返回值，pthread_join 可能导致未定义行为。

    输出：42

#### 3.6 pthread-ex-06

    功能：主线程为每个子线程分配独立的内存存储 i 的值。

    关键点：每次循环使用 malloc 分配新内存，避免数据竞争。

    输出：Thread 0 Thread 1

#### 3.7 pthread-ex-07

    功能：主线程将循环变量 i 的地址传递给子线程。

    问题：所有子线程共享 i 的地址，可能导致读取到更新后的值。

    输出：Thread 1。
## 任务4：理发师问题
```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CHAIRS 3      // 等待椅数量
#define NUM_CUSTOMERS 5   // 顾客数量

sem_t customers;         // 等待的顾客信号量
sem_t barber;           // 理发师状态信号量
sem_t mutex;            // 互斥锁
int waiting = 0;        // 正在等待的顾客数
int served_customers = 0; // 已服务的顾客数

void* barber_thread(void* arg) {
    while (served_customers < NUM_CUSTOMERS) {
        printf("理发师正在睡觉...\n");
        sem_wait(&customers); // 等待顾客到来
        
        sem_wait(&mutex);
        waiting--; // 顾客从等待椅起身
        sem_post(&mutex);
        
        printf("理发师开始理发\n");
        sleep(1 + rand() % 3); // 随机理发时间1-3秒
        
        sem_post(&barber); // 理发完成
        served_customers++;
    }
    printf("理发师: 今天服务已完成\n");
    return NULL;
}

void* customer_thread(void* arg) {
    int id = *(int*)arg;
    free(arg); // 释放动态分配的内存
    
    sem_wait(&mutex);
    if (waiting < NUM_CHAIRS) {
        waiting++;
        printf("顾客%d坐下等待 (等待人数: %d)\n", id, waiting);
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

int main() {3.1 pthread-ex01

    功能：主线程创建子线程，子线程通过 pthread_exit 返回 42，主线程通过 pthread_join 获取返回值并打印。

    问题：pthread_join 的语法有误（(void **\&i) 应为 (void **)&i），可能是笔误。

    输出：42（若修正语法错误）。

3.2 pthread-ex02

    功能：子线程调用 exit(42) 终止整个进程。

    问题：exit 会直接终止进程，主线程的 pthread_join 和 printf 不会执行。

    输出：无输出，进程直接以状态码 42 退出。

3.3 pthread-ex03

    功能：两个线程分别操作共享变量 i：

        thread 返回 i 的初始值 42。

        thread2 将 i 设为 0，并返回 31。

    执行顺序：

        thread 可能先读取 i=42，主线程通过 pthread_join 将 i 设为 42。

        thread2 随后将 i 修改为 0。

    输出：0（最终 i 被 thread2 修改）。

3.4 pthread-ex-O4

    功能：子线程调用 pthread_detach 后返回 42，主线程尝试 pthread_join。

    问题：已分离的线程无法通过 pthread_join 获取返回值，导致未定义行为。

    输出：不确定（可能是 0 或随机值）。

3.5 pthread-ex-05

    功能：两个线程操作全局变量 i：

        thread 打印 i。

        thread2 将 i 设为 31。

    问题：

        线程执行顺序不确定，输出可能是 42 或 31。

        thread 未显式返回值，pthread_join 可能导致未定义行为。

    输出：42 或 31（取决于线程执行顺序）。

3.6 pthread-ex-06

    功能：主线程为每个子线程分配独立的内存存储 i 的值。

    关键点：每次循环使用 malloc 分配新内存，避免数据竞争。

    输出：Thread 0 和 Thread 1（顺序可能颠倒）。

3.7 pthread-ex-07

    功能：主线程将循环变量 i 的地址传递给子线程。

    问题：所有子线程共享 i 的地址，可能导致读取到更新后的值。

    输出：通常为 Thread 1 和 Thread 1（或 Thread 2，取决于线程执行时机）。
    pthread_t barber_tid;
    pthread_t customer_tids[NUM_CUSTOMERS];
    
    // 初始化随机数种子
    srand(time(NULL));
    
    // 初始化信号量
    sem_init(&customers, 0, 0);
    sem_init(&barber, 0, 0);
    sem_init(&mutex, 0, 1);
    
    // 创建理发师线程
    pthread_create(&barber_tid, NULL, barber_thread, NULL);
    
    // 创建顾客线程
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&customer_tids[i], NULL, customer_thread, id);
        sleep(1 + rand() % 2); // 随机间隔1-2秒
    }
    
    // 等待所有线程完成
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customer_tids[i], NULL);
    }
    pthread_join(barber_tid, NULL);
    
    // 销毁信号量
    sem_destroy(&customers);
    sem_destroy(&barber);
    sem_destroy(&mutex);
    
    return 0;
}
```
代码说明

    同步机制：
3.1 pthread-ex01

    功能：主线程创建子线程，子线程通过 pthread_exit 返回 42，主线程通过 pthread_join 获取返回值并打印。

    问题：pthread_join 的语法有误（(void **\&i) 应为 (void **)&i），可能是笔误。

    输出：42（若修正语法错误）。

3.2 pthread-ex02

    功能：子线程调用 exit(42) 终止整个进程。

    问题：exit 会直接终止进程，主线程的 pthread_join 和 printf 不会执行。

    输出：无输出，进程直接以状态码 42 退出。

3.3 pthread-ex03

    功能：两个线程分别操作共享变量 i：

        thread 返回 i 的初始值 42。

        thread2 将 i 设为 0，并返回 31。

    执行顺序：

        thread 可能先读取 i=42，主线程通过 pthread_join 将 i 设为 42。

        thread2 随后将 i 修改为 0。

    输出：0（最终 i 被 thread2 修改）。

3.4 pthread-ex-O4

    功能：子线程调用 pthread_detach 后返回 42，主线程尝试 pthread_join。

    问题：已分离的线程无法通过 pthread_join 获取返回值，导致未定义行为。

    输出：不确定（可能是 0 或随机值）。

3.5 pthread-ex-05

    功能：两个线程操作全局变量 i：

        thread 打印 i。

        thread2 将 i 设为 31。

    问题：

        线程执行顺序不确定，输出可能是 42 或 31。

        thread 未显式返回值，pthread_join 可能导致未定义行为。

    输出：42 或 31（取决于线程执行顺序）。

3.6 pthread-ex-06

    功能：主线程为每个子线程分配独立的内存存储 i 的值。

    关键点：每次循环使用 malloc 分配新内存，避免数据竞争。

    输出：Thread 0 和 Thread 1（顺序可能颠倒）。

3.7 pthread-ex-07

    功能：主线程将循环变量 i 的地址传递给子线程。

    问题：所有子线程共享 i 的地址，可能导致读取到更新后的值。

    输出：通常为 Thread 1 和 Thread 1（或 Thread 2，取决于线程执行时机）。
        customers信号量：表示等待理发的顾客数量

        barber信号量：表示理发师是否空闲

        mutex：保护共享变量waiting的互斥锁

    理发师行为：

        没有顾客时睡觉（等待customers信号量）

        被唤醒后开始理发

        理发完成后释放barber信号量

    顾客行为：

        检查是否有空椅子（通过waiting变量）

        有空位则等待理发，否则离开

        等待理发时通知理发师（通过customers信号量）

    随机性：

        顾客到达间隔随机（1-2秒）

        理发时间随机（1-3秒）

输出:

理发师正在睡觉...
顾客1坐下等待 (等待人数: 1)
理发师开始理发
顾客2坐下等待 (等待人数: 2)
顾客1理发完成
理发师开始理发
顾客3坐下等待 (等待人数: 3)
顾客2理发完成
理发师开始理发
顾客4: 没有空椅子，离开
顾客3理发完成
理发师正在睡觉...
顾客5坐下等待 (等待人数: 1)
理发师开始理发
顾客5理发完成
理发师: 今天服务已完成

