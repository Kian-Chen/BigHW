/* 2250951 光电 陈麒安 */ 
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <iostream>
#include <fstream>
#include<string>
#include "../include/class_crt.h"
using namespace std;

/* 允许加入需要的头文件，给出 cfgfile_read_tools类的实现 */ 


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

cfgfile_read_tools::cfgfile_read_tools(const char* _cfgname)
{
	cfgname = string(_cfgname);
}

cfgfile_read_tools::cfgfile_read_tools(const string& _cfgname)
{
	cfgname = _cfgname;
}

cfgfile_read_tools::~cfgfile_read_tools()
{
    groups.clear();
}

/* 判断配置文件是否打开成功 */
bool cfgfile_read_tools::is_open()
{
	ifstream fin(cfgname);
	bool open = fin.is_open();
	fin.close();
	return open;
}

/* 组读取函数，读出所有的，放在string型的vector中 */
int cfgfile_read_tools::get_all_group(vector <string>& ret)
{
    ifstream fin(cfgname);
    if (!fin.is_open())
    {
        return -1;
    }
    groups.clear();
    string line;
    while (getline(fin, line)) 
    {
        // 截断注释
        size_t comment_pos = line.find_first_of(";#");
        if (comment_pos != string::npos) 
        {
            line = line.substr(0, comment_pos);
        }

        // 去除前后空格/tab
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // 检查是否是组名
        if (!line.empty() && line.front() == '[' && line.back() == ']') 
        {
            ret.push_back(line);
        }
    }

    groups = ret;
    fin.close();
    return ret.size();
}

/* 项读取函数 - 取某个group的所有item，放在string型的vector中 */
int cfgfile_read_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool ignore_lower_upper_case )	//测试函数中缺省区分大小写
{
    const char* str = (group_name ? group_name : "");
    return get_all_item(string(str), ret, ignore_lower_upper_case);
}

int cfgfile_read_tools::get_all_item(const string& group_name, vector <string>& ret, const bool ignore_lower_upper_case)
{
    ifstream file(cfgname);
    if (!file.is_open()) 
    {
        return -1; // 文件无法打开
    }
    ret.clear();
    string line;
    bool in_group = group_name.empty(); // 如果group_name为空，则读取所有项
    while (getline(file, line)) 
    {
        // 截断注释
        size_t comment_pos = line.find_first_of(";#");
        if (comment_pos != string::npos) 
        {
            line = line.substr(0, comment_pos);
        }

        // 去除前后空格/tab
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // 检查是否是组名
        if (!line.empty() && line.front() == '[' && line.back() == ']') 
        {
            string current_group = line;
            if (group_name.empty()) 
            {
                in_group = true; // 如果group_name为空，则读取所有组的项
            }
            else if (ignore_lower_upper_case ?
                       case_insensitive_match(current_group, group_name) :
                       current_group == group_name) 
            {
                in_group = true;
            } 
            else 
            {
                if (!in_group)
                {
                    continue;
                }
                in_group = false;
            }
        }
        else if (in_group && !line.empty() && line.find('=') != string::npos)
        {
            ret.push_back(line); // 添加项到向量中
        }
    }

    file.close();
    if (group_name.empty() || !groups.empty()) 
    {
        return ret.size();
    }
    return -1; // 如果指定组名不存在，则返回-1

}

/* 项读取函数 - 取某个group的一个item，放在string中 */
int cfgfile_read_tools::get_one_item(const char* const group_name, const char* const item_name, string& ret, const bool ignore_lower_upper_case)
{
    return get_one_item(string(group_name), item_name, ret, ignore_lower_upper_case);
}

