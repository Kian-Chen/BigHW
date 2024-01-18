/* 2250951 ¹âµç ³Â÷è°² */
#pragma once
#include "../include/class_aat.h"
#include"../include/cmd_console_tools.h"

enum OPT_ARGS
{
	OPT_ARGS_HELP = 0,
	OPT_ARGS_DEBUG,
	OPT_ARGS_FILE1,
	OPT_ARGS_FILE2,
	OPT_ARGS_TRIM,
	OPT_ARGS_LINE_SKIP,
	OPT_ARGS_LINE_OFFSET,
	OPT_ARGS_IGNORE_BLANK,
	OPT_ARGS_CR_CRLF_NOT_EQUAL,
	OPT_ARGS_MAX_DIFFNUM,
	OPT_ARGS_MAX_LINENUM,
	OPT_ARGS_DISPLAY
};
enum FileFormat { Win, Linux, Confuse };

struct info
{
	bool empty;
	bool eof;
};
string extractFilename(const char* path);

void args_distribute(const args_analyse_tools* args);
