/*
setitimer ：可以周期性循环计时，每次定是结束发送信号
*/
#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/time.h>

// 
int main(){
    struct itimerval new_value;
    // 设置间隔时间
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;
    // 设置延迟 过多久执行
    new_value.it_value.tv_sec = 2;
    new_value.it_value.tv_usec = 0;
    // 非阻塞 
    setitimer(ITIMER_REAL,&new_value,NULL);
    // 等待两秒钟会发送sigalarm信号
    getchar();

}
