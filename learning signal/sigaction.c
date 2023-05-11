/**
 * 捕捉信号函数
 * sigaction(int signum,const struct sigaction *act,struct *act)
    const struct *
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

    struct sigaction act;
    // 设置是一嗯哪一个
    act.sa_flags=0;
    act.sa_handler=my;
    // 临时清空阻塞信号集合
    sigemptyset(&act.sa_mask);
    // 注册信号捕捉

    sigaction(SIGALRM,&act,NULL);
   

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
    while(1){};

}
