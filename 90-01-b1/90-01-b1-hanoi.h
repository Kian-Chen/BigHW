/* 2250951 光电 陈麒安 */
#pragma once
/* ------------------------------------------------------------------------------------------------------

	 本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问函数的函数声明
	2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
	3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
   ------------------------------------------------------------------------------------------------------ */
#include "../include/utilities.h"
void contr(int order, int src, int dst, int tmp, int n);
void get_hanoi(int data[]);
void arr_clear(int arr[], int n);
int i = 1;
int indi;
int stack[3][11], top[3];


