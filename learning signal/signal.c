/*
信号捕捉：
    进程在受到型号之后，如果不捕捉就会执行默认动作
*/


#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/time.h>

void my(int num){
    printf("捕捉到了信号\n");
}
int main(){
    // 注册信号捕捉
    signal(SIGALRM,my);

    struct itimerval new_value;
    // 设置间隔时间
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;
    // 设置延迟 过多久执行
    new_value.it_value.tv_sec = 2;
    new_value.it_value.tv_usec = 0;
    // 非阻塞 
    setitimer(ITIMER_REAL,&new_value,NULL);
    printf("定时器开始\n");

    // 等待两秒钟会发送sigalarm信号
    getchar();

}
