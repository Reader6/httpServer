#ifndef THREADPOOL_H
#define THREADPOOL_H
#include<list>
#include<cstdio>
#include<iostream>
#include<pthread.h>
#include"locker.h"
#include"easylogging++.h"
template <typename T>
class threadpool{
public:
    threadpool(int thread_number=8,int max_requests=10000);//线程数  最大请求数
    ~threadpool();
    bool append(T *request);
private:
    static void *worker(void *arg);
    void run();
private:
    int m_thread_number;    //线程池中的线程数
    int m_max_requests; // 最大请求队列
    pthread_t *m_threads;   // 描述线程池的数组
    std::list<T*> m_workqueue;  //请求队列
    locker m_queuelocker;   //保护请求队列的互斥锁
    sem m_queuestat;    //是否有重要任务需要处理
    bool m_stop;    //是否结束线程
};

template<typename T>
threadpool<T>::threadpool(int thread_number, int max_requests):m_thread_number(thread_number),m_max_requests(max_requests),m_stop(false),m_threads(NULL)
{
    if(thread_number<=0|| max_requests<=0){
        throw std::exception();
    }
    m_threads=new pthread_t[m_thread_number];
    if(!m_threads){
        LOG(ERROR) << "线程创建时，分配内存出现错误";
        throw std::exception();
    }
    for(int i=0;i<thread_number;++i){
        LOG(DEBUG) << "create thr " << i << "th thread";
        //printf("create thr %dth thread\n",i);
        if(pthread_create(m_threads+i,NULL,worker,this)!=0){
            delete []m_threads;
            throw std::exception();
        }
        //pthread_join()函数的替代函数，可回收创建时detachstate属性设置为PTHREAD_CREATE_JOINABLE的线程的存储空间
        if(pthread_detach(m_threads[i])){
            delete []m_threads;
            throw std::exception();
        }
    }
}
template<typename T> threadpool<T>::~threadpool()
{
    delete [] m_threads;
    m_stop=true;
}
template<typename T>
bool threadpool<T>::append(T *request)
{
    m_queuelocker.lock();
    if(m_workqueue.size()>m_max_requests){
        m_queuelocker.unlock();
        LOG(DEBUG) << "请求数过多，暂时不能服务";
        return false;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();                   //生产者消费者问题
    LOG(DEBUG) << "追加请求成功";
    return  true;
}
template<typename T>
void *threadpool<T>::worker(void *arg)  //线程执行函数
{
       threadpool *pool=(threadpool*)arg;
       pool->run();
       return pool;
}
template<typename T>
void threadpool<T>::run()
{
    while (!m_stop) {
        m_queuestat.wait();
        m_queuelocker.lock();
        if(m_workqueue.empty()){
            m_queuelocker.unlock();
            LOG(DEBUG) << "请求队列为空，继续重新等待";
            continue;
        }
        T *request=m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();

        if(!request){
            LOG(WARNING) << "当前没有请求，继续重新等待";
            //std::cout << "warn 当前没有请求" << std::endl;
            continue;
        }
        request->process();
        std::cout<< "thread_id"<<pthread_self() << "ok" << std::endl;
    }
}
#endif // THREADPOOL_H
