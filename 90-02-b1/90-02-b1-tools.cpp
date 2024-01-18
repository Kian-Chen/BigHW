/* 2250951 光电 陈麒安 */
#include<iostream>
#include<Windows.h>
#include <time.h>
#include <random>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include"../include/utilities.h"
#include"90-02-b1.h"
using namespace std;


void copyTwoShape(int copy[5][5], const int origin[5][5])
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			copy[i][j] = origin[i][j];
}

int getNextNum(bool is_now, bool is_update, const bool new_seed, const unsigned int seed)
{
	static int now, next, i = 0;
	if (new_seed)
		srand(seed);
	if (i == 0)
	{
		now = rand() % 10;
		next = rand() % 10;
	}
	if (is_update == true)
	{
		now = next;
		next = rand() % 10;
	}
	i++;

	if (is_now == true)
	{
		return now;
	}
	else
	{
		return next;
	}
}

struct node getPixFromPhy(struct node max_phy)
{
	struct node max_pix;
	max_pix.x = max_phy.x * 6;
	max_pix.y = max_phy.y * 3 + 3;
	return max_pix;
}

struct node transPhyCenterToPix(struct node block_phy)
{
	struct node block_pix;
	block_pix.x = block_phy.x * 6 + 2;
	block_pix.y = block_phy.y * 3 + 1;
	return block_pix;
}

void background(struct node max_phy)
{
	struct node max_pix = getPixFromPhy(max_phy);
	cct_setconsoleborder(max_pix.x, max_pix.y);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);

	for (int i = 0; i < max_phy.y * 3; i++)
	{
		for (int j = 0; j < max_phy.x * 6; j++)
		{
			cout << " ";
		}
	}
	cct_setcolor();

}

void swapNumber(int* first, int* second)
{
	int temp = *first;
	*first = *second;
	*second = temp;
}

void transposeNumPhyShape(int num_shape[5][5])
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = i; j < 5; j++)
		{
			swapNumber(&num_shape[i][j], &num_shape[j][i]);
		}
	}
}

void antiClockNumPhyShape(int num_shape[5][5])
{
	int temp[5][5] = { 0,0 };

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			temp[i][j] = num_shape[j][4 - i];
		}
	}

	copyTwoShape(num_shape, temp);

}

void eliminateNumBookFromCenter(struct node center_phy, int num_shape[5][5], int board_book_phy[21][26])
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (num_shape[i][j] != 0)
			{
				board_book_phy[center_phy.x + i - 2][center_phy.y + j - 2] = 0;
			}
		}
	}
}

void drawNumBookFromCenter(struct node center_phy, int num_shape[5][5], int board_book_phy[21][26])
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (num_shape[i][j] != 0)
			{
				board_book_phy[center_phy.x + i - 2][center_phy.y + j - 2] = 1;
			}
		}
	}
}

void oneStepDown(struct node& center_phy, int num_shape[5][5])
{
	center_phy.y++;
}

void oneStepLeft(struct node& center_phy, int num_shape[5][5])
{
	center_phy.x--;
}

void oneStepRight(struct node& center_phy, int num_shape[5][5])
{
	center_phy.x++;
}

void oneStepAntiClock(struct node& center_phy, int num_shape[5][5])
{
	antiClockNumPhyShape(num_shape);
}

void oneStepDownInverse(struct node& center_phy, int num_shape[5][5])
{
	center_phy.y--;
}

void oneStepLeftInverse(struct node& center_phy, int num_shape[5][5])
{
	center_phy.x++;
}

void oneStepRightInverse(struct node& center_phy, int num_shape[5][5])
{
	center_phy.x--;
}

void oneStepAntiClockInverse(struct node& center_phy, int num_shape[5][5])
{
	antiClockNumPhyShape(num_shape);
	antiClockNumPhyShape(num_shape);
	antiClockNumPhyShape(num_shape);

}

void oneStepNull(struct node& center_phy, int num_shape[5][5])
{
	;
}

bool judgeContourIegality(struct node center_phy, int num_shape[5][5], struct node max_of_board_phy)
{
	bool is_iegal = true;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (num_shape[i][j] != 0)
			{
				if ((center_phy.x + i - 2) > max_of_board_phy.x || (center_phy.x + i - 2) < 0)
				{
					is_iegal = false;
					return is_iegal;
				}
				if ((center_phy.y + j - 2) > max_of_board_phy.y)
				{
					is_iegal = false;
					return is_iegal;
				}
			}
		}
	}
	return is_iegal;
}

bool judgeBoardBookIegality(
	struct node center_phy,
	int num_shape[5][5],
	int board_book_without_last[21][26],
	struct node max_of_board_phy
)
{
	bool is_iegal = true;
	if (judgeContourIegality(center_phy, num_shape, max_of_board_phy) == false)
	{
		is_iegal = false;
		return is_iegal;
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (num_shape[i][j] == 1 && board_book_without_last[center_phy.x + i - 2][center_phy.y + j - 2])
			{
				is_iegal = false;
				return is_iegal;
			}
		}
	}
	return is_iegal;
}

