#include "testmysql.h"


testMySQL::testMySQL()
{

}

bool DB_Connector::InsertUser_info(MYSQL *conn, string user_id, string user_name, string user_pwd,
                                   string user_info, string user_phone,
                                   string user_emil, string user_header_img,
                                   string last_login_time)
{
    //user_id, user_name, user_pwd is not NULL,这个可以直接先插入
    string sql = "insert into User_info(user_id, user_name, "
                 "user_pwd)values(\""
            +user_id+"\",\""
            +user_name+"\",\""
            +user_pwd+"\");";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }

    if ("" != user_info)
    {
        //update User_info set user_info="*****" where user+id = 1;
        _Update(conn, "User_info","user_id",user_id, "user_info", user_info);
    }
    if("" != user_phone){
        _Update(conn, "User_info","user_id",user_id, "user_phone", user_phone);
    }
    if ("" != user_emil) {
        _Update(conn, "User_info","user_id",user_id, "user_emil", user_emil);
    }
    if ("" != user_header_img) {
        _Update(conn, "User_info","user_id",user_id, "user_header_img", user_header_img);
    }

    return true;
}

bool DB_Connector::InsertUser_tb(MYSQL *conn, string user_id, string user_name, string user_pwd)
{
    string sql = "insert into User_tb(user_id, user_name, "
                 "user_pwd)values(\""
            +user_id+"\",\""
            +user_name+"\",\""
            +user_pwd+"\");";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    return true;
}

bool DB_Connector::InsertUser_login_tb(MYSQL *conn, string user_id, 
                                       string identifier_type, string credential, string identifier)
{
    string sql = "insert into User_login_tb(user_id, identifier_type, credential"
                 "identifier)values(\""
            +user_id+"\",\""
            +identifier_type+"\",\""
            +credential+"\",\""
            +identifier+"\");";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    return true;
}

bool DB_Connector::InsertTable_tb(MYSQL *conn, string book_id, string book_name, string author_id, string book_info, string book_img, string book_word_number, string book_recommend_number)
{
    string sql = "insert into Table_tb(book_id, book_name, "
                 "author_id)values(\""
            +book_id+"\",\""
            +book_name+"\",\""
            +author_id+"\");";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    if ("" != book_info)
    {
        _Update(conn, "Table_tb","book_id",book_id, "book_info", book_info);
    }
    if ("" != book_img)
    {
        _Update(conn, "Table_tb","book_id",book_id, "book_img", book_img);
    }
    if ("" != book_word_number)
    {
        _Update(conn, "Table_tb","book_id",book_id, "book_word_number", book_word_number);
    }
    if ("" != book_recommend_number)
    {
        _Update(conn, "Table_tb","book_id",book_id, "book_recommend_number", book_recommend_number);
    }
    return true;
}

bool DB_Connector::InsertChapter_tb(MYSQL *conn, string book_id, string chapter_id, string chapter_name, string chapter_updata_time)
{
    string sql = "insert into Chapter_tb(book_id, chapter_id, "
                 "chapter_name)values(\""
            +book_id+"\",\""
            +chapter_id+"\",\""
            +chapter_name+"\");";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    if ("" != chapter_updata_time)
    {
        _Update(conn, "Chapter_tb","book_id",book_id, "chapter_updata_time", chapter_updata_time);
    }
    return true;
}

bool DB_Connector::InsertAuthor_info_tb(MYSQL *conn, string author_id,
                                        string author_header_img, string author_fans_nums,
                                        string author_work, string author_info,
                                        string author_birthday)
{
    string sql = "insert into Author_info_tb(author_id)values(\""
            +author_id+"\");";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    if ("" != author_header_img)
    {
        _Update(conn, "Author_info_tb","author_id",author_id, "author_header_img", author_header_img);
    }
    if ("" != author_fans_nums)
    {
        _Update(conn, "Author_info_tb","author_id",author_id, "author_fans_nums", author_fans_nums);
    }
    if ("" != author_work)
    {
        _Update(conn, "Author_info_tb","author_id",author_id, "author_work", author_work);
    }
    if ("" != author_info)
    {
        _Update(conn, "Author_info_tb","author_id",author_id, "author_info", author_info);
    }
    if ("" != author_birthday)
    {
        _Update(conn, "Author_info_tb","author_id",author_id, "author_birthday", author_birthday);
    }
    return true;
}

