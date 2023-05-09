/**
 * pthread_exit是线程自己终止
 * cancle是可以被别的线程终止  例如杀毒软件 扫描行为 
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int value = 10;
void * callback(void *arg){
    printf("child thread id : %ld\n",pthread_self());
    // return NULL;
    sleep(1);
      for(int i=0;i<5;i++){
         printf("child %d\n",i);
    }
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
    // 取消
    // sleep(1);

       pthread_cancel(tid);
    for(int i=0;i<5;i++){
         printf("%d\n",i);
    }
   

    
    return 0;
}