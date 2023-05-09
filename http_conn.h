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
        //静态  
        static int m_epollfd;
        static int m_user_count;
        http_conn(){};
        ~http_conn(){};

        // 处理客户端的请求
        void process();
    private:
        int m_socketfd;
        sockaddr_in m_address;

}

#endif