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
	char database[] = "Qreader";
	char *query;
	mysql_conn = mysql_init((MYSQL *)0);
	int rt;
	if (mysql_conn != NULL  &&  mysql_real_connect(mysql_conn, server, user, password, database, 0, NULL, 0)) {
		if(!mysql_select_db(mysql_conn,database)) {
		cout<<"databse connection sucess "<<endl;
		mysql_conn->reconnect = 1;
		query = "set names \'utf8\'";
		rt = mysql_real_query(mysql_conn,query,strlen(query));
		if(rt){
			cout<<"Error making query " << mysql_error(mysql_conn)<<endl;
		}else{

			cout<<"querry sucess"<<endl;
		
		}
		}	
	}else{
		cout << "database connection failed" << endl;
	}
}
MYSQL* sql_conn::get_conn()
{

	return mysql_conn;
}

sql_conn::~sql_conn()
{

	mysql_close(mysql_conn);
	cout << "¿¿¿¿¿¿¿" << endl;
}
