/* 2250951 光电 陈麒安 */
#include<iostream>
#include<conio.h>
#include "../include/cmd_console_tools.h"
#include"90-01-b2.h"
using namespace std;

void put_arr(int arr[10][10], int book[10][10], struct node max, const char* const prompt, const char ch,int all)
{
	if (prompt != NULL)
		cout << prompt << endl;
	cout << "  |";
	for (int i = 0; i < max.x; i++)
		cout << "  " << i;
	cout << endl << "--+";
	for (int i = 0; i < max.x; i++)
		cout << "---";
	cout << "-" << endl;

	for (int i = 0; i < max.y; i++)
	{
		cout << (char)('A' + i) << " |";
		for (int j = 0; j < max.x; j++)
		{
			cout << "  ";

			if(all==1)
			{
				if (arr[i][j] == 0)
				{
					cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
					cout << arr[i][j];
					cct_setcolor();
				}
				else
					cout << arr[i][j];
				continue;
			}

			if (book[i][j] != 0)
			{
				if (ch == 0)
				{
					cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
					cout << arr[i][j];
					cct_setcolor();
				}
				else
					cout << "*";
			}
			else
				cout << arr[i][j];

		}
		cout << endl ;
	}
	cout << endl << endl;
}

struct node get_cmd(int arr[10][10], struct node max)
{
	struct node input;
	while(1)
	{
		cout << "                                                    ";
		cout << "\r请以字母+数字形式[例：c2]输入矩阵坐标：";
		struct node wish;
		cct_getxy(wish.x, wish.y);
		char ch1, ch2;
		cin >> ch1 >> ch2;
		if (ch1 >= 'a' && ch1 <= 'z')
			ch1 -= 'a' - 'A';
		if ((ch1 - 'A' < 0 || ch1 - 'A' > max.y) || (ch2 - '0' < 0 || ch2 - '0' > max.x))
		{
			cout << "输入错误，请重新输入.";
			cin.ignore(114514, '\n');
			cct_gotoxy(wish.x, wish.y);
			continue;
		}
		cout << "输入为" << ch1 << "行" << ch2 << "列" << endl;
		input.x = ch2 - '0';
		input.y = ch1 - 'A';
		if (arr[input.y][input.x] == 0)
		{
			cout << "输入的矩阵坐标位置为0（非法位置），请重新输入" << endl;
			continue;
		}

		if (check_point(arr, input, max) == 0)
		{
			cout << "输入的矩阵坐标位置处无连续相同值，请重新输入" << endl;
			continue;
		}
		break;
	}
	return input;
}

int inquire_cmd(struct node now)
{
	cout << "请确认是否把" << (char)(now.y + 'A') << now.x << "及周围的相同值消除(Y / N / Q)：";
	char ch;
	int choice;
	while (1)
	{
		ch = _getch();
		if (ch >= 'a' && ch <= 'z')
			ch -= 'a' - 'A';
		if (ch != 'Y' && ch != 'N' && ch != 'Q')
			continue;
		else
		{
			cout << ch << endl;
			if (ch == 'Y')
				choice = 1;
			else if (ch == 'N')
				choice = 0;
			else
				choice = -1;
			break;
		}
	}
	return choice;
}

void get_enter(const char* const prompt)
{
	cout << prompt << endl;
	while (_getch() != '\r')
		;
}

void option_1(int star[10][10], int book[10][10], struct node max)
{
	put_arr(star, book, max, "当前数组：");
	struct node choice = get_cmd(star, max);
	bfs(star, book, choice, max);
	put_arr(book, book, max, "查找结果数组：", '*');
	put_arr(star, book, max, "当前数组(不同色标识)：");
	clear_arr(book, max);
}

void option_2(int star[10][10], int book[10][10], struct node max)
{
	put_arr(star, book, max, "当前数组：");
	struct node choice = get_cmd(star, max);
	int same = bfs(star, book, choice, max);
	put_arr(book, book, max, "查找结果数组：", '*');
	put_arr(star, book, max, "当前数组(不同色标识)：");
	int is_e = inquire_cmd(choice);
	if (is_e == 1)
	{
		eliminate(star, book, max);
		put_arr(star, book, max, "相同值归并后的数组(不同色标识)：", 0, 1);
		score(same);
	}
	score();
}

void option_3(int star[10][10], int book[10][10], struct node max)
{
	while (1)
	{
		put_arr(star, book, max, "当前数组：", 0, 1);
		struct node choice = get_cmd(star, max);
		int same = bfs(star, book, choice, max);
		put_arr(book, book, max, "查找结果数组：", '*');
		put_arr(star, book, max, "当前数组(不同色标识)：");
		int is_e = inquire_cmd(choice);
		if (is_e == 1)
		{
			eliminate(star, book, max);
			put_arr(star, book, max, "相同值归并后的数组(不同色标识)：");
			score(same);
			get_enter("按回车键进行数组下落操作...");
			down(star, max);
			combine(star, max);
			clear_arr(book, max);
			put_arr(star, book, max, "下落后的数组：", 0, 1);
			if (check_end(star, max) == 0)
			{
				score(same, get_left(star, max));
				cout << "剩余" << get_left(star, max) << "个不可消除项，本关结束!" << endl;
				break;
			}
			get_enter("本次消除结束，按回车键继续新一次的消除...");
		}
		else
		{
			clear_arr(book, max);
			if (is_e == -1)
				break;
		}
	}
}
