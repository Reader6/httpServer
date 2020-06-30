/*
 *   serialization.h
 *   author: rownh
 *   目的：将接受到http报文序列化为一个对象
 *   当写入时，在序列化为char文字
 *   
 * */

#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include "log/easylogging++.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

class serialiZation{

public:

	serialiZation();
	~seraliZation();


	//重置操作
	void init_state();
	
	//解析请求行
	commen::RESULT_CODE parse_request_line(char* text);
	
	//解析请求头
	commen::RESULT_CODE parse_headers(char* text);
	
	//解析请求内容
	commen::RESULT_CODE parse_content(char* text);
	
	//解析请求url
	commen::RESULT_CODE parse_url();

	//添加返回
	bool add_response(const char* format, ...); 
	
	//返回消息体
	bool add_content(const char* content);

	//状态码
	bool add_status_line(int status, const char* title);
	
	//消息长度
	bool add_content_length(int content_length);
	
	//连接长短
	bool add_linger();
	
	//响应内容类型
	bool add_content_type(const char* type);

	//间隔行
	bool add_blank_line();
	
	
	bool add_headers(int content_len);

	bool add_file(char * file);
public:

private:
	string buffer;
	commen::REQUEST_METHOD quest_method;
	char * m_url;
	char * m_version;
	char * m_host;
	char * m_encode_method;
	char * m_user_agent;
	bool m_link;
	char * m_file_address;
	struct iovec m_lv[2];
	ssize_t m_iv_count;

}