bool judgeActionIegality(
	struct node center_phy,
	int board_book_phy[21][26],
	struct node max_of_board_phy,
	int num_shape_phy[5][5],
	void (*oneStepAction)(
		struct node& center,
		int num_shape[5][5]
		)
)
{
	int temp_board_book_phy[21][26] = { 0,0 };
	int temp_num_shape_phy[5][5] = { 0,0 };
	struct node temp_center_phy = center_phy;

	for (int i = 0; i < max_of_board_phy.x; i++)
	{
		for (int j = 0; j < max_of_board_phy.y; j++)
		{
			temp_board_book_phy[i][j] = board_book_phy[i][j];
		}
	}
	copyTwoShape(temp_num_shape_phy, num_shape_phy);

	eliminateNumBookFromCenter(temp_center_phy, temp_num_shape_phy, temp_board_book_phy);
	oneStepAction(temp_center_phy, temp_num_shape_phy);

	return judgeBoardBookIegality(temp_center_phy, temp_num_shape_phy, temp_board_book_phy, max_of_board_phy);
}

void eliminateLine(int board_book[21][26], struct node max_of_board_phy)
{
	int temp[21][26] = { 0,0 };
	for (int i = 0; i < max_of_board_phy.x; i++)
	{
		for (int j = 0; j < max_of_board_phy.y - 1; j++)
		{
			temp[i][j + 1] = board_book[i][j];
		}
	}

	for (int i = 0; i < max_of_board_phy.x; i++)
	{
		for (int j = 0; j < max_of_board_phy.y; j++)
		{
			board_book[i][j] = temp[i][j];
		}
	}
}

int checkIfEliminate(int board_book[21][26], struct node max_of_board_phy)
{
	int is_complete = 0;

	for (int y = max_of_board_phy.y - 1; y > 0; y--)
	{
		int sum = 0;
		for (int x = 0; x < max_of_board_phy.x; x++)
		{
			sum += board_book[x][y];
		}
		if (sum == max_of_board_phy.x)
		{
			is_complete++;
		}
		else
		{
			break;
		}
	}

	return is_complete;
}


int interScore(bool is_score, int full_line)
{
	static int score = 0, line = 0;
	line += full_line;
	switch (full_line)
	{
	case 0:
		score += 0;
		break;
	case 1:
		score += 1;
		break;
	case 2:
		score += 3;
		break;
	case 3:
		score += 6;
		break;
	case 4:
		score += 10;
		break;
	case 5:
		score += 15;
		break;
	default:
		break;
	}
	if (is_score == true)
	{
		return score;
	}
	else
	{
		return line;
	}
}

int getSleep()
{
	int score = interScore();

	if (score >= 120)
	{
		return 300;
	}
	else
	{
		return 1000 - 100 * (score / 15);
	}
}

bool compareShape(int first_shape[5][5], int second_shape[5][5])
{
	bool is_same = true;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (first_shape[i][j] != second_shape[i][j])
			{
				is_same = false;
				return is_same;
			}
		}
	}
	return is_same;
}

void setTitleMsg()
{
	int nextNum = getNextNum(false);
	int line = interScore(false);
	int score = interScore();

	char title[256] = { 0 };

	strcat_s(title, "下一个数字: ");
	char nextNumStr[10];
	sprintf_s(nextNumStr, "%d", nextNum);
	strcat_s(title, nextNumStr);
	strcat_s(title, ", 已消除行数: ");
	char lineStr[10];
	sprintf_s(lineStr, "%d", line);
	strcat_s(title, lineStr);
	strcat_s(title, ", 当前得分: ");
	char scoreStr[10];
	sprintf_s(scoreStr, "%d", score);
	strcat_s(title, scoreStr);

	cct_setconsoletitle(title);
}

void waitEnter()
{
	cct_setconsoletitle("本回合结束，请按回车键退出");
	while (1)
		if (_getch() == '\r')
			break;
}

bool checkOver(int board_book_phy[21][26], struct node max_of_board_phy)
{
	bool is_over = false;
	return false;
}

ShapeFunction getAction(
	int key,
	void (*down)(
		struct node& center,
		int num_shape[5][5]
		),
	void (*left)(
		struct node& center,
		int num_shape[5][5]
		),
	void (*right)(
		struct node& center,
		int num_shape[5][5]
		),
	void (*rotate)(
		struct node& center,
		int num_shape[5][5]
		))
{
	switch (key)
	{
	case LEFTKEY:
		return oneStepLeft;
	case RIGHTKEY:
		return oneStepRight;
	case UPKEY:
		return oneStepAntiClock;
	case DOWNKEY:
		return oneStepDown;
	default:
		return oneStepNull;
		break;
	}
}

ShapeFunction getInverse(void (*action)(
	struct node& center,
	int num_shape[5][5]
	))
{
	if (action == oneStepDown)
		return oneStepDownInverse;
	else if (action == oneStepLeft)
		return oneStepLeftInverse;
	else if (action == oneStepRight)
		return oneStepRightInverse;
	else if (action == oneStepAntiClock)
		return oneStepAntiClockInverse;
	else
		return oneStepNull;
}