bool DB_Connector::InsertAuthor_tb(MYSQL *conn, string author_id, string author_name)
{
    string sql = "insert into Author_tb(author_id, author_name)values(\"" + author_id +
            "\",\"" + author_name + "\");";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    return true;
}

bool DB_Connector::InsertBook_mark_tb(MYSQL *conn, string book_id, string chapter_id, string mark_id, string mark_content, string mark_time)
{
    string sql = "insert into Book_mark_tb(book_id, chapter_id)values(\"" + book_id +
            "\",\"" + chapter_id + "\");";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    if ("" != mark_id)
    {
        _Update(conn, "Book_mark_tb","chapter_id",chapter_id, "mark_id", mark_id);
    }
    if ("" != mark_content)
    {
        _Update(conn, "Book_mark_tb","chapter_id",chapter_id, "mark_content", mark_content);
    }
    if ("" != mark_time)
    {
        _Update(conn, "Book_mark_tb","chapter_id",chapter_id, "mark_time", mark_time);
    }
    return true;
}

bool DB_Connector::InsertNote_tb(MYSQL *conn, string book_id, string chapter_id, string paragraph_id, string note_id)
{
    string sql = "insert into Note_tb(book_id, chapter_id)values(\"" + book_id +
            "\",\"" + chapter_id + "\");";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    if ("" != paragraph_id)
    {
        _Update(conn, "Note_tb","chapter_id",chapter_id, "paragraph_id", paragraph_id);
    }
    if ("" != note_id)
    {
        _Update(conn, "Note_tb","chapter_id",chapter_id, "note_id", note_id);
    }
    return true;
}

bool DB_Connector::InsertNote_info_tb(MYSQL *conn, string note_id, string note_content,
                                      string note_time, string note_author)
{
    string sql = "insert into Note_info_tb(note_id)values(\"" + note_id + "\");";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    if ("" != note_content)
    {
        _Update(conn, "Note_info_tb","note_id",note_id, "note_content", note_content);
    }
    if ("" != note_time)//这个时间应该是系统默认的时间
    {
        _Update(conn, "Note_info_tb","note_id",note_id, "note_time", note_time);
    }
    if ("" != note_author)
    {
        _Update(conn, "Note_info_tb","note_id",note_id, "note_author", note_author);
    }
    return true;
}

