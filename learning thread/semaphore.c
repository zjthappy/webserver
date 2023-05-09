/**
#include <semaphore.h>
int sem_init(sem_t *sem, int pshared, unsigned int value);
        -参数
        pshared 0 pthread
        value 信号量的zhi

感觉信号两 时一种更高集成度的变量 因为包含了 数据的数量 
比如此时有多少可以消费的数据 有多少可以生产的空间

*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t lock;



sem_t psem;
sem_t csem;

struct Node{
    int num;
    struct Node * next;
};
// 头指针
struct Node* head=NULL;

void * producer(void *arg){
    while(1){

        sem_wait(&psem);
        pthread_mutex_lock(&lock);
        struct  Node * newnode =(struct Node *)malloc(sizeof(struct Node));
        // 头插入
        newnode->next = head;
        head = newnode;
        // 模拟数据
        newnode->num = rand()%100;
        printf("producer %ld  num %d\n",pthread_self(),newnode->num);
        
          
        pthread_mutex_unlock(&lock);
        sem_post(&csem);
    }
    return NULL;
}
void * consumer(void *arg){
    while(1){
        //  消费头部节点
        // 如果不加锁 .

         sem_wait(&csem);
        pthread_mutex_lock(&lock);
  
        struct  Node * tmp =head;
        head = head ->next;
        printf("consumer %ld  num %d\n",pthread_self(),tmp->num);
        free(tmp);
        pthread_mutex_unlock(&lock);
       
                sem_post(&psem);
     
        

    }
    return NULL;
}
int main(){
 pthread_mutex_init(&lock,NULL);

sem_init(&psem,0,8);
sem_init(&csem,0,0);
    // 创建线程
    pthread_t ptids[5],ctids[5];
    for(int i=0;i<5;i++){
        pthread_create(&ptids[i],NULL,producer,NULL);
       
    }
    for(int i=0;i<5;i++){
       
        pthread_detach(ptids[i]);
    }
    for(int i=0;i<5;i++){
         pthread_create(&ctids[i],NULL,consumer,NULL);
       
    }
    for(int i=0;i<5;i++){
      
        pthread_detach(ctids[i]);
    }
    pthread_exit(NULL);
    pthread_mutex_destroy(&lock);

}
