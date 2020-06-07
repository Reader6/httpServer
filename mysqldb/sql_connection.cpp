#include "sql_connection.h"

sql_conn::sql_conn(const string& host, const string& user, const string& pwd, const string& db, size_t port, const string& unix_socket, size_t client_flag)
{
	connect_Db(host, user, pwd, db, port, unix_socket, client_flag);

}

bool sql_conn::connect_Db(const string& host, const string& Suser, const string& pwd, const string& db, size_t port, const string& unix_socket, size_t client_flag)
{
	mysql_conn= mysql_init(NULL);
	char server[] = "localhost";
	char user[] = "root";
	char password[] = "root";
	char database[] = "mysql";
	if (!mysql_real_connect(mysql_conn, server, user, password, database, 0, NULL, 0)) {
		cout << "database connetion failed " << mysql_error(mysql_conn) << endl;
		return true;
	}
	cout << "database connection sucesss" << endl;
}

MYSQL* sql_conn::get_conn()
{

	return mysql_conn;
}

sql_conn::~sql_conn()
{

	mysql_close(mysql_conn);
	cout << "Îö¹¹³É¹¦" << endl;
}
