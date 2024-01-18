/* 2250951 光电 陈麒安 */
#include<iostream>
#include<Windows.h>
#include<iomanip>
#include "../include/cmd_console_tools.h"
#include"90-01-b2.h"
using namespace std;
const struct node top_left = { 2,2 };
const struct node block_size = { 6,3 };

struct node get_size(struct node max, int order)
{
	struct node size;
	if (order == 0)
	{
		switch (max.x)
		{
		case 8:
			size.x = 55;
			break;
		case 9:
			size.x = 61;
			break;
		case 10:
			size.x = 67;
			break;
		}
		switch (max.y)
		{
		case 8:
			size.y = 32;
			break;
		case 9:
			size.y = 35;
			break;
		case 10:
			size.y = 38;
			break;
		}
	}
	else
	{
		switch (max.x)
		{
		case 8:
			size.x = 69;
			break;
		case 9:
			size.x = 77;
			break;
		case 10:
			size.x = 85;
			break;
		}
		switch (max.y)
		{
		case 8:
			size.y = 39;
			break;
		case 9:
			size.y = 43;
			break;
		case 10:
			size.y = 47;
			break;
		}
	}

	return size;
}

void trans_size(struct node max,int order)
{
	struct node size = get_size(max, order);
	cct_setconsoleborder(size.x, size.y);
	cout << "屏幕当前设置为" << size.y << "行" << size.x << "列" << endl;
}

void draw_rim(struct node size, struct node origin, int order)
{
	if(order==0)
	{
		for (int i = origin.y; i < size.y - 4; i++)
		{
			for (int j = origin.x; j < size.x - 2; j += 2)
			{
				cct_gotoxy(j, i);
				cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
				if (i == origin.y && j == top_left.x)
					cout << "╔";
				else if (i == origin.y && j == size.x - 3)
					cout << "╗";
				else if (i == size.y - 5 && j == top_left.x)
					cout << "╚";
				else if (i == size.y - 5 && j == size.x - 3)
					cout << "╝";
				else if (j == origin.x || j == size.x - 3)
					cout << "║";
				else if (i == origin.y || i == size.y - 5)
					cout << "═";
				else
					cout << "  ";
				cct_setcolor();
			}
		}
	}
	else
	{
		for (int i = origin.y; i < size.y - 4; i++)
		{
			for (int j = origin.x; j < size.x - 2; j += 2)
			{
				cct_gotoxy(j, i);
				cct_setcolor(COLOR_HWHITE, COLOR_BLACK);

				if (i == origin.y && j == top_left.x)
					cout << "╔";
				else if (i == origin.y && j == size.x - 3)
					cout << "╗";
				else if (i == size.y - 5 && j == top_left.x)
					cout << "╚";
				else if (i == size.y - 5 && j == size.x - 3)
					cout << "╝";
				else if ((j - origin.x) % 8 == 0)
				{
					if (i == origin.y)
						cout << "╦";
					else if (i == size.y - 5)
						cout << "╩";
					else if((i - origin.y) % 4 == 0)
					{
						if (j == origin.x)
							cout << "╠";
						else if (j == size.x - 3)
							cout << "╣";
						else
							cout << "╬";
					}
					else
						cout << "║";
				}
				else if ((i - origin.y) % 4 == 0)
					cout << "═";
				else
					cout << "  ";
				cct_setcolor();
			}
		}
	}
}

void block(struct node origin, int bg, int fg = COLOR_BLACK)
{
	cct_setcolor(bg, fg);
	cct_gotoxy(origin.x, origin.y);
	cout << "╔═╗";
	cct_gotoxy(origin.x, origin.y + 1);
	cout << "║★║";
	cct_gotoxy(origin.x, origin.y + 2);
	cout << "╚═╝";
	cct_setcolor();
}

