
#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
int main(){
    // 1.创建socket
    int fd =  socket( AF_INET, SOCK_STREAM,0);
    // 2.connect server

    struct sockaddr_in saddr;
    saddr.sin_family =AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&saddr.sin_addr.s_addr);
    saddr.sin_port = htons(9999); 
    int ret =connect(fd,(struct sockaddr *)&saddr,sizeof(saddr));

   
    
    int i = 0;

    // 循环 连接后不断和server通信
    while(1){
        char recvBuf[1024] = {0};
         // 3. send data to server
        fgets(recvBuf,sizeof(recvBuf),stdin);
        write(fd,recvBuf,strlen(recvBuf)+1);

        sleep(1);
       

           // 4. read data
        int len = read(fd,recvBuf,sizeof(recvBuf));

        
       
        if(len==-1){
            perror("read");
            exit(-1);
            // 调用失败
        }else if(len>0){
            // 读到了数据
            printf("recv data from server %s:",recvBuf);
        }else if(len==0){
            // 客户端断开连接
            printf("server closed.......");
            break;

        }
    }
    
}