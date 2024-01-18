/* 2250951 光电 陈麒安 */
#include "../include/cmd_console_tools.h"
#include "../include/utilities.h"
#include"90-01-b1-hanoi.h"
#include<iostream>
#include<conio.h>
using namespace std;

void arr_clear(int arr[], int n)
{
	for (int i = 0; i < n; i++)
		arr[i] = 0;
}

void draw()
{
	cout << "---------------------------------" << endl;
	cout << "1.基本解" << endl;
	cout << "2.基本解(步数记录)" << endl;
	cout << "3.内部数组显示(横向)" << endl;
	cout << "4.内部数组显示(纵向+横向)" << endl;
	cout << "5.图形解-预备-画三个圆柱" << endl;
	cout << "6.图形解-预备-在起始柱上画n个盘子" << endl;
	cout << "7.图形解-预备-第一次移动" << endl;
	cout << "8.图形解-自动移动版本" << endl;
	cout << "9.图形解-游戏版" << endl;
	cout << "0.退出" << endl;
	cout << "---------------------------------" << endl;
	cout << "[请选择:] ";
}

int main()
{
	cct_setconsoleborder(120, 40, 120, 9000);
	while(1)
	{
		int order = menu(draw, 9);
		if (!order)
			break;
		int data[4] = { 0 };
		if (order != 5)
			get_hanoi(data);
		int src = data[0], dst = data[1], tmp = data[2], n = data[3];
		contr(order, src, dst, tmp, n);
		cct_cls();
		arr_clear(stack[0], 11), arr_clear(stack[1], 11), arr_clear(stack[2], 11);
		top[0] = 0, top[1] = 0, top[2] = 0, i = 1, indi = 0;
	}
	return 0;
}
