#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
int main(){
    printf("start .....\n");
     
    pid_t pid = fork();
    if(pid == 0){
        printf("我是子进程\n");
    }else{
           printf("我是父进程\n");
    }


    printf("测试。\n");
}