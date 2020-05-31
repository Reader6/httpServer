#include <iostream>

#include"httpServer.h"
#include "sql_connection_pool.h"
using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{
	//加载日志配置文件
	el::Configurations conf("/root/xhmelody/qreader/httpseverLog/httpServer-master/my_log.conf");
	el::Loggers::reconfigureAllLoggers(conf);
	const char* ip = "127.0.0.1";
	sql_connection_pool test;


	int port = 80;
	httpServer* Server = new httpServer(port, ip, 2, 0);
	Server->event_listen();
	Server->event_loop();
	return 0;
}

