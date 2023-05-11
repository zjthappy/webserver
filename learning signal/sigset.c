#include <stdio.h>
#include <signal.h>

int main(){
    // 创建信号集合
    sigset_t set;
    // 清空信号及内容
    sigemptyset(&set);

    // 判断
    int ret =sigismember(&set,SIGINT);
    if(ret == 0){

    }

    // 添加信号
    sigaddset(&set,SIGSTOP);
    // 删除
    sigdelset(&set,SIGSTOP);

    
    // 添加若干信号
    return 0;
}