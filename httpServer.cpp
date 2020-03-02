#include "httpServer.h"

httpServer::httpServer(const size_t& port, char const* ip, const size_t& nthreads, const size_t log_level):m_port(port),m_ip(ip),m_threads(nthreads),m_log_level(log_level)
{

    addsig(SIGPIPE, SIG_IGN);//忽略信号
    try
    {
        users.resize(MAX_FD);//预先定义vector内存大小
        pool = new threadpool<http_conn>(nthreads); //分配线程池大小
    }
    catch (const std::exception&)
    {
        if (pool) {
            delete pool;
        }
        cout << "内存错误" << endl;
        throw "内存错误";
    }
    user_count = 0;
}

void httpServer::event_listen()
{

    m_listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(m_listenfd > 0);
    struct linger tmp = { 1,0 };//优雅关闭，立即关闭
    setsockopt(m_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, m_ip, &address.sin_addr);
    address.sin_port = htons(m_port);
    ret = bind(m_listenfd, (struct sockaddr*) & address, sizeof(address));
    assert(ret >= 0);
    ret = listen(m_listenfd, 50);
    assert(ret >= 0);
    epollfd = epoll_create(50); 
    //   assert(epollfd==-1);
    addfd(epollfd, m_listenfd, false);
    http_conn::m_epolled = epollfd;
}

void httpServer::event_loop()
{
    while (true) {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);

        if ((number < 0) && (errno != EINTR)) {
            printf("epoll failutr");
            break;
        }
        for (int i = 0; i < number; ++i) {

            int sockfd = events[i].data.fd;
            if (sockfd == m_listenfd) {
                struct sockaddr_in client_address;
                socklen_t client_addrlenth = sizeof(client_address);
                int connfd = accept(m_listenfd, (sockaddr*)&client_address, &client_addrlenth);
                if (connfd < 0) {
                    printf("error is:%d\n", errno);
                    continue;
                }
                if (http_conn::m_user_count > MAX_FD) {
                    show_error(connfd, "internal serverbusy");
                    continue;
                }
                users[connfd].init(connfd, client_address);
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                users[sockfd].close_conn();
            }
            else if (events[i].events & (EPOLLIN)) {
                if (users[sockfd].read()) {
                    pool->append(&users[sockfd]);
                }
                else {
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
