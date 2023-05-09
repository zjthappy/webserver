/**
 * 读写锁
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
int num = 1;


pthread_rwlock_t rwlock;
void * wnum(void *arg){
    while(1){
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("threadid:%ld  write num:%ld\n",pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
      
    }
    return NULL;
}

void * rnum(void *arg){
    while(1){
       usleep(10);
             pthread_rwlock_rdlock(&rwlock);
        printf("threadid:%ld  read num:%ld\n",pthread_self(), num);
                pthread_rwlock_unlock(&rwlock);
                  usleep(10);
    }
    return NULL;
}
int main(){

    pthread_rwlock_init(&rwlock,NULL);
    // 创建线程
    pthread_t rtids[5],wtids[3];
    for(int i=0;i<5;i++){
        pthread_create(&rtids[i],NULL,rnum,NULL);
       
    }
    for(int i=0;i<5;i++){
       
        pthread_detach(rtids[i]);
    }
    for(int i=0;i<3;i++){
         pthread_create(&wtids[i],NULL,wnum,NULL);
       
    }
    for(int i=0;i<3;i++){
      
        pthread_detach(wtids[i]);
    }

    pthread_exit(NULL);
    // 这个起始不会执行
    pthread_rwlock_destroy(&rwlock);
    return 0;

}