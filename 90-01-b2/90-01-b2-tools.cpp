/* 2250951 光电 陈麒安 */
#include<iostream>
#include<Windows.h>
#include <time.h>
#include <random>
#include "../include/cmd_console_tools.h"
#include"90-01-b2.h"
using namespace std;

void end(struct node max, int order)
{
	struct node size = get_size(max, order);
	cct_gotoxy(0, size.y - 2);
	cout << "本小题结束，请输入End继续…" << endl;
	while (1) {
		char tmp[1000] = { 0 };
		cin >> tmp;
		char ch1 = tmp[0], ch2 = tmp[1], ch3 = tmp[2];
		if (ch1 >= 'a' && ch1 <= 'z')
			ch1 -= 'a' - 'A';
		if (ch2 >= 'a' && ch2 <= 'z')
			ch2 -= 'a' - 'A';
		if (ch3 >= 'a' && ch3 <= 'z')
			ch3 -= 'a' - 'A';
		if (ch1=='E' && ch2=='N' && ch3=='D')
			break;
		cin.clear();
		cin.ignore(114514, '\n');
		cout << "输入错误，请重新输入 ：  " << endl;
	}
	cct_cls();

}

void clear_arr(int arr[10][10], struct node max)
{
	for (int i = 0; i < max.y; i++)
		for (int j = 0; j < max.x; j++)
			arr[i][j] = 0;
}

void randomset(int arr[10][10], struct node max)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < max.y; i++)
		for (int j = 0; j < max.x; j++)
			arr[i][j] = rand() % 5 + 1;
}

int check_point(int arr[10][10], struct node point, struct node max)
{
	int same = 0;
	if (point.x - 1 >= 0)
		if (arr[point.y][point.x] == arr[point.y][point.x - 1])
			same++;
	if (point.y - 1 >= 0)
		if (arr[point.y][point.x] == arr[point.y - 1][point.x])
			same++;
	if (point.x + 1 <= max.x)
		if (arr[point.y][point.x] == arr[point.y][point.x + 1])
			same++;
	if (point.y + 1 <= max.y)
		if (arr[point.y][point.x] == arr[point.y + 1][point.x])
			same++;
	return same ? 1 : 0;
}

int bfs(int arr[10][10], int book[10][10], struct node point, struct node max)
{
	struct node que[105];
	int next[4][2] = { {1,0},//往右走
		{0,1},//往下走
		{-1,0},//往左走
		{0,-1} };//往上走
	int head = 1, tail = 1;
	que[tail].x = point.x;
	que[tail].y = point.y;
	tail++;
	int tx, ty, same = 0;
	while (head < tail)
	{
		for (int i = 0; i <= 3; i++)
		{
			tx = que[head].x + next[i][0];
			ty = que[head].y + next[i][1];
			if (tx < 0 || tx>max.x || ty < 0 || ty>max.y)
				continue;
			if (book[ty][tx] == 0)
			{
				book[ty][tx] = 1;
				if (arr[ty][tx] == arr[point.y][point.x])
				{
					book[ty][tx] = 2;
					same++;
					que[tail].x = tx;
					que[tail].y = ty;
					tail++;
				}
			}
		}
		head++;
	}

	for (int i = 0; i < max.y; i++)
		for (int j = 0; j < max.x; j++)
			if (book[i][j])
				book[i][j]--;
	return same;
}

void swap(int& x, int& y)
{
	int t = x;
	x = y, y = t;
}

int col_vac(int arr[10][10], int  col, struct node max)
{
	int flag = 0;
	for (int j = 0; j < max.y; j++)
	{
		if (arr[j][col])
		{
			flag = 1;
			break;
		}
	}
	return flag;
}

void combine(int arr[10][10], struct node max)
{
	for (int i = 0; i < max.x; i++)
	{
		int left = 0;
		for (; col_vac(arr, left, max) == 0; left++)
			;

		if (col_vac(arr, i, max) == 0 && i != 0)
		{
			for (int j = 0; j < max.y; j++)
				swap(arr[j][i], arr[j][i - 1]);
			i = left;
		}
	}
}

void eliminate(int arr[10][10], int book[10][10], struct node max)
{
	for (int i = 0; i < max.y; i++)
	{
		for (int j = 0; j < max.x; j++)
		{
			if (book[i][j])
				arr[i][j] = 0;
		}
	}
}

void score(int same,int num)
{
	static int total = 0;
	if (same == 0)
	{
		total = 0;
		return;
	}
	int one_score = same * same * 5;
	if (num < 10)
		total += (10 - num) * 180;
	total += one_score;
	cout << "本次得分：" << one_score << " 总得分： " << total << "         " << endl;
}

void col_down(int arr[10][10], struct node max, int col)
{
	for (int lowest = max.y-1; lowest >= 0; lowest--)
	{
		if (arr[lowest][col] == 0)
		{
			for (int i = lowest; i >= 0; i--)
			{
				if (arr[i][col])
				{
					swap(arr[i][col], arr[lowest][col]);
					break;
				}
			}
		}
	}
}

void down(int arr[10][10], struct node max)
{
	for (int i = 0; i < max.y - 1; i++)
	{
		for (int j = 0; j < max.x; j++)
		{
			if (arr[i][j] != 0 && arr[i + 1][j] == 0)
				col_down(arr,max, j);
		}
	}
}

int check_end(int arr[10][10], struct node max)
{
	int flag = 0;
	for (int i = 0; i < max.y; i++)
	{
		for (int j = 0; j < max.x; j++)
		{
			struct node now;
			now.x = j, now.y = i;
			if (arr[i][j])
				flag = check_point(arr, now, max);
			if (flag)
				break;
		}
		if (flag)
			break;
	}
	return flag;
}

int get_left(int arr[10][10], struct node max)
{
	int left = 0;
	for (int i = 0; i < max.y; i++)
		for (int j = 0; j < max.x; j++)
			if (arr[i][j])
				left++;
	return left;
}