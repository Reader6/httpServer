#include "serialization.h"



serialiZation::serialiZation(){


}

serialiZation::~serialiZation(){

}
void serialiZation::init_state(){


}

commen::RESULT_CODE serialiZation::parse_request_line(char *text)
{	
	m_url = strpbrk(text, " "); //取得\t的位置  在text中寻找第一个匹配\t的元素
	if (!m_url) {
		return BAD_REQUEST;    //如果没有寻找到 说明格式错误
	}

	*m_url++ = '\0'; //前面分隔 获得空格指向的下一个位置
	char* method = text;//text;
	if (strcasecmp(method, "GET") == 0) {
		m_quest_method = GET;//如果请求方式为GET
	}
	else {
		return BAD_REQUEST;//暂时只支持GET
	}
	m_url += strspn(m_url, " ");//消除连续的空格
	m_version = strpbrk(m_url, " ");//然后在寻找下一个"\t"
	if (!m_version) {
		return BAD_REQUEST;
	}
	*m_version++ = '\0';
	if (strncasecmp(m_version, "HTTP/1.1", 7) != 0) {
		return BAD_REQUEST;
	}
	m_version += strspn(m_version, "\r\n");
	if (strncasecmp(m_url, "http://", 7) == 0) {
		m_url += 7;
		m_url = strchr(m_url, '/');
	}
	if (!m_url || m_url[0] != '/') {
		return BAD_REQUEST;
	}

	m_check_state = CHEACK_STATE_HEADER;
	return NO_REQUEST;
}

commen::RESULT_CODE serialiZation::parse_headers(char* text)
{
	if (text[0] == '\0') {
		if (m_content_length != 0) {
			m_check_state = CHEACK_STATE_CONTENT;
			return NO_REQUEST;
		}
		return GET_REQUEST;
	}
	else if (strncasecmp(text, "Connection:", 11) == 0) {
		text += 11;
		text += strspn(text, " ");
		if (strcasecmp(text, "keep-alive") == 0) {
			m_link = true;
		}
	}
	else if (strncasecmp(text, "Content-Length:15", 15) == 0) {
		text += 15;
		text += strspn(text, " ");
		m_content_length = atoi(text);
	}
	else if (strncasecmp(text, "Host:", 5) == 0) {
		text += 5;
		text += strspn(text, " ");
		m_host = text;
	}
	else if (strncasecmp(text, "Accept-Encoding:", 16) == 0) {
		text += 16;
		text += strspn(text, " ");
		m_encode_method = text;
	}
	else if (strncasecmp(text, "User-Agent:", 11) == 0) {
		text += 11;
		text += strspn(text, " ");
		m_user_agent = text;
	}
	else if (strncasecmp(text, "Accept:", 7) == 0) {
		text += 7;
		text += strspn(text, " ");
	}
}
	


