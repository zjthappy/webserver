/**
 * 可以把一个程序看成只有一个线程 main所在的就是主线程
 * 其他成为子线程
 * 
 * 程序中默认只有一个线程，调用pthreadcreate 两个线程
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
void * callback(void *arg){
    printf("child thread id : %d",pthread_self());
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
    sleep(1);
    for(int i=0;i<5;i++){
         printf("%d\n",i);
    }
}