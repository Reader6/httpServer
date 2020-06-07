#include "sql_connection.h"
#include "sql_connection_pool.h"
sql_connection_pool::sql_connection_pool(int count)
{
	mysql_count = count;
	init_sql_pool();
}

void sql_connection_pool::init_sql_pool()
{
	m_sql_conn_pool = new priority_queue<pair<sql_conn*, int>*>[mysql_count];
	m_host = "localhost";
	m_user = "root";
	m_pwd = "root";
	m_port = 0;
	db = "mysql";
	
	for (int i = 0; i < mysql_count; ++i) {

		sql_conn* sql_temp = new sql_conn(m_host, m_user, m_pwd, db, m_port,"", client_flag);
		pair<sql_conn*, int>*sql_struct = new pair<sql_conn*, int>(sql_temp, 0);
		m_sql_conn_pool->push(sql_struct);

	}

}


sql_connection_pool::~sql_connection_pool()
{

	for (int i = 0; i <m_sql_conn_pool->size(); i++) {

		pair<sql_conn*, int> *j = m_sql_conn_pool->top();

		delete j->first;

		m_sql_conn_pool->pop();

	}

}
