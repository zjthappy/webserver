#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
int main(){
    // 1.创建socket
    int fd =  socket( AF_INET, SOCK_STREAM,0);
    // 2.connect server

    struct sockaddr_in saddr;
    saddr.sin_family =AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&saddr.sin_addr.s_addr);
    saddr.sin_port = htons(9999); 
    int ret =connect(fd,(struct sockaddr_in *)&saddr,sizeof(saddr));

     // 3. send data to server
    char * data = "hello,i am client";
    write(fd,data,strlen(data));

   // 4. read data
   
    char recvBuf[1024] = {0};
    int len = read(fd,recvBuf,sizeof(recvBuf));
    if(len==-1){
        // 调用失败
    }else if(len>0){
        // 读到了数据
        printf("recv data from server %s:",recvBuf);
    }else if(len==0){
        // 客户端断开连接
          printf("server closed.......");

    }
}