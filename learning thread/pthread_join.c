/**
 * 可以把一个程序看成只有一个线程 main所在的就是主线程
 * 其他成为子线程
 * 
 * 程序中默认只有一个线程，调用pthreadcreate 两个线程
 * 
 * 线程连接可以回收其他线程的资源
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int value = 10;
void * callback(void *arg){
    printf("child thread id : %ld\n",pthread_self());
    // return NULL;
    sleep(3);
  
    //
    pthread_exit((void *)&value);

}


 int main(){
    pthread_t tid ; 
    // tid , , 线程逻辑, 参数
     int ret = pthread_create(&tid,NULL,callback,NULL);

    if(ret!=0){
        char * errstr = strerror(tid);
        printf("error  :%s\n",errstr);
    }
    // sleep(1);
    for(int i=0;i<5;i++){
         printf("%d\n",i);
    }

    printf("parentid:%ld childid:%ld\n",pthread_self(),tid);

    int * thread_ret;
    // 阻塞函数 每次只能连接一个终止子线程

    // 巧妙的地方在于 pthread_exit((void *)&value);传递的是指针
    // pthread_join是二级指针 
    pthread_join(tid,(void**)(&thread_ret));

    printf("子线程返回的数据：%d\n",*thread_ret);
    return 0;
}