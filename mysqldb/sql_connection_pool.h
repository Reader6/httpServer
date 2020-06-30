#pragma once
#include "sql_connection.h"
#include <queue>
#include <algorithm>

using namespace std;

class sql_connection_pool
{

public:
	sql_connection_pool(int count);
	~sql_connection_pool();
	//真真真
	pair<sql_conn*, int>* get_sql_connetion_pool() {

		if (m_sql_conn_pool->size()) {
			pair<sql_conn*, int>*i = m_sql_conn_pool->top();
			i->second += 1;
			m_sql_conn_pool->pop();
			return i;
		}
		cout <<"真真真真真"<< endl;
		return NULL;
	}
	//真真真
	void push_sql_connetion_pool(pair<sql_conn*, int>*i) {
		i->second -= 1;
		m_sql_conn_pool->push(i);
	}

private:
	//真�
	void init_sql_pool();

private:
	
	//真真�
	priority_queue<pair<sql_conn*, int>*>*m_sql_conn_pool;

private:
	string m_host;
	string m_user;
	string m_pwd;
	string db;
	size_t m_port;
	int client_flag;
	size_t mysql_count;

};

