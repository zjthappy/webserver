/*
    sigprocmask 检查和修改阻塞信号集

*/

// 编写程序 将常规信号味觉状态打印
// 设置某些信号阻塞

#include <stdio.h>

int main(){
    // 设置 2 3 号阻塞

    sigset_t set;
    sigemptyset(&set);

    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGQUIT);


    sigprocmask(SIG_BLOCK,&set,NULL);


}