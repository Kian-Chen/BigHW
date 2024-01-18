/* 2250951 光电 陈麒安 */
#include <iostream>
#include"90-02-b1.h"
#include "../include/cmd_console_tools.h"
#include"../include/utilities.h"
using namespace std;

void prompt()
{
	cout << "---------------------------------" << endl;
	cout << "1.单个数字下落（实现下落逻辑与边控）" << endl;
	cout << "2.在1的基础上加入旋转控制" << endl;
	cout << "3.加入左右控制" << endl;
	cout << "4.多个下落（利用随机序列获取）" << endl;
	cout << "5.加入计分规则" << endl;
	cout << "0.退出" << endl;
	cout << "---------------------------------" << endl;
	cout << "注意，为了利用窗口标题设置这一函数，该程序采用将游戏简要信息显示在窗口标题中！" << endl;
	cout << "[请选择:] ";
}

void control(int order, struct node max_phy)
{
	int board_book_phy[21][26] = { 0,0 };
	char title[256] = { 0 };
	cct_setconsoletitle("欢迎来到俄罗斯方块");
	switch (order)
	{
	case 1:
		oneBlockFall(
			getNextNum(true, true),
			board_book_phy,
			max_phy,
			1);
		break;
	case 2:
		oneBlockFall(getNextNum(true, true),
			board_book_phy, 
			max_phy,
			2);
		break;
	case 3:
		oneBlockFall(getNextNum(true, true),
			board_book_phy,
			max_phy, 
			3);
		break;
	case 4:
		while (oneBlockFall(getNextNum(true, true),
			board_book_phy,
			max_phy,
			4))
		{
			int n = checkIfEliminate(board_book_phy, max_phy);
			eliminateLine(board_book_phy, max_phy);
			drawBoard(board_book_phy, max_phy);
		}
		break;
	case 5:
		while (oneBlockFall(getNextNum(true, true),
			board_book_phy,
			max_phy,
			4))
		{
			int n = checkIfEliminate(board_book_phy, max_phy);
			interScore(true, n);
			eliminateLine(board_book_phy, max_phy);
			drawBoard(board_book_phy, max_phy);
		}
		break;
	default:
		break;
	}
}

void getConsoleRC(struct node& max_phy)
{
	while (1) {
		cout << "请输入高度（18-26） ：  " << endl;
		cin >> max_phy.y;
		if (max_phy.y >= 18 && max_phy.y <= 26 && cin.good() == 1)
			break;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(114514, '\n');
		}
	}

	while (1) {
		cout << "请输入宽度（12-21，且为3整数倍） ：  " << endl;
		cin >> max_phy.x;
		if (max_phy.x >= 12 && max_phy.x <= 21 && max_phy.x % 3 == 0 && cin.good() == 1)
			break;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(114514, '\n');
		}
	}
}

int main()
{
	while (1)
	{
		int order = menu(prompt, 5);
		if (!order)
			break;
		cct_cls();
		struct node max_console_phy;
		getConsoleRC(max_console_phy);
		background(max_console_phy);
		control(order, max_console_phy);

		waitEnter();
		cct_cls();
	}
	cct_setconsoleborder(120, 30, 120, 9001);
	return 0;
}
