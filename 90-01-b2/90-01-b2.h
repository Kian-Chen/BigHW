/* 2250951 ¹âµç ³Â÷è°² */
#include "../include/utilities.h"
#pragma once
void randomset(int arr[10][10], struct node max);
void put_arr(int arr[10][10], int book[10][10], struct node max, const char* const prompt, const char ch = 0, int all = 0);
void clear_arr(int arr[10][10], struct node max);
int check_point(int arr[10][10], struct node point, struct node max);
struct node get_cmd(int arr[10][10], struct node max);
int bfs(int arr[10][10], int book[10][10], struct node point, struct node max);
int inquire_cmd(struct node now);
void eliminate(int arr[10][10], int book[10][10], struct node max);
void score(int same = 0, int num = 1000);
void down(int arr[10][10], struct node max);
int check_end(int arr[10][10], struct node max);
void get_enter(const char* const prompt);
void combine(int arr[10][10], struct node max);
void end(struct node max, int order);
int get_left(int arr[10][10], struct node max);
void trans_size(struct node max, int order=1);
void option_1(int star[10][10], int book[10][10], struct node max);
void option_2(int star[10][10], int book[10][10], struct node max);
void option_3(int star[10][10], int book[10][10], struct node max);
void draw_bg(int arr[10][10], struct node max, int order = 1);
struct node op_45(struct node max, int arr[10][10], int order = 1);
struct node get_size(struct node max, int order);
void op_6(int arr[10][10], int book[10][10], struct node max);
void col_down(int arr[10][10], struct node max, int col);
void swap(int& x, int& y);
void op_7(int star[10][10], int book[10][10], struct node max);
int col_vac(int arr[10][10], int  col, struct node max);