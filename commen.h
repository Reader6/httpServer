#pragma once
//…Ë÷√∑«◊Ë»˚
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<sys/stat.h>
#include<string.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/uio.h>
#include<stdarg.h>
#include<errno.h>
#define MAX_FD  65536
#define MAX_EVENT_NUMBER 10000
int setnonblacking(int fd);
void addfd(int epollfd, int fd, bool one_shot);
void removefd(int epolled, int fd);
void modfd(int epolled, int fd, int ev);
void addsig(int sig, void (handler)(int), bool restart=true );
void show_error(int connfd, const char* info);