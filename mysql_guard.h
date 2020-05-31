#pragma once
#include <iostream>
#include "sql_connection.h"
using namespace std;
/*
	利用RAII机制管理数据库连接池
	尽可能保证数据库连接对象进可能早的回到连接池中。

*/
class mysql_guard
{
public:
	mysql_guard();
	MYSQL* operator()();
	~mysql_guard();

private:
	pair<sql_conn*, int>* m_conn;
};

