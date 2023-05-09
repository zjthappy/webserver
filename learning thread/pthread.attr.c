#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
void * callback(void *arg){
    printf("child thread id : %ld",pthread_self());
    sleep(3);
    return NULL;

}
int main(){
    pthread_t tid ; 
    // tid , , 线程逻辑, 参数
    int ret = pthread_create(&tid,NULL,callback,NULL);

    if(ret!=0){
        char * errstr = strerror(ret);
        printf("error  :%s\n",errstr);
    }
    
    for(int i=0;i<5;i++){
         printf("%d\n",i);
    }

    // 设置 线程 detach 不用join回收
    pthread_detach(tid);
    return 0;

}