bool DB_Connector::Select(MYSQL *conn, char *TableName, char *ColumnName,
                          char* ConstraintColumnName, char* ConstraintValue,char *ConstraintFromula,
                          char *Distinct)
{
    char temp[128];
    if (strcmp("", Distinct))//是否去冗余
    {
        sprintf(temp, "select %s", Distinct);
    }else{
        strcpy(temp, "select");
    }
    if (strcmp("", ConstraintValue))//是否有限制性约束
    {
        if (ConstraintValue[0] >= '0' && ConstraintValue[0] <= '9')
        {
            sprintf(temp, "%s %s from %s where %s %s %s",temp, ColumnName,
                    TableName, ConstraintColumnName,ConstraintFromula,ConstraintValue);
        }
        else
        {
            sprintf(temp, "%s %s from %s where %s %s \'%s\'",temp, ColumnName,
                    TableName, ConstraintColumnName,ConstraintFromula,ConstraintValue);
        }

    }else{
        sprintf(temp, "%s %s from %s",temp, ColumnName, TableName);
    }


    cout << "插入的sql语句为" << temp << endl;
    if (query(conn, temp) != 0){
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    //进行了值拷贝，数据的复制
    auto getValue = getResult(conn);

    //进行JSON的组装,ColumnName是每一列的名字
    ToJson(ColumnName, getValue);

}

bool DB_Connector::_Update(MYSQL *conn,string TableName,string Constraint,
                           string ConstraintValue,
                           string columnName, string newValue)
{
    string sql = "update "+TableName+" set "+ columnName+"="+"\""+newValue+"\""
            +" where "+Constraint+"="+"\""+ConstraintValue+"\"";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    return true;
}

void DB_Connector::ToJson(char *ColumnName, vector<vector<string> > ColumnValue)
{
    //对字段名进行拆分
    char *singleColumnName;
    vector<string> s;
    ofstream ofile;
    ofile.open("/root/js");
    singleColumnName = strsep(&ColumnName, ",");
    while (NULL != singleColumnName)
    {
        //去掉空格
        singleColumnName += strspn(singleColumnName, " ");
        s.push_back(singleColumnName);
        //printf("拆分后的字符串为:%s\n", p);
        singleColumnName = strsep(&ColumnName, ",");
    }

    //进行json组合
    json_t j;
    vector<string>::iterator iter = s.begin();
    size_t cur = ColumnValue.size() - 1;
    for (size_t i = 0 ; i <= cur; i++)
    {
        for (int k = 0; iter != s.end();k++, iter++)
        {
           j[*iter] = ColumnValue[i][k];
        }
        ofile << j.dump(2) << endl;;
        cout << "json解析后为：" <<j.dump(2) << endl;
        //遍历第二次
        iter = s.begin();
    }
    ofile.close();

//    vector<string>::iterator iter = s.begin();
//    size_t cur = ColumnValue.size() - 1;
//    for (size_t i = 0 ; i <= cur; i++)
//    {
//        for (int j = 0; iter != s.end(); j++, iter++)
//        {
//            if (iter == s.end() - 1)
//            {//某一列值的末尾
//                ww += *iter+"\":\""+ColumnValue[i][j]+"\"";
//            }
//            else
//            {
//                ww += *iter+"\":\""+ColumnValue[i][j]+"\",\"";
//            }
//        }

//        if (i == cur)
//        {//json末尾
//            ww += "}]";
//        }else{
//            ww += "},";
//            ww += "{\"";
//        }
//        //遍历第二次
//        iter = s.begin();
//    }
    //cout << "ww is " << ww << endl;
    cout << "值的个数为：" << cur << endl;
}

bool DB_Connector::Update(MYSQL *conn,
                          string TableName,
                          string Constraint,
                          string ConstraintValue,
                          string user_name,
                          string user_pwd,
                          string user_info,
                          string user_phone,
                          string user_emil,
                          string user_header_img)
{
    if ("" != user_name)
    {
        _Update(conn, TableName, Constraint, ConstraintValue, "user_name", user_name);
    }
    if("" != user_pwd){
        _Update(conn, TableName, Constraint, ConstraintValue, "user_pwd", user_pwd);
    }
    if("" != user_info){
        _Update(conn, TableName, Constraint, ConstraintValue, "user_info", user_info);
    }
    if("" != user_phone){
        _Update(conn, TableName, Constraint, ConstraintValue, "user_phone", user_phone);
    }
    if("" != user_emil){
        _Update(conn, TableName, Constraint, ConstraintValue, "user_emil", user_emil);
    }
    if("" != user_header_img){
        _Update(conn, TableName, Constraint, ConstraintValue, "user_header_img", user_header_img);
    }
}

bool DB_Connector::Delete(MYSQL *conn,string TableName, string columnName, string columnVaule)
{
    string sql = "delete from " + TableName + " where " + columnName + "=\""+columnVaule+"\"";
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    return true;
}
//没测试运行过
bool DB_Connector::Delete(MYSQL *conn,string TableName)
{
    //是在里面拿，还是在外边传进来
    mysql_guard guard;
    string sql = "delete from " + TableName;
    cout << "插入的sql语句为" << sql << endl;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0
        printf("query failed: %s\n", mysql_error(conn));
        return false;
    }
    return true;
}

bool DB_Connector::_Insert(MYSQL *conn,string TableName, string columnName,string Value)
{
    string sql = "insert into "+TableName+"("+ columnName+")values("+"\""+Value+"\""+");";
    cout << "插入的sql语句为" << sql << endl;

    mysql_guard guard;
    if (query(conn, sql.c_str()) != 0) { //执行sql语句失败返回非0

        printf("query failed: %s\n", mysql_error(conn));

    }
    cout << "Insert sql is " << sql << endl;
    return true;
}


