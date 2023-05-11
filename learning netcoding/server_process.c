/**
 * 多进程实现并发服务器
 * 代码流程：
 * 主进程一直循环监听新的连接
 * 如果有连接，就会创建新的文件描述符用来通信，并创建新的进程，在新的进程中处理
*/

#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>

#include<wait.h>

void recyleChild(int arg){
    while(1){
        //-1 all 
        int ret = waitpid(-1,NULL,WNOHANG);
        if(ret == -1){
            break;
        }else if(ret == 0){
            break;
        }
        else if(ret > 0){
            printf("");
        }
    }

}
int main(){

    // 注册 信号 捕捉
    struct sigaction act;
    act.sa_flags =0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recyleChild;
    sigaction(SIGCHLD,&act,NULL);
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
    while(1){
        // 4.服务端调用 accept，等待客户端连接； (阻塞)
        struct sockaddr_in clientaddr;
        socklen_t socklen =sizeof(clientaddr);
        int cfd = accept(lfd,(struct sockaddr*)&clientaddr,&socklen);

        pid_t pid = fork();

        if(pid == 0){
            // 紫禁城
            // 输出客户端的信息
            char clientIP[16];
            inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,clientIP,sizeof(clientIP));
            unsigned short clientPort = ntohs(clientaddr.sin_port);
            printf("client ip is %s port is %d",clientIP,clientPort);

            //5. 通信
            // 接受client data
            char recvBuf[1024] = {0};

            // 子线程会一直while循环进行数据读写通信
            while(1){
                 int len = read(cfd,recvBuf,sizeof(recvBuf));
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
                write(cfd,data,strlen(data));
            }
           
            // 关闭
            close(cfd);
            exit(0);
        }
        /*
        当上面的if执行之后，能否调用wait回收ne？
        不能在这里调用wait：
            wait是阻塞的 会一直停在这里 ，会影响后续其他客户端的连接
        能否调用waitpid非阻塞回收呢？
            参考评论的回答：父进程中直接调用 wait/waitpid 有个问题，就是可能一直没有新客户端连接，父进程会一直阻塞在 accept() 处，
            无法执行到 wait 处。所以要设置信号捕捉。这样父进程阻塞在 accept() 处时会被打断去回收子进程。
        */
       
    }
    close(lfd);
    return 0;
}