void draw_bg(int arr[10][10], struct node max,int order)
{
	struct node size = get_size(max, order);
	trans_size(max, order);
	cout << " ";
	for (int i = 0; i < max.x; i++)
	{
		if (order && i)
			cout << "  ";
		cout << "     " << i;
	}
	for (int i = 0; i < max.y; i++)
	{
		cct_gotoxy(0, 4 + (3 + order) * i);
		cout << (char)('A' + i);
	}
	
	draw_rim(size, top_left, order);

	for (int i = 0; i < max.y; i++)
	{
		for (int j = 0; j < max.x; j++)
		{
			struct node tmp;
			tmp.x = top_left.x + 2 + j * (block_size.x + order * 2);
			tmp.y = top_left.y + 1 + i * (block_size.y + order * 1);
			block(tmp, arr[i][j]);
		}
	}
}

struct node mat2co(struct node mat, int order)
{
	struct node tmp;
	tmp.x = top_left.x + 2 + mat.x * (block_size.x + order * 2);
	tmp.y = top_left.y + 1 + mat.y * (block_size.y + order * 1);
	return tmp;
}

struct node co2mat(int x, int y, int order, struct node max)
{
	struct node mat;
	struct node size = get_size(max, order);

	if ((x >= size.x - 3) || (x <= top_left.x) || (y >= size.y - 5) || (y <= top_left.y))
	{
		mat.x = -1;
		mat.y = -1;
		return mat;
	}

	if(order==0)
	{
		if (((x - top_left.x) % block_size.x == 0) || ((y - top_left.y) % block_size.y == 0))
		{
			mat.x = -1;
			mat.y = -1;
			return mat;
		}
		else
		{
			mat.x = (x - top_left.x) / block_size.x;
			mat.y = (y - top_left.y) / block_size.y;
		}
	}
	else
	{
		if ((x - top_left.x) % (block_size.x + 2) == 0 || (y - top_left.y) % (block_size.y+1) == 0)
		{
			mat.x = -1;
			mat.y = -1;
			return mat;
		}
		else
		{
			mat.x = (x - top_left.x) / (block_size.x + 2);
			mat.y = (y - top_left.y) / (block_size.y + 1);
		}
	}

	if (mat.x >= max.x)
		mat.x = -1;

	return mat;
}

struct node op_45(struct node max,int arr[10][10], int order)
{
	struct node former_mat = { 0,0 };
	struct node size = get_size(max, order);
	int X, Y;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	cct_enable_mouse();
	cct_setcursor(CURSOR_INVISIBLE);
	struct node former_co = mat2co(former_mat, order);
	//block(former_co, arr[former_mat.x][former_mat.y], COLOR_HWHITE);
	struct node later_mat{0,0};

	while (loop) {

		int flag = 1;
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		


		if (ret == CCT_MOUSE_EVENT) {
			later_mat = co2mat(X, Y, order, max);
			cct_gotoxy(0, size.y - 4);
			cout << "[当前鼠标] ";
			if (later_mat.x == -1)
			{
				cout << "位置非法";
				block(former_co, arr[former_mat.y][former_mat.x]);
			}
			else
			{
				cout << (char)('A' + later_mat.y) << "行" << later_mat.x << "列  ";
			}


			switch (maction) {
			case MOUSE_LEFT_BUTTON_CLICK:			
			case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//双击左键
			case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
			case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//双击右键
				if (later_mat.x != -1)
					loop = 0;
				break;
			case MOUSE_ONLY_MOVED:
				if (later_mat.x != -1)
					block(mat2co(later_mat, order), arr[later_mat.y][later_mat.x], COLOR_HWHITE);

				if (!(later_mat.x == -1 && former_mat.x != -1))
				{
					former_mat = later_mat;
					former_co = mat2co(former_mat, order);
				}
			}


		}


		else if (ret == CCT_KEYBOARD_EVENT) {

			if (former_mat.x == -1)
				former_mat.x = 0, former_mat.y = 0;

			switch (keycode1) {
			case 224:
				switch (keycode2) {
				case KB_ARROW_UP:
					if (former_mat.y - 1 >= 0)
						later_mat.y = former_mat.y - 1;
					else
						later_mat.y = max.y - 1;
					break;
				case KB_ARROW_DOWN:
					if (former_mat.y + 1 <= max.y - 1)
						later_mat.y = former_mat.y + 1;
					else
						later_mat.y = 0;
					break;
				case KB_ARROW_LEFT:
					if (former_mat.x - 1 >= 0)
						later_mat.x = former_mat.x - 1;
					else
						later_mat.x = max.x - 1;
					break;
				case KB_ARROW_RIGHT:
					if (former_mat.x + 1 <= max.x - 1)
						later_mat.x = former_mat.x + 1;
					else
						later_mat.x = 0;
					break;
				}
				break;
			case 13:
			case 81:
			case 113:
				flag = 0;
			}



			cct_gotoxy(0, size.y - 4);
			cout << "[当前键盘] " << (char)('A' + later_mat.y) << "行" << later_mat.x << "列";

			if (later_mat.x != -1 && former_mat.x != -1)
				block(former_co, arr[former_mat.y][former_mat.x]);

			if (later_mat.x != -1 && former_mat.x != -1)
				block(mat2co(later_mat, order), arr[later_mat.y][later_mat.x], COLOR_HWHITE);

			former_mat = later_mat;
			former_co = mat2co(former_mat, order);
			

		}

		if (flag == 0)
			break;
	}



