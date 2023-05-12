#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/poll.h>
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

    // 初始化检测的文件描述符数组
    struct pollfd fds[1024];
    for(int i=0;i<1024;i++){
        fds[i].fd =-1;
        // 要监听的事件
        fds[i].events =POLLIN;
    }
    fds[0].fd =lfd;
    int nfds=0;

    while(1){
        int ret = poll(fds,nfds+1,-1);
        if(ret == -1){
            perror("poll");
        }else if(ret==0){
            continue;
        }else if(ret>0){
            // 如果新的客户段连接
            if(fds[0].revents & POLLIN){
                struct sockaddr_in clientaddr;
                socklen_t socklen =sizeof(clientaddr);
                // 如果在这里阻塞时 紫禁城结束，就会处理信号 
                // 然后处理晚后accept就不会阻塞了 但如果此时没有连接 就会报错
                int cfd = accept(lfd,(struct sockaddr*)&clientaddr,&socklen);
                // 如果有新的连接，将文件描述符添加到set中
                for(int i=1;i<1024;i++){
                    if(fds[i].fd==-1){
                        fds[i].fd=cfd;
                        fds[i].events=POLLIN;
                        break;
                    }
                   
                }
                nfds =nfds>cfd?nfds:cfd;

            }

            // 循环遍历其他通信描述符
            for(int i=lfd+1;i<nfds;i++){
                if(fds[i].revents & POLLIN){
                    char buf [1024]={0};
                    int len = read(fds[i].fd,buf,sizeof(buf));
                    if(len==-1){
                         perror("read");
                    }else if(len >0){
                        
                        read(fds[i].fd,buf,sizeof(buf)+1);
                        printf("read buf= %s\n",buf);
                    }else if(len ==0){
                        printf("client closed...\n");
                        close(fds[i].fd);
                        fds[i].fd=-1;
                    }

                }
            }

        }

    }

    close(lfd);



    return 0;


}