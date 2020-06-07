#pragma once
#include <iostream>
#include "sql_connection.h"
using namespace std;
/*
	����RAII���ƹ������ݿ����ӳ�
	�����ܱ�֤���ݿ����Ӷ����������Ļص����ӳ��С�

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

