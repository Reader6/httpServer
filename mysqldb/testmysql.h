#ifndef TESTMYSQL_H
#define TESTMYSQL_H
#include <mysql/mysql.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "json.hpp"
#include "mysql_guard.h"

using namespace std;
using json_t = nlohmann::json;

///数据库连接器类,负责连接数据库,并封装基本的数据库相关操作
class DB_Connector {
protected:

public:
    inline DB_Connector();
    inline virtual ~DB_Connector();
    //创建当前必须的表
    inline void createTable(MYSQL *conn);
    //执行sql语句,成功则返回0,失败返回非0
    inline int query(MYSQL *conn,const char*sql);
    //返回上一次的错误提示
    inline string error(MYSQL *conn);
    //获取结果集,将其保存到二维string数组中
    inline vector<vector<string> > getResult(MYSQL *conn);

    //这个函数应该属于全局的还就只是这个类的
    bool executeSql(MYSQL* conn, unordered_map<string, string> myMap);

    /*1.用户id；2.用户名；3.用户密码
     * 4.用户信息；5:用户电话；6:用户邮箱
     * 7:用户头像；8:用户上一次登录的时间
     * 对User_info表进行插入操作
     * */
    //对User_info表进行操作
    bool InsertUser_info(MYSQL *conn, string user_id, string user_name, string user_pwd,
                  string user_info="", string user_phone="", string user_emil="",
                  string user_header_img="", string last_login_time="");
    //User_tb用来记录用户登录的信息
    bool InsertUser_tb(MYSQL *conn, string user_id, string user_name, string user_pwd);
    //User_login_tb用来记录第三方登录信息
    bool InsertUser_login_tb(MYSQL *conn, string user_id, string identifier_type, string credential,
                             string identifier);
    //Book_tb作品信息表
    bool InsertTable_tb(MYSQL *conn, string book_id, string book_name, string author_id,
                        string book_info, string book_img, string book_word_number,
                        string book_recommend_number);
    //Chapter_tb章节信息表
    bool InsertChapter_tb(MYSQL *conn, string book_id, string chapter_id, string chapter_name,
                          string chapter_updata_time);
    //Author_info_tb作者信息
    bool InsertAuthor_info_tb(MYSQL *conn, string author_id, string author_header_img,
                              string author_fans_nums, string author_work,
                              string author_info, string author_birthday);
    //Author_tb用户姓名，分表，所以导致表内内容较少，提高性能
    bool InsertAuthor_tb(MYSQL *conn, string author_id, string author_name);
    //Book_mark_tb书签表
    bool InsertBook_mark_tb(MYSQL *conn, string book_id, string chapter_id, string mark_id,
                            string mark_content, string mark_time);

    //Note_tb评论表
    bool InsertNote_tb(MYSQL *conn, string book_id, string chapter_id, string paragraph_id,
                       string note_id);
    //Note_info_tb评论信息表
    bool InsertNote_info_tb(MYSQL *conn, string note_id, string note_content,
                            string note_time, string note_author);

    //验证登录信息
    bool Select_login(MYSQL *conn, unordered_map<string, string> myMap);
    //查找不同类型的书籍
    bool Select_ClassifyBook(MYSQL *conn, unordered_map<string, string> myMap);
private:
    bool _Insert(MYSQL *conn,string TableName,string columnName,string Value);//这个应该用不到
    bool _Update(MYSQL *conn,string TableName,string Constraint, string ConstraintValue,
                 string columnName,string newValue);//修改表中的数据。外边传进来的数据，我里面一条一条的修改
    json_t ToJson(char *ColumnName, vector<vector<string>> ColumnValue);
    bool _Select(MYSQL *conn, char *TableName, char *ColumnName,
                char* ConstraintColumnName="", char* ConstraintValue="",
                char *ConstraintFromula="=",char *Distinct="");
    //这个用户需要回返值用来验证
    bool _Select(vector<json_t> &j, MYSQL *conn, char *TableName, char *ColumnName,
                char* ConstraintColumnName="", char* ConstraintValue="",
                char *ConstraintFromula="=",char *Distinct="");
public:
    //用于用户修改自身信息,这个是针对User_info表的格式来写的
    bool Update(MYSQL *conn,
                string TableName,
                string Constraint,
                string ConstraintValue,
                string user_name="", string user_pwd="",
                string user_info="", string user_phone="", string user_emil="",
                string user_header_img="");
    //get那里数据怎么解析
    bool Update(MYSQL *conn, string TableName, string Constraint, string ConstraintValue,
                string updateValue);
    bool Delete(MYSQL *conn,string TableName, string columnName, string columnVaule);//删除某一列
    bool Delete(MYSQL *conn,string TableName);//删除全部数据，就是删除一张表
};

DB_Connector::DB_Connector()
{

    //conn = new mysql_guard();
}

DB_Connector::~DB_Connector()
{
    //delete conn;
    cout << "关闭数据库" << endl;
}
//创建默认表格
void DB_Connector::createTable(MYSQL *conn)
{
    const char *pString = "show tables like 'User_info'";
    if (0 != query(conn, pString)){
        printf("query failed: %s\n", mysql_error(conn));
        delete conn;
        //mysql_library_end();
        return;
    }

    auto res = getResult(conn);
    if (res.size() > 0 && res[0][0] == "User_info") {
        cout << "User_info alreader exist" << endl;
        return;
    }
    //如果该表不存在，那么就创建它
    const char * sql = "CREATE TABLE User_info ("
                       "user_id CHAR(11) NOT NULL PRIMARY KEY,"
                       "user_name varchar(50) NOT NULL,"
                       "user_pwd varchar(50) NOT NULL,"
                       "user_info varchar(50),"
                       "user_phone varchar(50),"
                       "user_emil varchar(50),"
                       "user_header_img varchar(50),"
                       "last_login_time TIMESTAMP"
                       ") ENGINE=InnoDB DEFAULT CHARSET=utf8;";

    if (0 != query(conn, sql)){
        printf("query failed: %s\n", mysql_error(conn));

        //mysql_library_end();
        return;
    }else {
        cout << "创建表成功" << endl;
    }
}

//执行sql语句,成功则返回0,失败返回非0
int DB_Connector::query(MYSQL *conn,const char*sql)
{
    int length = strlen(sql);
    return mysql_real_query(conn, sql, length);
}

//返回上一次的错误提示
string DB_Connector::error(MYSQL *conn)
{
    return mysql_error(conn);
}

//获取结果集,将其保存到二维string数组中
vector<vector<string> > DB_Connector::getResult(MYSQL *conn)
{
    vector<vector<string> >result;
    MYSQL_RES* res = mysql_store_result(conn);
    if (res == NULL)
        return result;
    unsigned num = mysql_num_fields(res); //获取结果集的列数，每一个元组
    cout << "num is " << num << endl;
    MYSQL_ROW row;
    //mysql_fetch_row获得每一行，执行后返回一个二级指针，此函数执行完后会返回下一个字符串数组的地址，如果没有那么就返回null
    for (; (row = mysql_fetch_row(res)) != NULL;) {
        result.push_back(vector<string>(num));//循环一次压入一行，每次都在末尾操作
        for (size_t cur = result.size() - 1, i = 0; i < num; ++i) {
            //cout << "cur is " << cur << endl;
            result[cur][i] = row[i];
            //数据库中如果没有值，默认为空字符串""
            //cout << row[i] << endl;
        }
    }
    mysql_free_result(res); //释放结果集资源
    return result;
}

class testMySQL
{
public:
    testMySQL();
};

#endif // TESTMYSQL_H
