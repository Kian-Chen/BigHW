/* 2250951 光电 陈麒安 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../include/class_aat.h"
//如有必要，可以加入其它头文件
using namespace std;

#if !ENABLE_LIB_COMMON_TOOLS //不使用lib才有效

/* ---------------------------------------------------------------
	 允许加入其它需要static函数（内部工具用）
   ---------------------------------------------------------------- */


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
    args_name = "";
    extargs_type=ST_EXTARGS_TYPE::null;
    extargs_num = 0; 

    bool extargs_bool_default = true;

    extargs_int_default = -1;	//int型额外参数的缺省值（default）
    extargs_int_min = -1;		//int型额外参数的最小值（min）
    extargs_int_max = -1;		//int型额外参数的最大值（max）

    extargs_string_default = "";

    extargs_ipaddr_default = 0;	
    extargs_int_set = NULL;
    extargs_string_set = NULL;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：boolean
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const bool def)
{
    args_name = name;
    extargs_type = type;
    extargs_num = ext_num;
    extargs_bool_default = def;
    args_existed = false;

    extargs_int_set = NULL;
    extargs_string_set = NULL;
    extargs_int_default = -1;	//int型额外参数的缺省值（default）
    extargs_int_min = -1;		//int型额外参数的最小值（min）
    extargs_int_max = -1;		//int型额外参数的最大值（max）

    extargs_string_default = "";

    extargs_ipaddr_default = 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_default、int_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def, const int _min, const int _max)
{
    args_name = name;
    extargs_type = type;
    extargs_num = ext_num;
    extargs_int_default = def;
    extargs_int_value = def;
    extargs_int_min = _min;
    extargs_int_max = _max;
    args_existed = false;

    extargs_int_set = NULL;
    extargs_string_set = NULL;

    extargs_string_default = "";
    extargs_ipaddr_default = 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_set_default、int_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const int* const set)
{
    extargs_int_set = (int*)set;
    args_name = name;
    extargs_type = type;
    extargs_num = ext_num;
    extargs_int_default = set[def_of_set_pos];
    int len = 0;
    while (set[len++] != INVALID_INT_VALUE_OF_SET)
        ;
    extargs_int_set = new int[len];
    for (int i = 0; i < len; i++)
        extargs_int_set[i] = set[i];
    args_existed = false;

    extargs_int_min = -1;		//int型额外参数的最小值（min）
    extargs_int_max = -1;		//int型额外参数的最大值（max）
    extargs_string_default = "";
    extargs_ipaddr_default = 0;
    extargs_string_set = NULL;
}

