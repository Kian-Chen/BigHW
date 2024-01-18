/* 2250951 光电 陈麒安 */
#include <iostream>
#include<conio.h>
#include<Windows.h>
#include"90-01-b2.h"
#include "../include/cmd_console_tools.h"
#include "../include/utilities.h"
using namespace std;

void prompt()
{
	cout << "--------------------------------------------" << endl;
	cout << "A.命令行找出可消除项并标识" << endl;
	cout << "B.命令行完成一次消除（分步骤显示）" << endl;
	cout << "C.命令行完成一关（分步骤显示）" << endl;
	cout << "D.伪图形界面下用鼠标选择一个色块（无分隔线）" << endl;
	cout << "E.伪图形界面下用鼠标选择一个色块（有分隔线）" << endl;
	cout << "F.伪图形界面完成一次消除（分步骤）" << endl;
	cout << "G.伪图形界面完整版" << endl;
	cout << "Q.退出" << endl;
	cout << "--------------------------------------------" << endl;
	cout << "[请选择:]" << endl;
}


void get_rc(struct node &max)
{
	while (1) {
		cout << "请输入行数（8-10） ：  " << endl;
		cin >> max.y;   
		if (max.y >= 8 && max.y <= 10 && cin.good() == 1)
			break;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(114514, '\n');
		}
	}

	while (1) {
		cout << "请输入列数（8-10） ：  " << endl;
		cin >> max.x;
		if (max.x >= 8 && max.x <= 10 && cin.good() == 1)
			break;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(114514, '\n');
		}
	}
}

void control(int order, struct node max)
{
	int star[10][10] = { 0,0 }, book[10][10] = { 0,0 };
	randomset(star, max); 

	switch (order)
	{
	case 1:
		clear_arr(book, max);
		option_1(star, book, max);
		break;
	case 2:
		clear_arr(book, max);
		option_2(star, book, max);
		break;
	case 3:
		clear_arr(book, max);
		option_3(star, book, max);
		break;
	case 4:
		draw_bg(star, max, 0);
		op_45(max, star, 0);
		break;
	case 5:
		draw_bg(star, max);
		op_45(max, star);
		break;
	case 6:
		draw_bg(star, max, 0);
		op_6(star, book, max);
		break;
	case 7:
		draw_bg(star, max);
		op_7(star, book, max);
		break;
	}
}


int main()
{
	while (1)
	{
		int order = menu(prompt,7,true);
		if (!order)
			break;
		cct_cls();
		struct node max_console;
		get_rc(max_console);
		control(order, max_console);
		end(max_console, order);
		
	}
	cct_setconsoleborder(120, 30, 120, 9001);
	return 0;
}
