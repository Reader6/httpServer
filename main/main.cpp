#include <iostream>

#include "httpserver/httpServer.h"
#include "mysqldb/mysql_guard.h"
#include "log/easylogging++.h"
#include <mysql/mysql.h>
using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{

	{
		mysql_guard sql_guard;
		try
		{
			char temp[400];
			char *tbname = "user";
			int rt;
			sprintf(temp,"select * from %s;",tbname);
			MYSQL_RES *res;
			MYSQL_ROW row;
			rt = mysql_real_query(sql_guard(),temp,strlen(temp));
			if(rt == 0){
				cout<<"select * from "<<tbname<<": sucess!"<<endl;
				
				res = mysql_store_result(sql_guard());
				while (row = mysql_fetch_row(res)){
		
					for(int i = 0; i < mysql_num_fields(res);i++){
						cout<< row[i]<< " "; 
					}		
					cout<<endl;
				}
			
			}else{
				cout<<"select * from "<<tbname<<": error !: "<<mysql_error(sql_guard())<<endl;

			}
		
		}
		catch (string& error_msg)
		{
			cout << error_msg << endl;
			return 0;
		}
		catch (...)
		{
			cout << "MySQL operation is error!" << endl;
			return 0;
		}

		cout << "MySQL is OK." << endl;
		return 1;
	};
	/*
	//加载日志配置文件
	el::Configurations conf("my_log.conf");
	el::Loggers::reconfigureAllLoggers(conf);
	const char* ip = "0.0.0.0";


	int port = 80;
	//端口号 ip地址 线程数
	httpServer* Server = new httpServer(port, ip, 64, 0);
	Server->event_listen();
	Server->event_loop();
	*/
	return 0;
}

