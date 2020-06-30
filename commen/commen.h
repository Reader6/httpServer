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

	enum REQUEST_METHOD {
		GET = 0,//获得特定资源
		POST,//向特定资源提交数据进行处理请求 数据包含在请求体中.POST请求可能导致新的资源创建
		HEAD,//与GET一样,响应题不返回,获得消息头的元信息
		PUT,//向指定资源上传最新内容
		DELETE,// 请求服务器删除Request_URL所标识的资源
		TRACE,//回显服务器收到的请求
		OPTIONS,    //返回服务器针对特定资源所支持的请求方法
		CONNECT,//连接改为管道的代理服务器
		PATCH //补丁
	};

	//主机状态
	enum CHEACK_STATE {
		CHEACK_STATE_REQUESTLINE = 0,//请求行
		CHEACK_STATE_HEADER, //请求头
		CHEACK_STATE_CONTENT  //请 content
	};

	//处理结果
	enum RESULT_CODE {
		NO_REQUEST,
		GET_REQUEST,
		BAD_REQUEST,
		NO_RESOURCE,
		FORBIDDEN_REQUEST,
		EN_REQUEST,
		FILE_REQUEST,
		INTERNAL_ERROR,
		CLOSED_CONNECTION
	};

	enum LINE_STATUS {
		LINE_OK = 0,  //一行读取成功
		LINE_BAD,   //错误格式
		LINE_OPEN   //数据未读完
	};

}
