#ifndef THREADPOOL_H
#define THREADPOOL_H
// 头文件
#include<pthread.h>
#include<list>
#include<exception>
#include<cstdio>
#include "locker.h"
using namespace std;
// 线程池 模板类
template<typename T>
class threadpool{
    public:
        threadpool(int thread_number=8,int max_requests=10000);
        ~threadpool();
        bool append(T* request);
        void run();

    private:
        static void * worker(void * arg);
    private:
        
        // 线程数量
        int m_thread_number;

        // 线程池数组 大小为m_thread_number
        pthread_t * m_threads;

        // 请求队列中最多允许的请求数量
        int m_max_requests;

        // 请求队列
        list<T*>m_workqueue;

        // 互斥锁 控制访问请求队列
        locker m_queuelocker;

        // 信号量 是否有任务需要处理
        sem m_queuestat;

        // 是否结束线程
        bool m_stop;    

        threadpool(/* args */);
        ~threadpool();
};


template<typename T>
threadpool<T>::threadpool(int pthread_number,int max_requests):
    m_thread_number(pthread_number),m_max_requests(max_requests),
    m_threads(nullptr),m_stop(false)
{
    if(pthread_number<=0 || max_requests<=0){
        throw exception();
    }
    // 创建线程池数组
    m_threads = new pthread_t[m_thread_number];

    if(!mthreads){
        throw exception();
    }

    // 创建线程
    for(int i=;i<pthread_number;i++){
        printf("create the %dth thread! ",i);

        if(pthread_create(m_threads+i,nullptr,worker,this)!=0){
            delete [] m_threads;
            throw exception();
        }

        if(pthread_detach(m_threads[i])){
            delete [] m_threads;
            throw exception();
        }

    }
}

template<typename T>
threadpool<T>::~threadpool(){
    delete [] m_threads;
    m_stop=true;
}

template<typename T>
bool threadpool<T>::append(T * request){
    m_queuelocker.lock();
    if(m_workqueue.size() >m_max_requests){
         m_queuelocker.unlock();
         return false;
    }
    m_workqueue.push_back(request);

    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}

// 静态函数，arg是this指针？？
template<typename T>
void * threadpool<T>::worker(void * arg){
    threadpool* pool = (threadpool*)arg;
    pool->run();
    return pool;
}

template<typename T>
void threadpool<T>::run(){
    // m_stop是线程停止的标志 线程什么时候停止 线程执行什么代码？？
    while(!m_stop){
        m_queuestat.wait();
        m_queuestat.lock();
        // 如果没有待处理的任务 解锁并继续循环
        if(m_workqueue.empty()){
            m_queuestat.unlock();
            continue;
        }

        // 获取
        T * request = m_workqueue.front();
        // 删掉
        m_workqueue.pop_front();
        // 解锁
        m_queuelocker.unlock();
        if(!request){
            continue;
        }
        // 执行任务
        request->process();




    }
}
/*
问题：
线程创建的方式；
各种锁 信号量的用法和区别
*/
#endif