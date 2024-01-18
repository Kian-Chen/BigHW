/* 2250951 光电 陈麒安 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<fstream>
#include<iomanip>
#include<sstream>
#include"bmp_subtitles.h"
//按需加入需要的头文件 
using namespace std;

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
static void usage(const char* const argv0)
{
	char procname[256];

	/* 从"xx\\xx\\txt_compare.exe形式的argv[0]中截取txt_compare出来" */
	strcpy(procname, argv0); //这句替换成自己的截取函数 

	ostringstream msg;
	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;
	cout << endl;

	msg << procname << " [Ver1.0.1]" << endl;
	msg << endl;

	msg << "Usage: " << procname << " --file1 xxx --file2 xxx [ --trim none/left/rigth/all | --lineskip 0..100 | --lineoffset -100..100 | --ignore_blank | --max_diff 0..100 | --display none/normal/detailed ]" << endl;

	msg << setw(wkey) << ' ' << "必选项：" << endl;
	msg << setw(wopt) << ' ' << "--bmpfile origin           : 原始文件" << endl;
	msg << setw(wopt) << ' ' << "--outfile outputname       : 新的文件" << endl;
	msg << setw(wopt) << ' ' << "--posx x                   : x坐标" << endl;
	msg << setw(wopt) << ' ' << "--posy y                   : y坐标" << endl;
	msg << setw(wopt) << ' ' << "--color color              : 颜色选择（24b之外也要填，但默认黑）" << endl;
	msg << setw(wopt) << ' ' << "--subtitles str            : 字幕" << endl;

	msg << endl;

	msg << setw(wkey) << ' ' << "可选项：" << endl;
	msg << setw(wopt) << ' ' << "--traditional true/false       : 简体or繁体（无此项则默认为简体）" << endl;
	msg << setw(wopt) << ' ' << "--style horizontal/vertical    : 横竖（无此项则横）" << endl;
	msg << endl;
	/* 一次全部显示出来 */
	cout << msg.str() << endl;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main(int argc, char** argv)
{
	/* 指定去除空格的方式 */
	const string StyleType[] = { "horizontal",	"vertical", "" };

	args_analyse_tools args[] = {
		args_analyse_tools("--help",			ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--debug",			ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--bmpfile",			ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--outfile",			ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--traditional",	    ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--subtitles",		ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--posx",            ST_EXTARGS_TYPE::int_with_error,          1, 12345, 0, 1919),
		args_analyse_tools("--posy",            ST_EXTARGS_TYPE::int_with_error,          1, 12345, 0, 1079),
		args_analyse_tools("--color",		    ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--style",			ST_EXTARGS_TYPE::str_with_set_default, 1, 0, StyleType),
		args_analyse_tools()  //最后一个，用于结束
	};
	int cur_argc, ret = 0;

	//最后一个参数1，表示除选项参数外，还有其它参数
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//错误信息在函数中已打印
		args_analyse_print(args);
		usage(argv[0]);
		return -1;
	}

	/* 对help做特殊处理 */
	if (args[OPT_ARGS_HELP].existed()) {
		//只要有 --help，其它参数都忽略，显示帮助即可
		args_analyse_print(args);
		usage(argv[0]);
		return -1; //执行完成直接退出
	}

	if (args[OPT_ARGS_BMP].existed() + args[OPT_ARGS_OUTPUT].existed() + 
		args[OPT_ARGS_POSX].existed() + args[OPT_ARGS_POSY].existed()
		+ args[OPT_ARGS_COLOR].existed() + args[OPT_ARGS_SUBTITLES].existed() != 6) {
		usage(argv[0]);
		cout << "必须指定参数[" << args[OPT_ARGS_BMP].get_name() << "和" << args[OPT_ARGS_OUTPUT].get_name() << "]" << endl;
		return -1;
	}
	/*
	if (args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed() && (args[OPT_ARGS_CR_CRLF_NOT_EQUAL].get_string() == "right" || args[OPT_ARGS_CR_CRLF_NOT_EQUAL].get_string() == "all")) {
		usage(argv[0]);
		cout << "参数[" << args[OPT_ARGS_CR_CRLF_NOT_EQUAL].get_name() << "]不能和[" << args[OPT_ARGS_TRIM].get_name() << " right/all]同时存在." << endl;
		return -1;
	}*/

	if (args[OPT_ARGS_DEBUG].existed())
		args_analyse_print(args);

	/* 进入实际的功能调用，完成相应的功能 */

	args_distribute(args);

	return 0;
}

