#include "hw_check.h"
#include <regex>
#include <algorithm>

using namespace std;

Constraint get_constraint(const args_analyse_tools* args)
{
	string action = args[OPT_ARGS_ACTION].get_string();
	string cno = args[OPT_ARGS_CNO].get_string();
	string stu = args[OPT_ARGS_STU].get_string();
	string file = args[OPT_ARGS_FILE].get_string();
	int chap_num = args[OPT_ARGS_CHAPTER].get_int();
	int week_num = args[OPT_ARGS_WEEK].get_int();
	string chapter, week;
	if (chap_num == -1)
		chapter = "all";
	else
		chapter = to_string(chap_num);
	if (week_num == -1)
		week = "all";
	else
		week = to_string(week_num);

	Constraint constraint;
	stringstream ss(action);
	ss >> constraint.action;
	constraint.cno = cno;
	constraint.stu = stu;
	constraint.file = file;
	constraint.chapter = chapter;
	constraint.week = week;

	return constraint;
}

void args_distribute(const args_analyse_tools* args)
{
	Config config(args[OPT_ARGS_CFGFILE].get_string());

	Constraint constraint = get_constraint(args);

	DataManager(constraint, config);
}

string get_exten(const string& filename) 
{
	size_t dotPos = filename.find_last_of('.');
	if (dotPos != string::npos && dotPos < filename.length() - 1) 
		return filename.substr(dotPos + 1);
	else
		return "";
}

vector<int> extractInt(const string& input) {
	// 正则表达式模式，用于匹配整数
	regex integerPattern("\\d+");

	// 存储提取到的整数的 vector
	vector<int> integers;

	// 迭代器对字符串进行匹配
	auto words_begin = sregex_iterator(input.begin(), input.end(), integerPattern);
	auto words_end = sregex_iterator();

	// 遍历匹配到的整数并存储到 vector 中
	for (sregex_iterator i = words_begin; i != words_end; ++i) 
	{
		smatch match = *i;
		string no = match.str();
		if (no.length() == 7 && no[2] == '5')
			integers.push_back(stoi(no));
	}

	return integers;
}

bool intersect(const vector<int>& vector1, const vector<int>& vector2) 
{
	// 先将两个向量排序
	vector<int> sortedVector1 = vector1;
	vector<int> sortedVector2 = vector2;

	sort(sortedVector1.begin(), sortedVector1.end());
	sort(sortedVector2.begin(), sortedVector2.end());

	// 创建一个临时向量用于存储交集
	vector<int> intersection;

	// 使用 std::set_intersection 计算交集
	set_intersection(sortedVector1.begin(), sortedVector1.end(),
		sortedVector2.begin(), sortedVector2.end(),
		back_inserter(intersection));

	// 检查交集的大小是否大于 0
	return !intersection.empty();
}