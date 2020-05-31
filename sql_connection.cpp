#include "sql_connection.h"

sql_conn::sql_conn(const string& host, const string& user, const string& pwd, const string& db, size_t port, const string& unix_socket, size_t client_flag)
{
	connect_Db(host, user, pwd, db, port, unix_socket, client_flag);

}

bool sql_conn::connect_Db(const string& host, const string& user, const string& pwd, const string& db, size_t port, const string& unix_socket, size_t client_flag)
{
	mysql_init(mysql_conn);
	if (!mysql_real_connect(mysql_conn, host.c_str(), user.c_str(), pwd.c_str(), db.c_str(), port, unix_socket.c_str(), client_flag)) {
		cout << "数据库连接失败:" << mysql_error(mysql_conn) << endl;
		return true;
	}
	cout << "数据库连接成功" << endl;
}

sql_conn::~sql_conn()
{

	mysql_close(mysql_conn);
	cout << "析构成功" << endl;
}
