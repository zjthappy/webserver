#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include <errno.h>

#include <pthread.h>

struct sockInfo{
    int cfd;
    pthread_t tid;
    struct sockaddr_in addr;
};
// 紫禁城需要使用连接信息数据
struct sockInfo sockInfos[128];
void *working(void * arg){
            // 输出客户端的信息
            struct sockInfo * pinfo = (struct sockInfo *)arg;
            char clientIP[16];
            inet_ntop(AF_INET,&pinfo->addr.sin_addr.s_addr,clientIP,sizeof(clientIP));
            unsigned short clientPort = ntohs(pinfo->addr.sin_port);
            printf("client ip is %s port is %d",clientIP,clientPort);

            //5. 通信
            // 接受client data
            char recvBuf[1024] = {0};

            // 子线程会一直while循环进行数据读写通信
            while(1){
                
                int len = read(pinfo->cfd,recvBuf,sizeof(recvBuf));
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
                write(pinfo->cfd,data,strlen(data));
            }
           
            // 关闭
            close(pinfo->cfd);


}
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

    // 初始化数据

    int max = sizeof(sockInfos)/sizeof(sockInfos);
    for(int i = 0;i<max;i++){
        bzero(&sockInfos[i],sizeof(sockInfos[i]));
        sockInfos[i].cfd = -1;
         sockInfos[i].tid = -1;
    }

    while(1){
        // 4.服务端调用 accept，等待客户端连接； (阻塞)
        struct sockaddr_in clientaddr;
        socklen_t socklen =sizeof(clientaddr);
        // 如果在这里阻塞时 紫禁城结束，就会处理信号 
        // 然后处理晚后accept就不会阻塞了 但如果此时没有连接 就会报错
        int cfd = accept(lfd,(struct sockaddr*)&clientaddr,&socklen);
        if(cfd == -1){
            if(errno == EINTR){
                continue;
            }
            exit(-1);
        }


        struct sockInfo *pinfo;
        for(int i=0;i<max;i++){
            if(sockInfos[i].tid==-1){
                pinfo  = &sockInfos[i];
                break;
            }
            if(i==max-1){
                // 不能接受更多了 阻塞
                sleep(1);
                i--;
            }
        }
        pinfo->cfd = cfd;

        memcpy(&pinfo->addr,&clientaddr,socklen);

       
        pthread_create(&pinfo->tid,NULL,working,NULL);
        pthread_detach(pinfo->tid);

        
    
    }

    close(lfd);
}