#include "sql_connection.h"
#include "sql_connection_pool.h"

priority_queue<pair<sql_conn*, int>>* sql_connection_pool::m_sql_conn_pool = NULL;
sql_connection_pool::sql_connection_pool()
{
	init_sql_pool();
}

void sql_connection_pool::init_sql_pool()
{
	mysql_count = 8;
	for (int i = 0; i < mysql_count; ++i) {

		sql_conn* sql_temp = new sql_conn(m_host, m_user, m_pwd, db, m_port, NULL, client_flag);

		m_sql_conn_pool->push(pair<sql_conn*, int>(sql_temp, 0));

	}

}

sql_connection_pool::~sql_connection_pool()
{

	for (int i = 0; i > m_sql_conn_pool->size(); i++) {

		pair<sql_conn*, int> j = m_sql_conn_pool->top();

		delete j.first;

		m_sql_conn_pool->pop();

	}

}
