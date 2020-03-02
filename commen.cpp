#include "commen.h"
int setnonblacking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);//ETģʽ  һ�ζ�ȡ
    int new_option = old_option | O_NONBLOCK;//�Ƕ���
    fcntl(fd, F_SETFL, new_option);//����·���
    return old_option;//����֮ǰ����

};
void addfd(int epollfd, int fd, bool one_shot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;//ע�����  ���� �¼�
    if (one_shot) {//��ֹ����߳�ͬʱ����һ������
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