#pragma once
#include "sql_connection.h"
#include "locker.h"
#include <queue>

#include <map>
#include <algorithm>

using namespace std;

class sql_connection_pool
{

public:
	sql_connection_pool();
	~sql_connection_pool();
	static sql_conn* get_sql_connetion_pool() {

		if (m_sql_conn_pool->size()) {
			pair<sql_conn*, int>i = m_sql_conn_pool->top();
			m_sql_conn_pool->pop();
			i.second += 1;
			m_sql_conn_pool->push(i);
			return i.first;
		}
		cout << "为该连接分配数据库连接池" << endl;
		return nullptr;
	}


private:

	void init_sql_pool();
	//初始化创建连接池

private:
	static priority_queue<pair<sql_conn*, int>>*m_sql_conn_pool;

private:
	string m_host;
	string m_user;
	string m_pwd;
	string db;
	size_t m_port;
	int client_flag;
	size_t mysql_count;

};

