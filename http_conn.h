#ifndef HTTPCONN_H
#define  HTTPCONN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "locker.h"

class http_conn{
    public:
        //静态 所有对象共享
        static int m_epollfd;
        static int m_user_count;
        http_conn(){};
        ~http_conn(){};

        // 处理客户端的请求
        void process();

        void init(int sockfd,const sockaddr_in & addr);

        void close_conn();

        // 非阻塞读写
        bool read();
        bool write();
    private:
        int m_socketfd;
        sockaddr_in m_address;

};

#endif