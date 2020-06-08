#include <iostream>

#include "httpserver/httpServer.h"
#include "mysqldb/mysql_guard.h"
#include "log/easylogging++.h"
using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{
/*
	{
		mysql_guard sql_guard;
		try
		{
			MYSQL_RES* result = mysql_store_result(sql_guard());
			string sql = "select * from user;";
			mysql_query(sql_guard(), sql.c_str());
			if (!result)
				throw string("MySQL not result!");

			//获取字段数量
			int num_fields = mysql_num_fields(result);
			if (0 == num_fields)
				throw string("MySQL fields number is 0!");

			//获取字段名
			MYSQL_FIELD* fields = mysql_fetch_fields(result);
			if (!fields)
				throw string("MySQL fields fetch is error!");

			for (int i = 0; i < num_fields; i++)
			{
				cout << "field " << i << " name is " << fields[i].name << endl;
			}

			cout << "MySQL is OK." << endl;
		}
		catch (string& error_msg)
		{
			cout << error_msg << endl;
		}
		catch (...)
		{
			cout << "MySQL operation is error!" << endl;
		}
		cout << "MySQL is OK." << endl;
	};
	*/
	//加载日志配置文件
	el::Configurations conf("my_log.conf");
	el::Loggers::reconfigureAllLoggers(conf);
	const char* ip = "0.0.0.0";


	int port = 80;
	httpServer* Server = new httpServer(port, ip, 2, 0);
	Server->event_listen();
	Server->event_loop();
	return 0;
}