	cct_disable_mouse();
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	cct_gotoxy(size.x, size.y);
	return later_mat;
}

void showfg(struct node choice, struct node max, int arr[10][10], int book[10][10], int color = COLOR_BLACK)
{
	for (int i = 0; i < max.y; i++)
		for (int j = 0; j < max.x; j++)
		{
			struct node now = { j,i };
			if (book[i][j])
				block(mat2co(now, 0), arr[i][j], color);
		}
}

int inquire(struct node max, struct node now, int order)
{
	struct node size = get_size(max, order);
	cct_gotoxy(0, size.y - 4);
	cout << "箭头键/鼠标移动取消当前选择，回车键/单击左键合成";
	int X, Y;
	int ret, maction;
	int keycode1, keycode2;
	cct_enable_mouse();
	cct_setcursor(CURSOR_INVISIBLE);
	ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
	int flag = 0;
	if (ret == CCT_MOUSE_EVENT&&maction== MOUSE_LEFT_BUTTON_CLICK)
	{
		if (ret == CCT_KEYBOARD_EVENT&& keycode1==13)
		{
			flag = 1;
		}
	}

	cct_disable_mouse();
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	return flag;
}

void elim_gra(int arr[10][10], int book[10][10], struct node max, int order)
{
	for (int i = 0; i < max.y; i++)
	{
		for (int j = 0; j < max.x; j++)
		{
			if (book[i][j])
			{
				arr[i][j] = 0;
				struct node tmp = { j,i };
				struct node origin = mat2co(tmp, order);
				cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
				cct_gotoxy(origin.x, origin.y);
				cout << "      ";
				cct_gotoxy(origin.x, origin.y + 1);
				cout << "      ";
				cct_gotoxy(origin.x, origin.y + 2);
				cout << "      ";
				cct_setcolor();
			}
		}
	}
}

void blank(struct node origin, int order)
{
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	char ch1[] = "      ";
	char ch2[] = "═══";

	cct_gotoxy(origin.x, origin.y);
	if ((origin.y - top_left.y) % 4 == 0 && order)
		cout << ch2;
	else
		cout << ch1;
	cct_gotoxy(origin.x, origin.y + 1);
	cout << ch1;
	cct_gotoxy(origin.x, origin.y + 2);
	cout << ch1;
	cct_setcolor();
}

void fall(struct node src, struct node dst,int color,int order)
{
	int len = (dst.y - src.y) * (block_size.y + order);
	src = mat2co(src, order);
	dst = mat2co(src, order);
	for (int i = 0; i < len;)
	{
		blank(src, order);
		i++, src.y++;
		block(src, color);
		Sleep(10);
	}
}

