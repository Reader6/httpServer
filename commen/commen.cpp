#include "commen.h"
int setnonblacking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);//ET模式  一次读取
    int new_option = old_option | O_NONBLOCK;//非堵塞
    fcntl(fd, F_SETFL, new_option);//添加新方法
    return old_option;//返回之前设置

};
void addfd(int epollfd, int fd, bool one_shot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;//注册接受  挂起 事件
    if (one_shot) {//防止多个线程同时处理一个连接
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblacking(fd);
}
void removefd(int epolled, int fd) {
    epoll_ctl(epolled, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}
void modfd(int epolled, int fd, int ev) {
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epolled, EPOLL_CTL_MOD, fd, &event);
}
void addsig(int sig, void (handler)(int), bool restart) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if (restart) {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}
void show_error(int connfd, const char* info) {
    printf("%s", info);
    send(connfd, info, strlen(info), 0);
    close(connfd);
} 
