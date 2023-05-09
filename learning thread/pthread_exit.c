#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void * callback(void *arg){
    printf("child thread....");

    // 相当于调用pthread_exit()
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
    // 不会影响其他线程
    pthread_exit(NULL);
    for(int i=0;i<5;i++){
         printf("%d\n",i);
    }
}