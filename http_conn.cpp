#include "http_conn.h"

// 向epoll中添加需要监听的文件描述符
void addfd(int epollfd,int fd,bool one_shot){
    epoll_event event;
    event.data.fd =fd ;
    event.events = EPOLLIN  |  EPOLLRDHUP; 

    if(one_shot){
        event.events | EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);

}
// 移除
void removefd(int epollfd,int fd){
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
    
}
// 修改 重置socket上的EPOLLONESHOT事件  确保下一次可读时 epollin事件被触发
extern int modfd(int epollfd,int fd,int ev){
      epoll_event event;
      event.data.fd =fd;
      event.events =ev  | EPOLLONESHOT |EPOLLRDHUP;
      epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event );

}