void col_down_gra(int arr[10][10], struct node max, int col, int order)
{
	for (int lowest = max.y - 1; lowest >= 0; lowest--)
	{
		if (arr[lowest][col] == 0)
		{
			for (int i = lowest; i >= 0; i--)
			{
				if (arr[i][col])
				{
					struct node src = { col,i };
					struct node dst = { col,lowest };
					fall(src, dst, arr[i][col], order);
					swap(arr[i][col], arr[lowest][col]);
					break;
				}
			}
		}
	}
}

void down_gra(int arr[10][10], struct node max, int order)
{
	for (int i = 0; i < max.y - 1; i++)
	{
		for (int j = 0; j < max.x; j++)
		{
			if (arr[i][j] != 0 && arr[i + 1][j] == 0)
			{
				col_down_gra(arr, max, j, order);
			}
		}
	}
}

void op_6(int arr[10][10], int book[10][10], struct node max)
{
	while (1)
	{
		struct node choice = op_45(max, arr, 0);
		int same = bfs(arr, book, choice, max);
		showfg(choice, max, arr, book, COLOR_HWHITE);
		if (inquire)
		{
			eliminate(arr, book, max);
			elim_gra(arr, book, max, 0);
			cct_gotoxy(0, get_size(max, 0).y - 4);
			cout << "合成完成，单击左键/回车下落";
			cct_gotoxy(0, 0);
			score(same);
			down_gra(arr, max, 0);
			break;
		}
		else
		{
			showfg(choice, max, arr, book);
			continue;
		}
		clear_arr(book, max);
	}
	cct_gotoxy(get_size(max, 0).x, get_size(max, 0).y);
}

void shift(struct node src,  int color)
{
	src = mat2co(src, 1);
	cct_gotoxy(src.x, src.y);
	blank(src, 0);
	src.x += 2;
	cct_gotoxy(src.x, src.y);
	block(src, color);
	cct_gotoxy(src.x, src.y);
	blank(src, 0);
	src.x += 2;
	cct_gotoxy(src.x, src.y);
	block(src, color);
	cct_gotoxy(src.x, src.y);
	blank(src, 0);
	src.x += 2;
	cct_gotoxy(src.x, src.y);
	block(src, color);
	cct_gotoxy(src.x, src.y);
	{
		blank(src, 0);
		struct node tmp = src;
		cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
		cct_gotoxy(tmp.x, tmp.y++);
		cout << "║";
		cct_gotoxy(tmp.x, tmp.y++);
		cout << "║";
		cct_gotoxy(tmp.x, tmp.y++);
		cout << "║";
	}
	src.x += 2;
	cct_gotoxy(src.x, src.y);
	block(src, color);
}

void combine_gra(int arr[10][10], struct node max)
{
	for (int i = 0; i < max.x; i++)
	{
		int left = 0;
		for (; col_vac(arr, left, max) == 0; left++)
			;

		if (col_vac(arr, i, max) == 0 && i != 0)
		{
			for (int j = 0; j < max.y; j++)
			{
				struct node src = { i - 1,j };
				shift(src, arr[j][i - 1]);
				swap(arr[j][i], arr[j][i - 1]);
			}
			i = left;
		}
	}
}

void op_7(int star[10][10], int book[10][10], struct node max)
{

	while (1)
	{
		while(1)
		{
			struct node choice = op_45(max, star, 1);
			int same = bfs(star, book, choice, max);
			showfg(choice, max, star, book, COLOR_HWHITE);
			if (inquire)
			{
				eliminate(star, book, max);
				elim_gra(star, book, max, 1);
				combine_gra(star, max);
				cct_gotoxy(0, get_size(max, 0).y - 4);
				cout << "合成完成，单击左键/回车下落";
				cct_gotoxy(0, 0);
				score(same);
				down_gra(star, max, 1);
			}
			else
			{
				showfg(choice, max, star, book);
			}
			clear_arr(book, max);
			if (check_end(star, max) == 0)
			{
				score(same, get_left(star, max));
				cout << "剩余" << get_left(star, max) << "个不可消除项，本关结束!" << endl;
				break;
			}
		}
		get_enter("本次消除结束，按回车键继续新一次的消除...");
	}
}