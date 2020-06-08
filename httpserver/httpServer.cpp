#include "httpServer.h"

httpServer::httpServer(const size_t& port, char const* ip, const size_t& nthreads, const size_t log_level):m_port(port),m_ip(ip),m_threads(nthreads),m_log_level(log_level)
{


    //当服务器close一个连接时，若client端接着发数据。根据TCP协议的规定，会收到一个RST响应，client再往这个服务器发送数据时，系统会发出一个SIGPIPE信号给进程，告诉进程这个连接已经断开了，不要再写了。

    addsig(SIGPIPE, SIG_IGN);//忽略信号
    try
    {
        users.resize(MAX_FD);//预先定义vector内存大小
        pool = new threadpool<http_conn>(nthreads); //分配线程池大小
        LOG(DEBUG) << "*****  服务器启动  *****";
        LOG(DEBUG) << "服务器的信息：";
        LOG(DEBUG) << "地址：" << ip << "  端口：" << port << "  启动线程：" << nthreads;
    }
    catch (const std::exception&)
    {
        if (pool) {
            delete pool;
        }
        LOG(ERROR)   << "***** 线程池分配内存错误  *****";
        throw "内存错误";
    }
    user_count = 0;
}

void httpServer::event_listen()
{

    m_listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(m_listenfd > 0);
    //struct linger tmp = { 1,0 };//优雅关闭，立即关闭
   // setsockopt(m_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, m_ip, &address.sin_addr);//inet_pton是一个IP地址转换函数，可以在将IP地址在“点分十进制”和“二进制整数”之间转换
    address.sin_port = htons(m_port);
    ret = bind(m_listenfd, (struct sockaddr*) & address, sizeof(address));
    assert(ret >= 0);
    ret = listen(m_listenfd, 50);
    assert(ret >= 0);
    epollfd = epoll_create(50);
    //   assert(epollfd==-1);
    addfd(epollfd, m_listenfd, false);
    http_conn::m_epolled = epollfd;

    //这里直接打印到终端，而不用记录
    LOG(DEBUG) << "*****  服务器监听成功！！！  *****";
    
}

void httpServer::event_loop()
{
    while (true) {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);

        if ((number < 0) && (errno != EINTR)) {
        	
		LOG(ERROR) << "epoll failure";
            break;
        }
        for (int i = 0; i < number; ++i) {

            int sockfd = events[i].data.fd;
            if (sockfd == m_listenfd) {//有新用户连接
                struct sockaddr_in client_address;
                socklen_t client_addrlenth = sizeof(client_address);
                int connfd = accept(m_listenfd, (sockaddr*)&client_address, &client_addrlenth);
                LOG(INFO)    << "***** 用户连接：  *****";
                LOG(INFO) << "connfd is " << &connfd << "  address is " << &client_address;

                if (connfd < 0) {
                    LOG(WARNING) << "连接新用户出错：" << &errno;
                    continue;
                }
                if (http_conn::m_user_count > MAX_FD) {
                    LOG(WARNING) << "internal server busy, the new request will close";
                    LOG(WARNING) << "close connfd is " << &connfd << "  address is " << &client_address;
                    show_error(connfd, "internal serverbusy");
                    continue;
                }
                users[connfd].init(connfd, client_address);
            }
            //tcp连接被对方关闭，或者对方关闭了写操作
            //挂起，比如管道的写端被关闭，读端将会收到这个信号
            //错误
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                    users[sockfd].close_conn();
            }
            else if (events[i].events & (EPOLLIN)) {
                if (users[sockfd].read()) {
                    pool->append(&users[sockfd]);//将该套接字追到请求队列中
                }
                else {
                    LOG(WARNING) << "读取用户套接字失败，关闭该套接字";
                    users[sockfd].close_conn();
                }
            }
            else if (events[i].events & EPOLLOUT) {
                if (!users[sockfd].write())
                {
                    users[sockfd].close_conn();
                }
            }
            else {
            }
        }
    }
}

httpServer::~httpServer()
{
    users.clear();
    close(epollfd);
    close(m_listenfd);
    delete pool;
}
 
