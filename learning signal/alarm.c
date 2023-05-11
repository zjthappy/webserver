/*
定时器alarm 到时间会发送SIGALARM信号 默认终止当前进程

*/
#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>

int main(){
    int seconds =alarm(1);
    int i=0;
    // 注意 定时器与状态无关 就算是slepp刮起 定时器仍然会继续计时
    sleep(5);
    while(1){
        printf("%d\n",i++);
    }

}