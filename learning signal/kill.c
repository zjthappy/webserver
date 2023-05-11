/*

alarm(pid,sig)
*/
#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>


int main(){
    pid_t pid = fork();
    if(pid == 0){
        
    }else{
        // 杀死子进程 
        kill(pid,SIGINT);
    }
}