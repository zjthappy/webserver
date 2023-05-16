#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "locker.h"
#include  "threadpool.h"
#include "http_conn.h"

#define MAX_FD 65535 //最多的文件描述符个数

#define MAX_EVENT_NUMBER 10000 

/*
通用的信号捕捉函数
信号 信号处理句柄
*/
void addsig(int sig,void(handler)(int)){
    
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.sa_handler = handler;
    // sa_mask是临时阻塞信号集,sigfillset将64都置1,相当于阻塞所有信号
    sigfillset(&sa.sa_mask);
    // 注册信号捕捉
    // 要捕捉的信号 sa结构体
    sigaction(sig,&sa,NULL);

}

//extern调用其他源文件中的变量或者函数(不需要include)
extern int addfd(int epollfd,int fd,bool one_shot );

// 从epoll中删除文件描述符
extern int removefd(int epollfd,int fd);
// 修改
extern int modfd(int epollfd,int fd,int ev);


int main(int argc,char* argv[]){
    if(argc<=1){
        /*zjt add :basename是啥*/
        printf("按照如下格式运行：%s port_number\n",basename(argv[0]));
        exit(-1);
    }
    // 端口号 字符串转整数
    int port = atoi(argv[1]);

    // 对sigpie信号进行处理,之后忽略操作(默认是终止)
    addsig(SIGPIPE,SIG_IGN);

    // 创建线程池,并初始化
    // proact模型,这里http_conn是任务类
    threadpool<http_conn> * pool =nullptr;
    try{
        pool =new threadpool<http_conn>();
    }
    // ... catch 块能够处理 try 块抛出的任何类型的异常
    catch(...){
        exit(-1);
    }

    // 创建数组保存 所有的客户端信息
    http_conn * users = new http_conn[MAX_FD];
    // 监听
    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    // 设置端口复用
    int reuse=1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    // 绑定
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr= INADDR_ANY;
    address.sin_port= htons(port);
    bind(listenfd,(struct sockaddr *)&address,sizeof(address));

    // 监听
    listen(listenfd,5);

    // 创建epoll对象
    struct epoll_event events[MAX_EVENT_NUMBER];
    int epollfd  =epoll_create(5);

    // 将监听的文件描述符
    addfd(epollfd,listenfd,false);
    http_conn::m_epollfd=epollfd;

    while(true){
        int num = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if((num<0)&&(errno !=EINTR)){
            printf("epoll failed\n");
            break;
        }
        // 循环遍历事件数组
        for(int i=0;i<num;i++){
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd){
                // 传出参数
                struct sockaddr_in clientaddr;
                socklen_t len =sizeof(clientaddr);
                int connfd = accept(listenfd,(struct sockaddr*)&clientaddr,&len);
                if(http_conn::m_user_count>=MAX_FD){
                    // 服务器能连接的客户端达到最大

                    close(connfd);
                    continue;
                }

                users[connfd].init(connfd,clientaddr);
            }else if(events[i].events &(EPOLLHUP|EPOLLRDHUP|EPOLLERR)){
                // 关闭连接
                users[sockfd].close_conn();
            }else if(events[i].events &(EPOLLIN)){
                if(users[sockfd].read()){

                }else{

                }
                
            }else if(events[i].events &(EPOLLOUT)){
                if(users[sockfd].write()){

                }else{
                    
                }
                
            }
        }
    }

    close(epollfd);
    close(listenfd);
    delete []users;
    delete pool;



}
