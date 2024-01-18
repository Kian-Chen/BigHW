/* 2250951 ¹âµç ³Â÷è°² */
#pragma once

#define LEFTKEY 75
#define UPKEY 72
#define RIGHTKEY 77
#define DOWNKEY 80
typedef void (*ShapeFunction)(struct node&, int[5][5]);

const int number[10][5][5] =
{
	{
		0,0,0,0,0,
		1,1,1,1,1,
		1,0,0,0,1,
		1,1,1,1,1,
		0,0,0,0,0,
	},
	{
		0,0,0,0,0,
		0,0,0,0,0,
		1,1,1,1,1,
		0,0,0,0,0,
		0,0,0,0,0,
	},
	{
		0,0,0,0,0,
		1,0,1,1,1,
		1,0,1,0,1,
		1,1,1,0,1,
		0,0,0,0,0,
	},
	{
		0,0,0,0,0,
		1,0,1,0,1,
		1,0,1,0,1,
		1,1,1,1,1,
		0,0,0,0,0,
	},
	{
		0,0,0,0,0,
		1,1,1,0,0,
		0,0,1,0,0,
		1,1,1,1,1,
		0,0,0,0,0,
	},
	{
		0,0,0,0,0,
		1,1,1,0,1,
		1,0,1,0,1,
		1,0,1,1,1,
		0,0,0,0,0,
	},
	{
		0,0,0,0,0,
		1,1,1,1,1,
		1,0,1,0,1,
		1,0,1,1,1,
		0,0,0,0,0,
	},
	{
		0,0,0,0,0,
		1,0,0,0,0,
		1,0,0,0,0,
		1,1,1,1,1,
		0,0,0,0,0,
	},
	{
		0,0,0,0,0,
		1,1,1,1,1,
		1,0,1,0,1,
		1,1,1,1,1,
		0,0,0,0,0,
	},
	{
		0,0,0,0,0,
		1,1,1,0,1,
		1,0,1,0,1,
		1,1,1,1,1,
		0,0,0,0,0,
	}
};
void copyTwoShape(int copy[5][5], const int origin[5][5]);
void background(struct node max_phy);
int getNextNum(bool is_now = true, bool is_update = false, const bool new_seed = false, const unsigned int seed = 0);
struct node transPhyCenterToPix(struct node block_phy);
void transposeNumPhyShape(int num_shape[5][5]);
void antiClockNumPhyShape(int num_shape[5][5]);
void eliminateNumBookFromCenter(struct node center_phy, int num_shape[5][5], int board_book_phy[21][26]);
void drawNumBookFromCenter(struct node center_phy, int num_shape[5][5], int board_book_phy[21][26]);
void oneStepDown(struct node& center_phy, int num_shape[5][5]);
void oneStepLeft(struct node& center_phy, int num_shape[5][5]);
void oneStepRight(struct node& center_phy, int num_shape[5][5]);
void oneStepAntiClock(struct node& center_phy, int num_shape[5][5]);
void oneStepDownInverse(struct node& center_phy, int num_shape[5][5]);
void oneStepLeftInverse(struct node& center_phy, int num_shape[5][5]);
void oneStepRightInverse(struct node& center_phy, int num_shape[5][5]);
void oneStepAntiClockInverse(struct node& center_phy, int num_shape[5][5]);
void oneStepNull(struct node& center_phy, int num_shape[5][5]);
int oneBlockFall(
	int num,
	int board_book_phy[21][26],
	struct node max_of_board_phy,
	int order

);
bool judgeContourIegality(struct node center_phy, int num_shape[5][5], struct node max_of_board_phy);
bool judgeBoardBookIegality(
	struct node center_phy,
	int num_shape[5][5],
	int board_book_without_last[21][26],
	struct node max_of_board_phy
);
bool judgeActionIegality(
	struct node center_phy,
	int board_book_phy[21][26],
	struct node max_of_board_phy,
	int num_shape_phy[5][5],
	void (*oneStepAction)(
		struct node& center,
		int num_shape[5][5]
		)
);

bool compareShape(int first_shape[5][5], int second_shape[5][5]);
void setTitleMsg();
void waitEnter();
int getSleep();
ShapeFunction getInverse(void (*action)(
	struct node& center,
	int num_shape[5][5]
	)
);
ShapeFunction getAction(
	int key,
	void (*down)(
		struct node& center,
		int num_shape[5][5]
		) = oneStepNull,
	void (*left)(
		struct node& center,
		int num_shape[5][5]
		) = oneStepNull,
	void (*right)(
		struct node& center,
		int num_shape[5][5]
		) = oneStepNull,
	void (*rotate)(
		struct node& center,
		int num_shape[5][5]
		) = oneStepNull
);
int checkIfEliminate(int board_book[21][26], struct node max_of_board_phy);
void eliminateLine(int board_book[21][26], struct node max_of_board_phy);
void drawBoard(int board_book[21][26], struct node max_of_board_phy);
int interScore(bool is_score = true, int full_line = 0);