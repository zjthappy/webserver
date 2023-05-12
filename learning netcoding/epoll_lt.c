
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include<arpa/inet.h>
#include<string.h>
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


    // 用epoll create 创建epoll实例
    // 在内核中创建了一个数据eventpoll
    int epfd = epoll_create(100);

    // 将监听的文件描述符加入到epoll
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd =lfd;
    // 参数：x x x 事件(监听哪个文件描述符的什么事件)
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&epev);

    struct epoll_event epevs[1024];

    // 不断循环,调用api检测是否有发生变化的文件描述符
    // 
    while(1){
        // 获得检测后的数据
        // 参数：epoll的文件描述符 传出参数保存发生了变化的文件描述符信息 第二个参数数组大小  阻塞时间
        int ret = epoll_wait(epfd,epevs,1024,-1);
        if(ret ==-1){
            perror("epoll");
            exit(-1);
        }
        // 输出检测到了几个
        printf("ret = %d\n",ret);

        for(int i=0;i<ret;i++){
            int curfd = epevs[i].data.fd;
            // 如果监听文件描述符发生变化
            if(curfd==lfd){
                struct sockaddr_in clientaddr;
                int len =sizeof(clientaddr);
                // 如果在这里阻塞时 紫禁城结束，就会处理信号 
                // 然后处理晚后accept就不会阻塞了 但如果此时没有连接 就会报错
                int cfd = accept(lfd,(struct sockaddr*)&clientaddr,&len);
                // 将新的连接加入到epoll
                epev.events = EPOLLIN;
                epev.data.fd =cfd;
                // 参数：x x x 事件(监听哪个文件描述符的什么事件)
                epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev);
            }else{
                // 数据到达
                char buf [5]={0};
                printf("读到client数据\n");
                int len = read(curfd,buf,sizeof(buf));
                if(len==-1){
                    perror("read");
                }else if(len ==0){
                    printf("client closed...\n");
                    epoll_ctl(epfd,EPOLL_CTL_DEL,curfd,NULL);
                    close(curfd);
                    
                }else if(len >0){  
                    printf("read buf:%s\n",buf);
                    write(curfd,buf,strlen(buf)+1);
                }
            }
        }


    }

    close(lfd);
    close(epfd);
}