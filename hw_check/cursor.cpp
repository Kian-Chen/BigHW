#include "hw_check.h"
using namespace std;

Cursor::Cursor(const Connection connection)
{
	this->connection = connection;
    init();
}

Cursor::~Cursor()
{
    mysql_close(mysql);
}

void Cursor::init()
{
    /* 初始化 mysql 变量，失败返回NULL */
    if ((mysql = mysql_init(NULL)) == NULL) {
        cout << "mysql_init failed" << endl;
        return;
    }

    /* 连接数据库，失败返回NULL
        1、mysqld没运行
        2、没有指定名称的数据库存在 */
    if (mysql_real_connect(mysql, connection.host.c_str(), connection.username.c_str(), 
        connection.passwd.c_str(), connection.dbname.c_str(), 0, NULL, 0) == NULL) {
        cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        return;
    }

    /* 设置字符集，否则读出的字符乱码 */
    mysql_set_character_set(mysql, "gbk");
}

Connection Cursor::get_connection()
{
	return connection;
}

vector<Student> Cursor::get_stu(string course, string student)
{
    string sql = "select stu_no, stu_name, stu_class_fname, stu_class_sname ";
    sql += "from " + stulist + " ";
    sql += " where stu_cno = " + course + " ";
    if (student == "all")
        sql += "AND stu_no IN (SELECT DISTINCT stu_no FROM " + stulist + ") ";
    else
        sql += "AND stu_no = " + student + " ";

    vector<Student> students;

    if (mysql_query(mysql, sql.c_str())) {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return students;
    }

    /* 将查询结果存储起来，出现错误则返回NULL
        注意：查询结果为NULL，不会返回NULL */
    if ((result = mysql_store_result(mysql)) == NULL) {
        cout << "mysql_store_result failed" << endl;
        return students;
    }


    while ((row = mysql_fetch_row(result)) != NULL) 
    {
        string no = row[0];
        string name = row[1];
        string fname = row[2];
        string sname = row[3];
        Student temp(no, name, fname, sname, course);
        students.push_back(temp);
    }

    /* 释放result，否则会丢失内存 */
    mysql_free_result(result);
    return students;
}

vector<Assignment> Cursor::get_hw(string course, string chapter, string week, string file)
{
    string sql = "SELECT hw_ftype, hw_id, hw_filename ";
    sql += "FROM " + hwlist + " ";
    sql += "WHERE hw_cno = " + course + " ";
    if (chapter == "all")
        sql += "AND hw_chapter IN (SELECT DISTINCT hw_chapter FROM " + hwlist + ") ";
    else
        sql += "AND hw_chapter = " + chapter + " ";
    if (week == "all")
        sql += "AND hw_week IN (SELECT DISTINCT hw_week FROM " + hwlist + ") ";
    else
        sql += "AND hw_week = " + week + " ";
    if (file == "all")
        sql += "AND hw_filename IN (SELECT DISTINCT hw_filename FROM " + hwlist + ") ";
    else
        sql += "AND hw_filename = \"" + file + "\" ";

    vector<Assignment> assignments;

    if (mysql_query(mysql, sql.c_str())) {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return assignments;
    }

    /* 将查询结果存储起来，出现错误则返回NULL
        注意：查询结果为NULL，不会返回NULL */
    if ((result = mysql_store_result(mysql)) == NULL) {
        cout << "mysql_store_result failed" << endl;
        return assignments;
    }


    while ((row = mysql_fetch_row(result)) != NULL) 
    {
        int id = stoi(row[1]);
        string name = row[2];
        Assignment temp(id, name);
        assignments.push_back(temp);
    }

    /* 释放result，否则会丢失内存 */
    mysql_free_result(result);
    return assignments;
}