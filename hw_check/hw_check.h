#pragma once
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <iomanip> 
#include<sstream>
#include "../include/class_aat.h"
#include "../include/class_crt.h"
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql特有

/***************************************************************************
  声明类别：参数设置
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
enum OPT_ARGS
{
    OPT_ARGS_HELP = 0,
    OPT_ARGS_DEBUG,
    OPT_ARGS_ACTION,
    OPT_ARGS_CNO,
    OPT_ARGS_STU,
    OPT_ARGS_FILE,
    OPT_ARGS_CHAPTER,
    OPT_ARGS_WEEK,
    OPT_ARGS_DISPLAY,
    OPT_ARGS_CFGFILE
};

struct  Constraint
{
    string action;
    string cno;
    string stu;
    string file;
    string chapter;
    string week;
};

// 定义错误类型
enum ErrorType {
    Correct = 0,
    NotSubmitted,
    CodeFormatIncorrect,
    ReportNotPDF,
    FirstLineNotComment,
    FirstLineCommentMismatch,
    NumErrorTypes  // 用于计算错误类型的总数
};


class Student;
class Assignment;
class File;
class DataManager;
class Config;
class ErrorCounter;

void args_distribute(const args_analyse_tools* args);
string get_exten(const string& filename);
vector<int> extractInt(const string& input);
bool intersect(const vector<int>& vector1, const vector<int>& vector2);

/***************************************************************************
  声明类别：游标
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

struct Connection
{
    string host;
    string port;
    string dbname;
    string username;
    string passwd;
};

class Cursor
{
protected:
    Connection connection;
    MYSQL* mysql;
    MYSQL_RES* result;
    MYSQL_ROW  row;
    const string hwlist = "view_hwcheck_hwlist";
    const string stulist = "view_hwcheck_stulist";

public:
    Cursor(const Connection connection);
    void init();
    ~Cursor();
    Connection get_connection();
    vector<Student> get_stu(string course, string student = "all");
    vector<Assignment> get_hw(string course, string chapter = "all", string week = "all", string file = "all");
};

/***************************************************************************
  声明类别：配置读取
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
class Config
{
protected:
    Connection connection;
    string source_dir;
    string cfg_name;

public:
    Config(string filename);
    Connection get_connection();
    string get_dir();
    void read_cfg();
};

/***************************************************************************
  声明类别：错误统计
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

// 数据结构存储错误信息和类型
struct ErrorInfo {
    ErrorType type;
    string message;
};

// 错误统计器
class ErrorCounter {
protected:
    static const int NumErrorTypes = 7;  // 错误类型总数
    int errorCounts[NumErrorTypes];      // 错误类型计数数组

public:
    ErrorCounter()
    {
        for (int i = 0; i < NumErrorTypes; i++)
            errorCounts[i] = 0;
    }
    friend class DataManager;
};

/***************************************************************************
  声明类别：数据存储
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/


// Assignment 独立的作业信息结构
class Assignment {
protected:
    int assignmentID;
    string hwname;

public:
    Assignment(int id, string hwname);
    Assignment(const Assignment& other);
    int get_id();
    string get_filename();
    void print();
    friend class File;
    friend class DataManager;
};

class Student
{
protected:
    string no;
    string name;
    string fclass;
    string sclass;
    string course;
    int studentID;
public:
    Student(string no, string name, string fclass, string sclass, string course);
    Student(const Student& other);
    int get_id();
    void print();
    friend class File;
    friend class DataManager;
};

class File: public Student,Assignment
{
protected:
    long long int fileID;
    string filename;
    ErrorInfo error;
    vector<int> checkmate;
    vector<string> pairans;

public:
    File(string name, Student stu, Assignment assignment);
    long long int hash(int stu_id, int assign_id);
    bool exist();
    bool is_ansi();
    bool is_pdf();
    ErrorInfo firstline();
    void get_mate();
    void check(string action);
    void check_pair(vector<File> files);
    void print_error() const;
    void print_mateno() const;
    void print_pair_ans() const;

    friend class DataManager;
};


// DataManager 数据管理类
class DataManager {
protected:
    vector<Student> students;
    vector<File> files;
    vector<Assignment> assignments;
    string root_dir;
    string course;
    string action;

public:
    DataManager(Constraint constraint, Config config);
    void generate_files(string action);
    File get_file(int stu_id, string assign_name) const;
    void printByStudentID() const;
    void printByAssignmentID(string assign_name) const;
    void print_cross(string assign_name) const;
    void count(vector<ErrorInfo> errors) const;
};

