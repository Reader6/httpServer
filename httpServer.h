#pragma once
#include <iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<vector>
#include<unistd.h>
#include<error.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<cassert>
#include<sys/epoll.h>
#include"locker.h"
#include"threadpool.h"
#include"http_conn.h"
#include"commen.h"
#include "easylogging++.h"
using namespace std;
class httpServer
{
public:
	explicit httpServer(const size_t& port, char const* ip, const size_t& nthreads, const size_t log_level);
	void event_listen();
	void event_loop();
	~httpServer();

private:
    size_t m_port;                          //端口
    const char* m_ip;                       //ip
	vector<http_conn> users;
	size_t user_count;
	threadpool<http_conn>* pool;
    epoll_event events[MAX_EVENT_NUMBER];  //最大连接数
	int epollfd;
	size_t m_listenfd;
	size_t m_threads;//线程数
	size_t m_log_level;
};

 