static unsigned int get_ipaddr_str(const string str)
{
    unsigned int ip1, ip2, ip3, ip4;
    sscanf(str.c_str(), "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4);
    unsigned int result = (ip1 << 24) + (ip2 << 16) + (ip3 << 8) + ip4;
    //cout << ip1 << " " << ip2 << " " << ip3 << " " << ip4 << " " << result << endl;
    return result;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str、ipaddr_with_default、ipaddr_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const string def)
{
    args_name = name;
    extargs_type = type;
    extargs_num = ext_num;

    if (type == ST_EXTARGS_TYPE::ipaddr_with_default)
    {
        extargs_ipaddr_default = get_ipaddr_str(def);
        extargs_ipaddr_value = extargs_ipaddr_default;
        extargs_string_default = ""; 
    }
    else
    {
        extargs_ipaddr_default = 0;
        extargs_string_default = def;
        extargs_string_value = def; 
    }
    args_existed = false;

    extargs_int_set = NULL;
    extargs_string_set = NULL;
    extargs_int_default = -1;	//int型额外参数的缺省值（default）
    extargs_int_min = -1;		//int型额外参数的最小值（min）
    extargs_int_max = -1;		//int型额外参数的最大值（max）
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str_with_set_default、str_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const string* const set)
{
    args_name = name;
    extargs_type = type;
    extargs_num = ext_num;
    extargs_string_default = set[def_of_set_pos];
    int len = 0;
    while (set[len++] != "")
        ;
    extargs_string_set = new string[len];
    for (int i = 0; i < len; i++)
        extargs_string_set[i] = set[i];

    args_existed = false;
    extargs_int_default = -1;	//int型额外参数的缺省值（default）
    extargs_int_min = -1;		//int型额外参数的最小值（min）
    extargs_int_max = -1;		//int型额外参数的最大值（max）
    extargs_ipaddr_default = 0;
    extargs_int_set = NULL;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{
    //cout << args_name << "'s space is NULL : " << (extargs_int_set != NULL) << endl;
    if (extargs_int_set == NULL)
        delete[] extargs_int_set;
    if(extargs_string_set==NULL)
        delete[] extargs_string_set;
}

/* ---------------------------------------------------------------
	 允许AAT中自定义成员函数的实现（private）
   ---------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return this->args_name;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：加!!后，只能是0/1
			已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!this->args_existed;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return this->extargs_int_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return this->extargs_string_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return this->extargs_ipaddr_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：将 extargs_ipaddr_value 的值从 0x7f000001 转为 "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
    unsigned int value = extargs_ipaddr_value;
    stringstream ss;
    for (int i = 0; i < 4; i++)
    {
        int num = (value >> (24 - i * 8)) & 0xFF;
        if (i != 0) ss << ".";
        ss << num;
    }
    return ss.str();
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：follow_up_args：是否有后续参数
			0  ：无后续参数
			1  ：有后续参数
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_process(const int argc, const char* const *const argv, args_analyse_tools* const args, const int follow_up_args)
{
    int cur_argc = 1;  // 跳过程序名称
    
    while (cur_argc < argc) 
    {
        const char* arg = argv[cur_argc];

        if (strncmp(arg, "--", 2) == 0) 
        {
            const string arg_name = arg;  
            args_analyse_tools* current_arg = nullptr;

            // 找到当前参数在args数组中的位置

            for (int i = 0; args[i].extargs_type != ST_EXTARGS_TYPE::null; i++)
            {
                if (args[i].get_name() == arg_name)
                {
                    current_arg = &args[i];
                    args[i].args_existed += 1;
                }
                if (args[i].args_existed > 1) 
                {
                    cout << "错误：参数" << arg_name << "重复出现" << endl;
                    return -1;
                }
            }
            if (current_arg) 
            {
                // 处理不同类型的参数
                switch (current_arg->extargs_type)
                {
                case ST_EXTARGS_TYPE::boolean:
                    current_arg->args_existed = true;
                    break;

                case ST_EXTARGS_TYPE::int_with_default:
                    // 处理整数类型参数（带缺省值）
                    cur_argc++;
                    if (cur_argc < argc&& strncmp(argv[cur_argc], "--", 2) != 0) 
                    {
                        int value = atoi(argv[cur_argc]);
                        current_arg->extargs_int_value = (value >= current_arg->extargs_int_min && value <= current_arg->extargs_int_max) ? value : current_arg->extargs_int_default;
                        current_arg->args_existed = true;
                    }
                    else {
                        // 参数值缺失错误处理
                        cout << "错误：参数" << arg << "缺少值" << endl;
                        return -1;
                    }
                    break;

                case ST_EXTARGS_TYPE::int_with_error:
                    // 处理整数类型参数（错误处理）
                    cur_argc++;
                    if (cur_argc < argc && strncmp(argv[cur_argc], "--", 2) != 0)
                    {
                        int value = atoi(argv[cur_argc]);
                        if (value >= current_arg->extargs_int_min && value <= current_arg->extargs_int_max)
                        {
                            current_arg->extargs_int_value = value;
                            current_arg->args_existed = true;
                        }
                        else 
                        {
                            // 参数值范围错误处理
                            cout << "错误：参数" << arg << "的值超出范围" << endl;
                            return -1;
                        }
                    }
                    else 
                    {
                        // 参数值缺失错误处理
                        cout << "错误：参数" << arg << "缺少值" << endl;
                        return -1;
                    }
                    break;


                case ST_EXTARGS_TYPE::int_with_set_default:
                    // 处理整数类型参数（带集合、缺省值）
                    cur_argc++;
                    if (cur_argc < argc && strncmp(argv[cur_argc], "--", 2) != 0) 
                    {
                        int value = atoi(argv[cur_argc]);
                        // 检查值是否在集合中，如果在，则将其赋值给 current_arg->extargs_int_value，否则使用缺省值
                        bool valueInSet = false;
                        for (int i = 0; current_arg->extargs_int_set[i]!= INVALID_INT_VALUE_OF_SET; ++i) 
                        {
                            if (current_arg->extargs_int_set[i] == value) 
                            {
                                valueInSet = true;
                                break;
                            }
                        }
                        current_arg->extargs_int_value = valueInSet ? value : current_arg->extargs_int_default;
                        current_arg->args_existed = true;
                    }
                    else 
                    {
                        // 参数值缺失错误处理
                        cout << "错误：参数" << arg << "缺少值" << endl;
                        return -1;
                    }
                    break;

                case ST_EXTARGS_TYPE::int_with_set_error:
                    // 处理整数类型参数（带集合、错误处理）
                    cur_argc++;
                    if (cur_argc < argc && strncmp(argv[cur_argc], "--", 2) != 0) 
                    {
                        int value = atoi(argv[cur_argc]);
                        // 检查值是否在集合中，如果在，则将其赋值给 current_arg->extargs_int_value，否则输出错误信息并返回 -1
                        bool valueInSet = false;
                        for (int i = 0; current_arg->extargs_int_set[i] != INVALID_INT_VALUE_OF_SET; ++i) 
                        {
                            if (current_arg->extargs_int_set[i] == value)
                            {
                                valueInSet = true;
                                break;
                            }
                        }
                        if (valueInSet) 
                        {
                            current_arg->extargs_int_value = value;
                            current_arg->args_existed = true;
                        }
                        else 
                        {
                            // 参数值集合错误处理
                            cout << "错误：参数" << arg << "的值不在允许的集合中" << endl;
                            return -1;
                        }
                    }
                    else 
                    {
                        // 参数值缺失错误处理
                        cout << "错误：参数" << arg << "缺少值" << endl;
                        return -1;
                    }
                    break;


                case ST_EXTARGS_TYPE::str:
                    // 处理字符串类型参数
                    cur_argc++;
                    if (cur_argc < argc && strncmp(argv[cur_argc], "--", 2) != 0) 
                    {
                        current_arg->extargs_string_value = argv[cur_argc];
                        current_arg->args_existed = true;
                    }
                    else
                    {
                        // 参数值缺失错误处理
                        cout << "错误：参数" << arg << "缺少值" << endl;
                        return -1;
                    }
                    break;

                case ST_EXTARGS_TYPE::str_with_set_default:
                    // 处理字符串类型参数（带集合、缺省值）
                    cur_argc++;
                    if (cur_argc < argc && strncmp(argv[cur_argc], "--", 2) != 0) 
                    {
                        string value = argv[cur_argc];
                        // 检查值是否在集合中，如果在，则将其赋值给 current_arg->extargs_string_value，否则使用缺省值
                        bool valueInSet = false;
                        for (int i = 0; current_arg->extargs_string_set[i] != ""; ++i)
                        {
                            if (current_arg->extargs_string_set[i] == value)
                            {
                                valueInSet = true;
                                break;
                            }
                        }
                        current_arg->extargs_string_value = valueInSet ? value : current_arg->extargs_string_default;
                        current_arg->args_existed = true;
                    }
                    else
                    {
                        // 参数值缺失错误处理
                        cout << "错误：参数" << arg << "缺少值" << endl;
                        return -1;
                    }
                    break;

                case ST_EXTARGS_TYPE::str_with_set_error:
                    // 处理字符串类型参数（带集合、错误处理）
                    cur_argc++;
                    if (cur_argc < argc && strncmp(argv[cur_argc], "--", 2) != 0)
                    {
                        string value = argv[cur_argc];
                        // 检查值是否在集合中，如果在，则将其赋值给 current_arg->extargs_string_value，否则输出错误信息并返回 -1
                        bool valueInSet = false;
                        for (int i = 0; current_arg->extargs_string_set[i] != ""; ++i) 
                        {
                            if (current_arg->extargs_string_set[i] == value)
                            {
                                valueInSet = true;
                                break;
                            }
                        }
                        if (valueInSet)
                        {
                            current_arg->extargs_string_value = value;
                            current_arg->args_existed = true;
                        }
                        else 
                        {
                            // 参数值集合错误处理，输出错误信息
                            cout << "错误：参数" << arg << "的值不在允许的集合中" << endl;
                            return -1;
                        }
                    }
                    else 
                    {
                        // 参数值缺失错误处理
                        cout << "错误：参数" << arg << "缺少值" << endl;
                        return -1;
                    }
                    break;



                case ST_EXTARGS_TYPE::ipaddr_with_default:
                    // 处理IP地址类型参数（带缺省值）
                    cur_argc++;
                    if (cur_argc < argc)
                    {
                        string ipStr = argv[cur_argc];
                        unsigned int ipValue = 0;
                        // 将 IP 地址字符串转换为 unsigned int
                        for (int i = 0, part = 24; i < 4; i++, part -= 8)
                        {
                            int num = 0;
                            size_t pos = ipStr.find('.', 0);
                            if (pos != string::npos)
                            {
                                num = stoi(ipStr.substr(0, pos));
                                ipStr = ipStr.substr(pos + 1);
                            }
                            else 
                            {
                                num = stoi(ipStr);
                            }
                            if (num < 0 || num > 255)
                            {
                                // IP 地址错误处理，输出错误信息
                                cout << "错误：参数" << arg << "的值不是合法的 IP 地址" << endl;
                                return -1;
                            }
                            ipValue |= (num << part);
                        }
                        current_arg->extargs_ipaddr_value = get_ipaddr_str(ipStr);
                        current_arg->args_existed = true;
                    }
                    else 
                    {
                        // 参数值缺失错误处理
                        cout << "错误：参数" << arg << "缺少值" << endl;
                        return -1;
                    }
                    break;

                case ST_EXTARGS_TYPE::ipaddr_with_error:
                    // 处理IP地址类型参数（错误处理）
                    cur_argc++;
                    if (cur_argc < argc) 
                    {
                        string ipStr = argv[cur_argc];
                        unsigned int ipValue = 0;
                        // 将 IP 地址字符串转换为 unsigned int
                        for (int i = 0, part = 24; i < 4; i++, part -= 8) 
                        {
                            int num = 0;
                            size_t pos = ipStr.find('.', 0);
                            if (pos != string::npos)
                            {
                                num = stoi(ipStr.substr(0, pos));
                                ipStr = ipStr.substr(pos + 1);
                            }
                            else 
                            {
                                num = stoi(ipStr);
                            }
                            if (num < 0 || num > 255) 
                            {
                                // IP 地址错误处理，输出错误信息
                                cout << "错误：参数" << arg << "的值不是合法的 IP 地址" << endl;
                                return -1;
                            }
                            ipValue |= (num << part);
                        }
                        // 检查是否是合法的 IP 地址
                        current_arg->extargs_ipaddr_value = ipValue;
                        current_arg->args_existed = true;
                    }
                    else 
                    {
                        // 参数值缺失错误处理
                        cout << "错误：参数" << arg << "缺少值" << endl;
                        return -1;
                    }
                    break;


                    // 处理其他类型的参数...

                default:
                    cout << "错误：未知的参数类型" << endl;
                    return -1;
                }

            }
            else 
            {
                cout << "错误：未知的参数" << arg << endl;
                return -1;
            }
        }
        else 
        {
            if (follow_up_args)
                return cur_argc;
            cout << "错误：无效的参数" << arg << endl;
            return -1;
        }

        cur_argc++;
    }
    return cur_argc;
}

static int get_len(string str)
{
    return str.length() + 1;
}

static int get_len(ST_EXTARGS_TYPE type)
{
    switch (type)
    {
    case ST_EXTARGS_TYPE::null:
        return 1;
        break;
    case ST_EXTARGS_TYPE::boolean:
        return 5;
        break;
    case ST_EXTARGS_TYPE::int_with_default:
        return 15;
        break;
    case ST_EXTARGS_TYPE::int_with_error:
        return 13;
        break;
    case ST_EXTARGS_TYPE::int_with_set_default:
        return 18;
        break;
    case ST_EXTARGS_TYPE::int_with_set_error:
        return 16;
        break;
    case ST_EXTARGS_TYPE::str:
        return 7;
        break;
    case ST_EXTARGS_TYPE::str_with_set_default:
        return 21;
        break;
    case ST_EXTARGS_TYPE::str_with_set_error:
        return 19;
        break;
    case ST_EXTARGS_TYPE::ipaddr_with_default:
        return 18;
        break;
    case ST_EXTARGS_TYPE::ipaddr_with_error:
        return 18;
        break;
    case ST_EXTARGS_TYPE::tmax:
        return 1;
        break;
    default:
        return 1;
        break;
    }
}

static int get_len(int x)
{
    if (x < 0)
        x = -x;
    int len = 0;
    if (x == 0) {
        return 1;
    }
    while (x != 0) {
        len++;
        x /= 10;
    }
    return len + 1;
}

static int get_len(int min, int max)
{
    return get_len(min) + get_len(max) + 3;
}

static int get_len(const int* set)
{
    if (set == NULL)
        return 1;
    int i = 0, len = 0;
    while (set[i] != INVALID_INT_VALUE_OF_SET)
        len += get_len(set[i++]);
    return len;
}

static int get_len(string* set)
{
    if (set == NULL)
        return 1;
    int i = 0, len = 0;
    while (set[i] != "")
        len += get_len(set[i++]);
    return len;
}

static int max_len(int* lens, int num)
{
    int max = 0;
    for (int i = 0; i < num; i++)
        max += lens[i];
    return max;
}

static void print_bar(int num)
{
    cout << setw(num) << setfill('=') << "" << endl;
    cout << setfill(' ');
}

static void print_title(int* lens, string* name, int num)
{
    for (int i = 0; i < num; i++)
        cout << setw(lens[i]) << setiosflags(ios::left) << name[i];
    cout << endl;
}

static string context(ST_EXTARGS_TYPE type)
{
    switch (type)
    {
    case ST_EXTARGS_TYPE::null:
        return "";
        break;
    case ST_EXTARGS_TYPE::boolean:
        return "Bool";
        break;
    case ST_EXTARGS_TYPE::int_with_default:
        return "IntWithDefault";
        break;
    case ST_EXTARGS_TYPE::int_with_error:
        return "IntWithError";
        break;
    case ST_EXTARGS_TYPE::int_with_set_default:
        return "IntSETWithDefault";
        break;
    case ST_EXTARGS_TYPE::int_with_set_error:
        return "IntSETWithError";
        break;
    case ST_EXTARGS_TYPE::str:
        return "String";
        break;
    case ST_EXTARGS_TYPE::str_with_set_default:
        return "StringSETWithDefault";
        break;
    case ST_EXTARGS_TYPE::str_with_set_error:
        return "StringSETWithError";
        break;
    case ST_EXTARGS_TYPE::ipaddr_with_default:
        return "IPAddrWithDefault";
        break;
    case ST_EXTARGS_TYPE::ipaddr_with_error:
        return "IPAddrWithError";
        break;
    case ST_EXTARGS_TYPE::tmax:
        return "";
        break;
    default:
        break;
    }
    return "";
}

static string get_def(bool bdef, int idef, string sdef, string ipdef, ST_EXTARGS_TYPE type)
{
    switch (type)
    {
    case ST_EXTARGS_TYPE::null:
        break;
    case ST_EXTARGS_TYPE::boolean:
        return bdef ? "true" : "false";
        break;
    case ST_EXTARGS_TYPE::int_with_default:
        return to_string(idef);
        break;
    case ST_EXTARGS_TYPE::int_with_error:
        break;
    case ST_EXTARGS_TYPE::int_with_set_default:
        return to_string(idef);
        break;
    case ST_EXTARGS_TYPE::int_with_set_error:
        break;
    case ST_EXTARGS_TYPE::str:
        if (sdef != "")
            return sdef;
        else
            return "/";
        break;
    case ST_EXTARGS_TYPE::str_with_set_default:
        if (sdef != "")
            return sdef;
        else
            return "/";
        break;
    case ST_EXTARGS_TYPE::str_with_set_error:
        break;
    case ST_EXTARGS_TYPE::ipaddr_with_default:
        return ipdef;
        break;
    case ST_EXTARGS_TYPE::ipaddr_with_error:
        break;
    case ST_EXTARGS_TYPE::tmax:
        break;
    default:
        break;
    }
    return "/";
}

static string get_val(bool bval, int ival, string sval, string ipval, ST_EXTARGS_TYPE type)
{
    switch (type)
    {
    case ST_EXTARGS_TYPE::null:
        break;
    case ST_EXTARGS_TYPE::boolean:
        return bval ? "true" : "false";
        break;
    case ST_EXTARGS_TYPE::int_with_default:
    case ST_EXTARGS_TYPE::int_with_error:
    case ST_EXTARGS_TYPE::int_with_set_default:
    case ST_EXTARGS_TYPE::int_with_set_error:
        return to_string(ival);
        break;
    case ST_EXTARGS_TYPE::str:
    case ST_EXTARGS_TYPE::str_with_set_default:
    case ST_EXTARGS_TYPE::str_with_set_error:
        return sval;
        break;
    case ST_EXTARGS_TYPE::ipaddr_with_default:
    case ST_EXTARGS_TYPE::ipaddr_with_error:
        return ipval;
        break;
    case ST_EXTARGS_TYPE::tmax:
        break;
    default:
        break;
    }
    return "";
}

static string get_range(int min, int max, int* iset, string* sset, ST_EXTARGS_TYPE type)
{
    string result;
    switch (type)
    {
    case ST_EXTARGS_TYPE::null:
    case ST_EXTARGS_TYPE::boolean:
        return "/";
    case ST_EXTARGS_TYPE::int_with_default:
    case ST_EXTARGS_TYPE::int_with_error:
        return "[" + to_string(min) + ".." + to_string(max) + "]";
        break;
    case ST_EXTARGS_TYPE::int_with_set_default:
    case ST_EXTARGS_TYPE::int_with_set_error:
        for (int i = 0; iset[i] != INVALID_INT_VALUE_OF_SET; i++)
        {
            if (i != 0)
                result += "/";
            result += to_string(iset[i]);
        }
        return result;
        break;
    case ST_EXTARGS_TYPE::str:
        return "/";
        break;
    case ST_EXTARGS_TYPE::str_with_set_default:
    case ST_EXTARGS_TYPE::str_with_set_error:
        for (int i = 0; sset[i] != ""; i++)
        {
            if (i != 0)
                result += "/";
            result += sset[i];
        }
        return result;
        break;
    case ST_EXTARGS_TYPE::ipaddr_with_default:
    case ST_EXTARGS_TYPE::ipaddr_with_error:
        return "/";
        break;
    case ST_EXTARGS_TYPE::tmax:
        break;
    default:
        break;
    }
    return "/";
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
    int len = 0, i = 0, lens[6] = { 0 };
    string title[] = { " name" ,"type " ,"default " ,"exists " ,"value " ,"range/set " };
    while (args[i].extargs_type != ST_EXTARGS_TYPE::null)
    {
        if (get_len(args[i].args_name) > lens[0])
            lens[0] = get_len(args[i].args_name);
        if (get_len(args[i].extargs_type) > lens[1])
            lens[1] = get_len(args[i].extargs_type);
        if (max(get_len(args[i].extargs_int_default), max(get_len(args[i].extargs_string_default),get_len(args[i].get_str_ipaddr()))) > lens[2])
            lens[2] = max(get_len(args[i].extargs_int_default), max(get_len(args[i].extargs_string_default), get_len(args[i].get_str_ipaddr())));
        if (get_len(args[i].args_existed) > lens[3])
            lens[3] = get_len(args[i].args_existed);
        if (max(get_len(args[i].extargs_int_value), max((!args[i].args_existed ? get_len(args[i].extargs_string_value):0), (!args[i].args_existed?get_len(args[i].get_str_ipaddr()):0))) > lens[4])
            lens[4] = max(get_len(args[i].extargs_int_value), max((!args[i].args_existed ? get_len(args[i].extargs_string_value) : 0), (!args[i].args_existed ? get_len(args[i].get_str_ipaddr()) : 0)));
        if (max(get_len(args[i].extargs_int_min, args[i].extargs_int_max), max(get_len(args[i].extargs_int_set), get_len(args[i].extargs_string_set))) > lens[5])
            lens[5] = max(get_len(args[i].extargs_int_min, args[i].extargs_int_max), max(get_len(args[i].extargs_int_set), get_len(args[i].extargs_string_set)));


        if (get_len(" name") > lens[0])
            lens[0] = get_len(" name");
        if (get_len("type ") > lens[1])
            lens[1] = get_len("type ");
        if (get_len("default ") > lens[2])
            lens[2] = get_len("default ");
        if (get_len("exists ") > lens[3])
            lens[3] = get_len("exists");
        if (get_len("value ") > lens[4])
            lens[4] = get_len("value ");
        if (get_len("range/set ") > lens[5])
            lens[5] = get_len("range/set ");
        i++;
    } 

    len = max_len(lens, 6);
    print_bar(len);
    print_title(lens, title, 6);
    print_bar(len);

    i = 0;
    while (args[i].extargs_type != ST_EXTARGS_TYPE::null)
    {
        cout << setw(lens[0]) << args[i].args_name;
        cout << setw(lens[1]) << context(args[i].extargs_type);
        cout << setw(lens[2]) << get_def(args[i].extargs_bool_default, args[i].extargs_int_default, args[i].extargs_string_default, args[i].get_str_ipaddr(), args[i].extargs_type);
        (args[i].args_existed > 0 ? cout << setw(lens[3]) << args[i].args_existed : cout << setw(lens[3]) << 0);
        cout << setw(lens[4]) << (args[i].args_existed > 0 ? get_val(args[i].args_existed, args[i].extargs_int_value, args[i].extargs_string_value, args[i].get_str_ipaddr(), args[i].extargs_type) : "/");
        cout << setw(lens[5]) << get_range(args[i].extargs_int_min, args[i].extargs_int_max, args[i].extargs_int_set, args[i].extargs_string_set, args[i].extargs_type);
        cout << endl;
        i++;
    }

    print_bar(len);

	return 0; //此句根据需要修改
}

#endif // !ENABLE_LIB_COMMON_TOOLS
