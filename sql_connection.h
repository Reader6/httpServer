#pragma once
#include<iostream>
#include <list>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include "./locker.h"
#include <mysql/mysql.h>
#include <string>
using namespace std;
class sql_conn
{
public:
	/*
		���캯�� �������Ӷ���
		host:����
		user:�û�
		pwd:����
		db:���ݿ�
		port:�˿�
		unix_socket:�Ƿ�ʹ��socket���Ƶ�¼
		client_Flag ���ӱ�־

	*/
	sql_conn(const string& host, const string& user,
		const string& pwd, const string& db,
		size_t port, const string& unix_socket,
		size_t client_flag);
	bool connect_Db(const string& host, const string& user,
		const string& pwd, const string& db,
		size_t port, const string& unix_socket,
		size_t client_flag);
	MYSQL* get_conn();
	~sql_conn();


private:
	MYSQL* mysql_conn;//���ݿ�����
};