int cfgfile_read_tools::get_one_item(const string& group_name, const string& item_name, string& ret, const bool ignore_lower_upper_case)
{
    vector<string> items;
    if (get_all_item(group_name, items, ignore_lower_upper_case) < 0)
        return -1;


    for (const auto& item : items)
    {
        // 获取等号位置
        size_t equal_pos = item.find('=');
        if (equal_pos == string::npos) 
        {
            continue; // 如果没有等号，跳过这一项
        }

        // 提取项名称，并去除前后空格
        string current_item_name = item.substr(0, equal_pos);
        current_item_name.erase(0, current_item_name.find_first_not_of(" \t"));
        current_item_name.erase(current_item_name.find_last_not_of(" \t") + 1);

        // 比较项名称，考虑是否忽略大小写
        if ((ignore_lower_upper_case && case_insensitive_match(current_item_name, item_name)) ||
            (!ignore_lower_upper_case && current_item_name == item_name)) 
        {
            // 找到匹配项，提取值
            ret = item;//.substr(equal_pos + 1);
            return 0; // 成功返回
        }
    }

    return -1; // 没有找到匹配的项，返回-1
}

/* 项读取函数 - 按数据类型读取某个group的某个item，放在对应数据类型的变量中
	数据类型包括：int/double/string/char * /char */
int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const char* const def_str, char* str, const int str_maxlen, const bool ignore_lower_upper_case)
{
    string str_value;
    if (get_str_value(group_name, item_name, str_value, ignore_lower_upper_case) < 0)
    {
        strcpy(str, def_str);
        return -1;
    }
    if (str_value.length() == 0)
    {
        str_value = def_str;
    }

    strncpy(str, str_value.c_str(), str_maxlen - 1);
    str[str_maxlen - 1] = '\0';
	return 0;
}

int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const char* const def_str, string& value, const bool ignore_lower_upper_case)
{
    string str_value;
    if (get_str_value(group_name, item_name, str_value, ignore_lower_upper_case) < 0)
    {
        value = def_str;
        return -1;
    }
    if (str_value.length() == 0)
    {
        str_value = def_str;
    }
    value = str_value;
	return 0;
}

int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, char& ch, const bool ignore_lower_upper_case)
{
    string str_value;
    if (get_str_value(group_name, item_name, str_value, ignore_lower_upper_case) < 0)
    {
        ch = ' ';
        return -1;
    }
    ch = str_value[0];
	return 0;
}

int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const int min_value, const int max_value, const int def_value, int& value, const bool ignore_lower_upper_case)
{
    string str_value;
    if (get_str_value(group_name, item_name, str_value, ignore_lower_upper_case) < 0)
    {
        value = def_value;
        return -1;
    }
    int number;
    try 
    {
        number = std::stoi(str_value);
    }
    catch (const exception& e) 
    {
        number = def_value;
    }
    if (number > max_value || number < min_value)
        number = def_value;
    value = number;
    return 0;
}

int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const double min_value, const double max_value, const double def_value, double& value, const bool ignore_lower_upper_case)
{
    string str_value;
    if (get_str_value(group_name, item_name, str_value, ignore_lower_upper_case) < 0)
    {
        value = def_value;
        return -1;
    }
    double number;
    try
    {
        number = std::stod(str_value);
    }
    catch (const exception& e)
    {
        number = def_value;
    }
    if (number > max_value || number < min_value)
        number = def_value;
    value = number;
    return 0;
}

/***************************************************************************
  函数类别：辅助函数
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

bool cfgfile_read_tools::case_insensitive_match(string str1, string str2)
{
    return ((str1.size() == str2.size()) &&
        equal(str1.begin(), str1.end(), str2.begin(),
            [](char& c1, char& c2) {
                return (tolower(c1) == tolower(c2));
            }));
}

int cfgfile_read_tools::get_str_value(const string& group_name, const string& item_name, string& ret, const bool ignore_lower_upper_case)
{
    string full_item;
    if (get_one_item(group_name, item_name, full_item, ignore_lower_upper_case) < 0)
        return -1;

    size_t equal_pos = full_item.find('=');
    string value = full_item.substr(equal_pos + 1);
    value.erase(remove(value.begin(), value.end(), ' '), value.end());
    ret = value;
    return 0;
}
