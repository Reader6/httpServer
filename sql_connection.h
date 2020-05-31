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
		构造函数 构建连接对象
		host:主机
		user:用户
		pwd:密码
		db:数据库
		port:端口
		unix_socket:是否使用socket机制登录
		client_Flag 连接标志

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
	MYSQL* mysql_conn;//数据库连接
};
