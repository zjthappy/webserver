/**
int socket(int domain, int type, int protocal)
三个参数分别代表：

domain 参数用来指定协议族，比如 AF_INET 用于 IPV4、AF_INET6 用于 IPV6、AF_LOCAL/AF_UNIX 用于本机；
type 参数用来指定通信特性，比如 SOCK_STREAM 表示的是字节流，对应 TCP、SOCK_DGRAM 表示的是数据报，对应 UDP、SOCK_RAW 表示的是原始套接字；
protocal 参数原本是用来指定通信协议的，但现在基本废弃。因为协议已经通过前面两个参数指定完成，protocol 目前一般写成 0 即可；

服务端和客户端初始化 socket，得到文件描述符；
服务端调用 bind，将绑定在 IP 地址和端口;
服务端调用 listen，进行监听；
服务端调用 accept，等待客户端连接；
客户端调用 connect，向服务器端的地址和端口发起连接请求；
服务端 accept 返回用于传输的 socket 的文件描述符；
客户端调用 write 写入数据；服务端调用 read 读取数据；
客户端断开连接时，会调用 close，那么服务端 read 读取数据的时候，就会读取到了 EOF，待处理完数据后，服务端调用 close，表示连接关闭。
*/

/*
目前只能实现一个客户端，通信一次
*/
#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
int main(){
    // 1.创建监听的socket
    int lfd =  socket( AF_INET, SOCK_STREAM,0);
    
    // 2.绑定 ip 端口
    struct sockaddr_in saddr;
    saddr.sin_family =AF_INET;
    // inet_pton(AF_INET,"127.0.0.1",saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999); 
    bind(lfd,(struct sockaddr*)&saddr,sizeof(saddr));
    // 3.监听
    int ret = listen(lfd,8);
    if(ret == -1){
        perror("listen:");
    }
    // 4.服务端调用 accept，等待客户端连接； (阻塞)
    struct sockaddr_in clientaddr;
    socklen_t socklen =sizeof(clientaddr);
    int cfd = accept(lfd,(struct sockaddr*)&clientaddr,&socklen);

    // 输出客户端的信息
    char clientIP[16];
    inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,clientIP,sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    printf("client ip is %s port is %d",clientIP,clientPort);

    //5. 通信
    // 接受client data
    char recvBuf[1024] = {0};
    int len = read(cfd,recvBuf,sizeof(recvBuf));
    if(len==-1){
        // 调用失败
    }else if(len>0){
        // 读到了数据
        printf("recv data from client %s:",recvBuf);
    }else if(len==0){
        // 客户端断开连接
          printf("client closed.......");

    }
    // send data to client
    char * data = "hello,i am server";
    write(cfd,data,strlen(data));

    // 关闭
    close(cfd);
    close(lfd);
    return 0;

}