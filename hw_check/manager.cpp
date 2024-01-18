#include "hw_check.h"

DataManager::DataManager(Constraint constraint, Config config)
{
	Connection connection = config.get_connection();
	root_dir = config.get_dir();
	course = constraint.cno;
	Cursor cursor(connection);

	students = cursor.get_stu(constraint.cno, constraint.stu);
	assignments = cursor.get_hw(constraint.cno, constraint.chapter, constraint.week, constraint.file);

	this->action = constraint.action;
	generate_files(constraint.action);

	if (constraint.stu == "all" && constraint.file != "all")
		this->printByAssignmentID(constraint.file);
	else
		this->printByStudentID();

	if (action == "secondline")
		this->print_cross(constraint.file);
}

void DataManager::generate_files(string action)
{
	for (Assignment& assignment : assignments)
	{
		for (Student& student : students)
		{
			int stu_id = student.get_id();
			int assign_id = assignment.get_id();
			string filename = this->root_dir;
			filename += course + "-" + to_string(student.get_id());
			filename+= "\\" + assignment.get_filename();

			File file(filename, student, assignment);
			if (action == "secondline")
				file.get_mate();
			file.check(action);
			if (action == "secondline")
				file.check_pair(this->files);
			files.push_back(file);
		}
	}
}

File DataManager::get_file(int stu_id, string assign_name) const
{
	//File temp;
	for (auto& file : files)
	{
		if (file.studentID == stu_id && file.hwname == assign_name)
			return file;
	}
}

void DataManager::printByStudentID() const
{
	vector<ErrorInfo> all_error;
	for (int i = 0; i < students.size(); i++)
	{
		"1  : 学号-1850772 姓名-张哲源 课号-10108001 文件数量-54";//样板
		cout << setw(3) << setiosflags(ios::left) << i + 1 << ": ";
		cout << "学号-" << students[i].studentID << " ";
		cout << "姓名-" << setw(8) << setiosflags(ios::left) << students[i].name << " ";
		cout << "课号-" << course << "  ";

		vector<File> personal;
		vector<ErrorInfo> errors;
		for (auto& file : files)
		{
			if (file.studentID == students[i].studentID)
			{
				personal.push_back(file);
				errors.push_back(file.error);
				all_error.push_back(file.error);
			}
		}
		cout << "文件数量-" << personal.size() << endl;

		for (auto file : personal)
		{
			"  12-b1-1.cpp                 : 首行班级不匹配  [/* 1850772 张哲源 计科2班 */]";//样板
			cout << " " << setw(30) << setiosflags(ios::left) << file.hwname << " : " << file.error.message << endl;
		}
		cout << endl;
		count(errors);
	}

	"共完成90个学生的检查，文件总数:4860";//样板
	cout << "共完成" << students.size() << "个学生的检查，文件总数:" << files.size() << endl;
	count(all_error);
}

void DataManager::printByAssignmentID(string assign_name) const
{
	cout << "课号 : " << this->course << " 学生数量 : " << students.size() << " 源文件名 : " << this->assignments[0].hwname << endl;

	vector<ErrorInfo> errors;
	for (int i = 0; i < students.size(); i++)
	{
		"1  : 1850772/张哲源   : 正确";//样板
		cout << setw(3) << setiosflags(ios::left) << i + 1 << " : ";
		cout << students[i].studentID << "/" << setw(14) << setiosflags(ios::left) << students[i].name;
		cout << ":  " << this->get_file(students[i].studentID, assign_name).error.message << endl;
		errors.push_back(this->get_file(students[i].studentID, assign_name).error);
	}
	cout << endl;
	count(errors);
}

void DataManager::count(vector<ErrorInfo> errors) const
{
	"检查通过74/90个，本次通过74个\
		======================================\
		详细信息\
		======================================\
		正确 : 74\
		未提交 : 15\
		源文件格式不正确(非GB编码) : 1\
		======================================";
	ErrorCounter counter;
	int sum = errors.size();

	for (auto& error : errors)
		counter.errorCounts[error.type]++;

	cout << "检查通过" << counter.errorCounts[Correct] << "/" << sum << "个，本次通过" << counter.errorCounts[Correct] << "个" << endl;
	cout << "======================================" << endl;
	cout << "详细信息" << endl;
	cout << "======================================" << endl;
	cout << setw(30) << setiosflags(ios::left) << "正确 " << ": " << counter.errorCounts[Correct] << endl;
	cout << setw(30) << setiosflags(ios::left) << "未提交 " << ": " << counter.errorCounts[NotSubmitted] << endl;
	cout << setw(30) << setiosflags(ios::left) << "源文件格式不正确(非GB编码) " << ": " << counter.errorCounts[CodeFormatIncorrect] << endl;
	cout << setw(30) << setiosflags(ios::left) << "报告文件不是PDF文件 " << ": " << counter.errorCounts[ReportNotPDF] << endl;
	cout << setw(30) << setiosflags(ios::left) << "首行不是注释 " << ": " << counter.errorCounts[FirstLineNotComment] << endl;
	cout << setw(30) << setiosflags(ios::left) << "首行注释匹配出错 " << ": " << counter.errorCounts[FirstLineCommentMismatch] << endl;

	cout << "======================================" << endl;
	cout << endl << endl;

}

void DataManager::print_cross(string assign_name) const
{
	cout << endl << endl;
	cout << "交叉检查结果：" << endl;
	for (int i = 0; i < students.size(); i++)
	{
		"1  : 1850772 张哲源";//样板
		cout << setw(2) <<  i + 1 << ":";
		cout << students[i].studentID << "/" << setw(14) << setiosflags(ios::left) << students[i].name << endl;
		this->get_file(students[i].studentID, assign_name).print_pair_ans();
	}
}

