#include "hw_check.h"

/***************************************************************************
  类别名称：Assignment
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Assignment::Assignment(int id, string hwname)
{
    this->assignmentID = id;
    this->hwname = hwname;
}

void Assignment::print()
{
    cout << "ID : " << setw(6) << assignmentID << "  ";
    cout << "作业名 : " << setw(28) << hwname << endl;
}

int Assignment::get_id()
{
    return this->assignmentID;
}

string Assignment::get_filename()
{
    return this->hwname;
}

Assignment::Assignment(const Assignment& other)
{
    this->assignmentID = other.assignmentID;
    this->hwname = other.hwname;
}

/***************************************************************************
  类别名称：Student
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Student::Student(string no, string name, string fclass, string sclass, string course)
{
    this->no = no;
    this->name = name;
    this->fclass = fclass;
    this->sclass = sclass;
    this->course = course;
    studentID = stoi(no);
}

void Student::print()
{
    cout << "学号 : " << setw(8) << no << " ";
    cout << "姓名 : " << setw(8) << name << " ";
    cout << "专业 : " << setw(6) << sclass << " ";
    cout << "课号 : " << setw(10) << course << " ";
    cout << "ID : " << setw(8) << studentID << endl;
}

int Student::get_id()
{
    return this->studentID;
}

Student::Student(const Student& other)
{
    this->no = other.no;
    this->name = other.name;
    this->fclass = other.fclass;
    this->sclass = other.sclass;
    this->course = other.course;
    this->studentID = other.studentID;
}