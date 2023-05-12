#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<errno.h>
#include<wait.h>
int main(){ 

    // 1.创建监听的socket
    int lfd =  socket( AF_INET, SOCK_STREAM,0);
    
    // 2.绑定 ip 端口
    struct sockaddr_in saddr;
    saddr.sin_family =AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999); 
    bind(lfd,(struct sockaddr*)&saddr,sizeof(saddr));

    // 3.监听
    int ret = listen(lfd,8);

    // 创建
    fd_set rdset,tmp;
    FD_ZERO(&rdset);
    // 设置需要检测的文件描述符
    FD_SET(lfd,&rdset);
    int maxfd = lfd;


    while(1){
        tmp = rdset;
        int ret = select(maxfd+1,&tmp,NULL,NULL,NULL);

        if(ret == -1){
            perror("select");

        }else if(ret==0){
            continue;

        }else if(ret>0){
            // 如果新的客户段连接
            if(FD_ISSET(lfd,&rdset)){
                struct sockaddr_in clientaddr;
                socklen_t socklen =sizeof(clientaddr);
                // 如果在这里阻塞时 紫禁城结束，就会处理信号 
                // 然后处理晚后accept就不会阻塞了 但如果此时没有连接 就会报错
                int cfd = accept(lfd,(struct sockaddr*)&clientaddr,&socklen);
                // 如果有新的连接，将文件描述符添加到set中
                FD_SET(cfd,&rdset);
                maxfd = maxfd > cfd ?maxfd:cfd;

            }

            // 循环遍历其他通信描述符
            for(int i=lfd+1;i<maxfd;i++){
                if(FD_ISSET(i,&tmp)){
                    char buf [1024]={0};
                    int len = read(i,buf,sizeof(buf));
                    if(len==-1){
                         perror("read");
                    }else if(len >0){
                        
                        read(i,buf,sizeof(buf)+1);
                        printf("read buf= %s\n",buf);
                    }else if(len ==0){
                        printf("client closed...\n");
                        close(i);
                        FD_CLR(i,&rdset);
                    }

                }
            }

        }

    }

    close(lfd);



    return 0;


}