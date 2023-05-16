#include "http_conn.h"

int http_conn::m_epollfd=-1;
int http_conn::m_user_count=0;
// 设置文件描述符为非阻塞
void setnonblocking(int fd){
    int old_flag = fcntl(fd,F_GETFL);
    int new_flag = old_flag | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_flag);
}
// 向epoll中添加需要监听的文件描述符
void addfd(int epollfd,int fd,b ool one_shot){
    epoll_event event;
    event.data.fd =fd ;
    event.events = EPOLLIN  |  EPOLLRDHUP; 

    if(one_shot){
        event.events | EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);

    setnonblocking(fd);
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

 void http_conn::init(int sockfd,const sockaddr_in & addr){
    m_socketfd = sockfd;
    m_address =addr;

    int reuse=1;
    setsockopt(m_socketfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    addfd(m_epollfd,sockfd,true);
    m_user_count++;
 }
void http_conn::close_conn(){
    if(m_socketfd!=-1){
        removefd(m_epollfd,m_socketfd);
        m_socketfd =-1;
        m_user_count--;
    }
 }

 bool http_conn::read(){
    printf("一次性读完数据");

    return true;
 }

bool http_conn::write(){
    printf("一次性写完数据");
    return true;

 } 
 void http_conn::process(){
    // 解析http请求
       printf("解析请求,创建响应");

 } 