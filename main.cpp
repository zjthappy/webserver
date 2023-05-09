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

// 添加信号捕捉
void addsig(int sig,void(handler)(int)){
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.__sigaction_handler;

}

//添加文件描述符到epoll中 (zjt add : extern怎么用？？)
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

    // 对sigpie信号进行处理(zjt add不懂)
    addsig(SIGPIPE,SIG_IGN);

    // 创建线程池,并初始化

    threadpool<http_conn> * pool =nullptr;
    try{
        pool =new threadpool<http_conn>;
    }catch(){
        exit(-1);
    }

    // 创建数组保存 所有的客户端信息
    http_conn * users = new http_conn[MAX_FD];
    // 监听
    int lisenfd = socket(PF_INET,SOCK_STREAM,0);
    // 设置端口复用
    int reuse=1;
    setsockopt(lisenfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    // 绑定
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr= INADDR_ANY;
    address.sin_port= htons(port);
    bind(lisenfd,(struct sockaddr *)&address,sizeof(address));

    // 监听
    listen(lisenfd,5);

    // 创建epoll对象
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd  =epoll_create(5);

    // 将监听的文件描述符
    addfd(epollfd,lisenfd,false);
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
            if(sockfd == lisenfd){
                // 
            }
        }
    }



}
