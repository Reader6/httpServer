#pragma once
//设置非阻塞
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

namespace commen{
 	static char* ok_200_title="OK";


	static char* error_400_title = "Bad Request";
	static char* error_400_form = "Your request has bad syntax or is in herently Impossible to staticfy.\n";
	static char* error_403_title = "Forbidden";

	static char* error_403_form = "You do not have permission to get file from this server.\n";
	static char* error_404_title = "Not Found";
	static char* error_404_form = "The requested file was not found on this server.\n";
	static char* error_500_title = "Internal Error";
       	static	char* error_500_form = "There was an unusual problem serving the requestd file\n";

	static char* doc_root = "/root/www";//网页根目录
}
