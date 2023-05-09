/**
 * 
 * 生产消费者模型是多线程常见的场景
 * 模拟生产消费者模型
 * 看会出现什么问题
 * 
 * 不加锁
 *     数据读写肯定会逻辑错误
 *     段错误：因为消费者同时读  
 * 加了互斥锁 
 *     还需要修改代码 当消费者发现没数据时就不读了 
 * 
 * 问题：消费者需要一直循环查询是否有数据
 * 
 * 注意：因例子中使用的链表 没有容器大小限制 所以生产者不需要查看
 * 是否满了 
 * 
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


pthread_mutex_t lock;
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
            pthread_mutex_unlock(&lock);
            continue;
        }
     
        

    }
    return NULL;
}
int main(){
   pthread_mutex_init(&lock,NULL);
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
