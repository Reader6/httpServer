#include "http_conn.h"
#include"commen/commen.h"


int http_conn::m_user_count = 0;
int http_conn::m_epolled = -1;
void http_conn::close_conn(bool real_close) {
	
	if (real_close && (m_socked != -1)) {
		removefd(m_epolled, m_socked);
		std::cout<<"socket:"<<m_socked<<" host:"<<m_host<<" close connection"<<std::endl;
		m_socked = -1;
		m_user_count--;
	}
}

void http_conn::process()
{

	RESULT_CODE read_ret = process_read();
	
	if (read_ret == NO_REQUEST) {

		modfd(m_epolled, m_socked, EPOLLIN);
		//异常
		return;
	}
	
	bool write_ret = process_write(read_ret);
	
	if (!write_ret) {

	   close_conn();
		
	}

	modfd(m_epolled, m_socked, EPOLLOUT);
	printf("finished one serverice");

}
void http_conn::init(int sockfd, const sockaddr_in& addr) {
	m_socked = sockfd;
	m_address = addr;
	int reuse = 1;
	setsockopt(m_socked, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	addfd(m_epolled, sockfd, true);
	m_user_count++;
	init_state();
}
void http_conn::init_state() {
	m_check_state = CHEACK_STATE_REQUESTLINE;
	m_link = false;//默认不保持连接
	m_quest_method = GET;
	m_url = 0;
	m_version = 0;
	m_content_length = 0;
	m_host = 0;
	m_start_line = 0;
	m_check_state = CHEACK_STATE_REQUESTLINE;
	m_read_idx = 0;
	m_write_idx = 0;
	m_checked_idx = 0;
	memset(m_read_buf, '\0', READ_BUFFER_SIZE);
	memset(m_write_buf, '\0', WRITE_BUFFER_SIZE);
	memset(m_real_file, '\0', FILENAME_LEN);
}

http_conn::RESULT_CODE http_conn::process_read()//main state mache
{
	LINE_STATUS line_status = LINE_OK;
	RESULT_CODE ret = NO_REQUEST;
	char* text = 0;
	while (((m_check_state == CHEACK_STATE_CONTENT) && (line_status == LINE_OK)) || ((line_status = parse_line()) == LINE_OK)) {
		text = get_line();
		m_start_line = m_checked_idx;
		switch (m_check_state) {
		case CHEACK_STATE_REQUESTLINE: {
			ret = parse_request_line(text);
			if (ret == BAD_REQUEST) {
				return BAD_REQUEST;
			}
			break;
		}
		case CHEACK_STATE_HEADER: {
			ret = parse_headers(text);
			if (ret == BAD_REQUEST) {
				return  BAD_REQUEST;

			}
			else if (ret == GET_REQUEST) {
				return do_request();
			}
			break;
		}
		case CHEACK_STATE_CONTENT:
		{
			ret = parse_content(text);
			if (ret == GET_REQUEST) {
				return do_request();
			}
			line_status = LINE_OPEN;
			break;
		}
		default: {
			return INTERNAL_ERROR;
		}
		}

	}
	return NO_REQUEST;
}

bool http_conn::process_write(http_conn::RESULT_CODE ret)
{
	switch (ret) {
	case INTERNAL_ERROR: {
		add_status_line(500,commen::error_500_title);
		add_headers(strlen(commen::error_500_form));
		if (!add_content(commen::error_500_form)) {
			return false;
		}
		break;
	}
	case BAD_REQUEST: {
		add_status_line(400, commen::error_400_title);
		add_headers(strlen(commen::error_400_form));
		if (!add_content(commen::error_400_form)) {
			return false;
		}
		break;
	}
	case NO_RESOURCE: {
		add_status_line(404,commen::error_404_title);
		add_headers(strlen(commen::error_404_form));
		if (!add_content(commen::error_404_form)) {
			return false;
		}
		break;
	}
	case FORBIDDEN_REQUEST: {
		add_status_line(400, commen::error_403_title);
		add_headers(strlen(commen::error_403_form));
		if (!add_content(commen::error_403_form)) {
			return false;
		}
		break;
	}
	case FILE_REQUEST: {
		add_status_line(200,commen::ok_200_title);
		if (m_file_stat.st_size != 0) {
			add_headers(m_file_stat.st_size);
			m_lv[0].iov_base = m_write_buf;
			m_lv[0].iov_len = m_write_idx;
			m_lv[1].iov_base = m_file_address;
			m_lv[1].iov_len = m_file_stat.st_size;
			m_iv_count = 2;
			return true;

		}
		else {
			const char* ok_string = "<html><body><h1>hello</h1></body></html>";
			add_headers(strlen(ok_string));
			if (!add_content(ok_string))
			{
				return false;
			}
		}

	}
	default: {
		return false;
	}
	}
	m_lv[0].iov_base = m_write_buf;
	m_lv[0].iov_len = m_write_idx;
	m_iv_count = 1;
	return true;

}



void http_conn::unmap()
{
	if (m_file_address) {
		munmap(m_file_address, m_file_stat.st_size);
		m_file_address = 0;
	}
}

bool http_conn::add_response(const char* format, ...)
{
	if (m_write_idx >= WRITE_BUFFER_SIZE) {
		return false;
	}
	va_list arg_list;
	va_start(arg_list, format);
	size_t len = vsnprintf(m_write_buf + m_write_idx, WRITE_BUFFER_SIZE - 1 - m_write_idx, format, arg_list);
	if (len >= (WRITE_BUFFER_SIZE - 1 - m_write_idx)) {
		return false;
	}
	m_write_idx += len;
	va_end(arg_list);
	return true;
}
bool http_conn::add_content(const char* content)
{
	return add_response("%s", content);
}

bool http_conn::add_status_line(int status, const char* title)
{
	return add_response("%s %d %s\r\n", "HTTP/1.1", status, title);
}

bool http_conn::add_content_length(int content_length)
{
	return add_response("Content-Length: %d\r\n", content_length);
}

bool http_conn::add_linger()
{
	return add_response("Connection: %s\r\n", (m_link == true) ? "keep-alive" : "close");
}

bool http_conn::add_content_type(const char* type)
{
	return add_response("Content-Type: %s;charset=utf-8\r\n", type);
}

bool http_conn::add_blank_line()
{
	return add_response("%s", "\r\n");
}

bool http_conn::add_headers(int content_len)
{
	add_content_length(content_len);
	//add_content_type("application/json");
	add_linger();
	add_blank_line();
}
bool http_conn::read()
{
	if (m_read_idx > READ_BUFFER_SIZE) {
		return false;
	}
	int bytes_read = 0;
	while (true) {
		bytes_read = recv(m_socked, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
		if (bytes_read == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break;
			}
			return false;
		}
		else if (bytes_read == 0) {
			return false;
		}
		m_read_idx += bytes_read;
	}
	return true;
}
bool http_conn::write()
{
	int temp = 0;
	int bytes_had_send = 0;
	int bytes_to_send = 0;
	int newadd = 0;
	for (int i = 0; i < m_iv_count; i++) {

		bytes_to_send += int(m_lv[i].iov_len);

	}

	while (1)
	{
		if (bytes_to_send <= 0) {
			modfd(m_epolled, m_socked, EPOLLIN);
			init_state();
			return true;
		}
		temp = writev(m_socked, m_lv, m_iv_count);

		if (temp > 0)
		{
			bytes_had_send += temp;
			newadd = bytes_had_send - m_write_idx;
			bytes_to_send -= temp;
			if (bytes_to_send >= m_lv[0].iov_len)
			{
				m_lv[0].iov_len = 0;
				m_lv[1].iov_base = m_file_address + newadd;
				m_lv[1].iov_len = bytes_to_send;
			}
			else
			{
				m_lv[0].iov_base = m_write_buf + bytes_to_send;
				m_lv[0].iov_len = m_lv[0].iov_len - bytes_to_send;

			}
		}
		else {
			if (errno == EAGAIN)
			{
				modfd(m_epolled, m_socked, EPOLLOUT);
				return true;
			}
			unmap();
			return false;
		}
	}
}

http_conn::LINE_STATUS http_conn::parse_line()
{
	char temp;//表示当前字节
	for (; m_checked_idx < m_read_idx; ++m_checked_idx) {
		temp = m_read_buf[m_checked_idx];
		if (temp == '\r') {
			if ((m_checked_idx + 1) == m_read_idx) {
				return LINE_OPEN;
			}
			else if (m_read_buf[m_checked_idx + 1] == '\n') {
				m_read_buf[m_checked_idx++] = '\0';
				m_read_buf[m_checked_idx++] = '\0';
				return LINE_OK;
			}
			return LINE_BAD;
		}
		else if (temp == '\n') {
			if ((m_checked_idx > 1) && (m_read_buf[m_checked_idx - 1] == '\r')) {
				m_read_buf[m_checked_idx - 1] = '\0';
				m_read_buf[m_checked_idx++] = '\0';
				return LINE_OK;
			}
			return LINE_BAD;
		}
	}
	return LINE_OPEN;
}
http_conn::RESULT_CODE http_conn::parse_request_line(char* text) {

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
	m_arg = strpbrk(m_url, "?");
	if(m_arg) {
		*m_arg++ = '\0';
		parse_args(m_arg); //去除？部分
		
	}else{
		std::cout<<m_url<<" :没有参数传递"<<std::endl;
	}
	m_check_state = CHEACK_STATE_HEADER;
	
	return NO_REQUEST;
}
http_conn::RESULT_CODE http_conn::parse_args(char *text) {
	char * m_idx = text;
	char * m_odx = text;

	while(m_idx){
		
		text = strpbrk(text,"&");
		if(text)*text++ = '\0';
		if(m_idx) {
			m_odx = strpbrk(m_idx,"=");
			*m_odx++ ='\0';
			m_args[m_idx]=m_odx;
		std::cout<<m_idx<<":"<<m_odx<<std::endl;
		}	
		m_idx = text;
	}
	return NO_REQUEST;
}

http_conn::RESULT_CODE http_conn::parse_headers(char* text)
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
			m_link = false;
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
		m_accept_types.push_back(text);
	}
	else if (strncasecmp(text, "Accept-Language:", 16) == 0) {
		text += 16;
		text += strspn(text, " ");
		m_accept_language.push_back(text);
	}
	else {
		printf("oop! unknow header %s\n", text);

	}
	return NO_REQUEST;
}

http_conn::RESULT_CODE http_conn::parse_content(char* text)
{
	if (m_read_idx >= (m_content_length + m_checked_idx)) {
		text[m_content_length] = '\0';
		return GET_REQUEST;
	}
	return NO_REQUEST;

}

http_conn::RESULT_CODE http_conn::do_request()
{
	strcpy(m_real_file,commen::doc_root);
	size_t len = strlen(commen::doc_root);

	strncpy(m_real_file + len, m_url, FILENAME_LEN - len - 1);
	printf("m_real_file %s \n", m_real_file);
	if (stat(m_real_file, &m_file_stat) < 0) {
		return NO_RESOURCE;
	}
	if (!(m_file_stat.st_mode & S_IROTH)) {
		return  FORBIDDEN_REQUEST;
	}
	if (S_ISDIR(m_file_stat.st_mode)) {
		return BAD_REQUEST;
	}
	int fd = open(m_real_file, O_RDONLY);
	m_file_address = (char*)mmap(0, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	return FILE_REQUEST;
}


http_conn::http_conn() {

}

http_conn::~http_conn()
{

}
