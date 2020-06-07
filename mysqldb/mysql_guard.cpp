#include "mysql_guard.h"
#include "sql_connection_pool.h"
using namespace std;
sql_connection_pool sql_pool(8);
mysql_guard::mysql_guard()
{
	m_conn=sql_pool.get_sql_connetion_pool();
}

MYSQL* mysql_guard::operator()()
{
	return m_conn->first->get_conn();
}

mysql_guard::~mysql_guard()
{	
	if (m_conn) {
		sql_pool.push_sql_connetion_pool(m_conn);
	}
}
