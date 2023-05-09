/*
条件变量
条件变量不是锁
*/
/**
 * 在prodcust.c中消费者需要不断循环查看有五数据
 * 有没有一种机制  每数据的时候消费者阻塞 
 * 当生产者写数据之后 能够同志消费者接触阻塞 那就是信号量 
 * 
 *  
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


pthread_mutex_t lock;

pthread_cond_t cond;
struct Node{
    int num;
    struct Node * next;
};
// 头指针
struct Node* head=NULL;

void * producer(void *arg){
    while(1){

        pthread_mutex_lock(&lock);
        struct  Node * newnode =(struct Node *)malloc(sizeof(struct Node));
        // 头插入
        newnode->next = head;
        head = newnode;
        // 模拟数据
        newnode->num = rand()%100;
        printf("producer %ld  num %d\n",pthread_self(),newnode->num);
        
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
void * consumer(void *arg){
    while(1){
        //  消费头部节点
        // 如果不加锁 .
        pthread_mutex_lock(&lock);
        if(head!=NULL){
            struct  Node * tmp =head;
            head = head ->next;
            printf("consumer %ld  num %d\n",pthread_self(),tmp->num);
            free(tmp);
            pthread_mutex_unlock(&lock);
        }else{
            // 阻塞 会解锁 解除阻塞的时候会再次加锁
            pthread_cond_wait(&cond,&lock);
            pthread_mutex_unlock(&lock);
        }
     
        

    }
    return NULL;
}
int main(){
   pthread_mutex_init(&lock,NULL);

  pthread_cond_init(&cond,NULL);
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

     pthread_mutex_destroy(&cond);
}
