/* 2250951 ¹âµç ³Â÷è°² */
#include<iostream>
#include<Windows.h>
#include <time.h>
#include <random>
#include<conio.h>
#include "../include/cmd_console_tools.h"
#include"../include/utilities.h"
#include"90-02-b1.h"
using namespace std;

struct node getPhyStartPoint(struct node max_phy)
{
	struct node start_point_phy;
	start_point_phy.x = max_phy.x / 2;
	start_point_phy.y = 2;
	return start_point_phy;
}

void getNumShape(int num_shape[5][5], int num)
{
	copyTwoShape(num_shape, number[num]);
}

void drawShapeFromPhyCenter(int shape[5][5], struct node center_phy, bool isEliminate = false)
{
	for (int i = -2; i < 3; i++)
	{
		for (int j = -2; j < 3; j++)
		{
			if (shape[i + 2][j + 2])
			{
				struct node temp_coord_phy;
				temp_coord_phy.x = center_phy.x + i;
				temp_coord_phy.y = center_phy.y + j;
				if (isEliminate == false)
				{
					drawBlockFromCenterPixCoord((temp_coord_phy));
				}
				else
				{
					eliminateBlockFromCenterPixCoord(transPhyCenterToPix(temp_coord_phy));
				}
			}
		}
	}
}

void drawBoard(int board_book[21][26], struct node max_of_board_phy)
{
	for (int i = 0; i < max_of_board_phy.x; i++)
	{
		for (int j = 0; j < max_of_board_phy.y; j++)
		{
			struct node temp_coord_phy;
			temp_coord_phy.x = i;
			temp_coord_phy.y = j;
			if(board_book[i][j])
				drawBlockFromCenterPixCoord(transPhyCenterToPix(temp_coord_phy));
		}
	}
}

void amendKey(char& key, int order)
{
	if (key == DOWNKEY)
		key = 0;
	switch (order)
	{
	case 1:
		key = 0;
		break;
	case 2:
		if (key == LEFTKEY || key == RIGHTKEY)
			key = 0;
	default:
		break;
	}
}

int oneBlockFall(
	int num,
	int board_book_phy[21][26],
	struct node max_of_board_phy,
	int order
) 
{
	struct node start_point_phy = getPhyStartPoint(max_of_board_phy);
	int num_shape[5][5] = { 0,0 };
	getNumShape(num_shape, num);

	if (judgeBoardBookIegality(start_point_phy, num_shape, board_book_phy, max_of_board_phy) == false)
	{
		cct_setconsoletitle("GameOver");
		return 0;
	}

	struct node temp_point_phy = start_point_phy;
	ShapeFunction action = oneStepNull;
	ShapeFunction action_last = action;
	while (judgeActionIegality(temp_point_phy, board_book_phy, max_of_board_phy, num_shape, oneStepDown))
	{
		setTitleMsg();
		struct node last_point_phy = temp_point_phy;
		int temp_num_shape[5][5] = { 0,0 };
		copyTwoShape(temp_num_shape, num_shape);

		if (_kbhit()) {
			char key = _getch();
			key = _getch();
			while (_kbhit()) 
			{
				_getch();
			}
			amendKey(key, order);
			action = getAction(key);
		}
		else
			action = oneStepNull;

		if ((last_point_phy.x != start_point_phy.x || last_point_phy.y != start_point_phy.y) && compareShape(temp_num_shape, num_shape))
		{
			oneStepDownInverse(last_point_phy, temp_num_shape);
			drawShapeFromPhyCenter(temp_num_shape, last_point_phy, true);
		}
		if (judgeActionIegality(temp_point_phy, board_book_phy, max_of_board_phy, num_shape, action))
			action(temp_point_phy, num_shape);
		if (judgeActionIegality(temp_point_phy, board_book_phy, max_of_board_phy, num_shape, action_last))
		{
			getInverse(action_last)(last_point_phy, temp_num_shape);
			drawShapeFromPhyCenter(temp_num_shape, last_point_phy, true);
		}

		drawShapeFromPhyCenter(num_shape, temp_point_phy);
		Sleep(getSleep());
		if (judgeActionIegality(temp_point_phy, board_book_phy, max_of_board_phy, num_shape, oneStepDown))
			oneStepDown(temp_point_phy, num_shape);

		action_last = action;
	}
	drawNumBookFromCenter(temp_point_phy, num_shape, board_book_phy);
	return 1;
}
