/* 2250951 ¹âµç ³Â÷è°² */
#include "../include/utilities.h"
#include"../include/cmd_console_tools.h"
#include<iostream>
#include<conio.h>
using namespace std;

int menu(void (*draw)(), int max, int is_ab)
{
	draw();
	char ch = 0;
	if (is_ab)
	{
		while (1)
		{
			ch = _getch();
			if (ch >= 'a' && ch <= 'g' || ch == 'q')
				ch -= 'a' - 'A';
			if (ch >= 'a' && ch <= 'g' || ch == 'q')
				ch -= 'a' - 'A';
			if ((ch >= 'A' && ch <= 'G') || ch == 'Q')
			{
				if (ch != 'Q')
				{
					putchar(ch);
				}
				break;
			}
		}
		int order;
		if (ch == 'Q' || ch == 'q')
			order = 0;
		else
			order = ch - 'A' + 1;
		return order;
	}
	else
	{
		while (1)
		{
			ch = _getch();
			if (ch >= '0' && ch <= '0' + max)
				break;
		}
		return (int)(ch - '0');
	}
}

void drawBlockFromCenterPixCoord(struct node center_pix, int bg, int fg)
{
	cct_setcolor(bg, fg);
	cct_gotoxy(center_pix.x - 2, center_pix.y - 1);
	cout << "¨X¨T¨[";
	cct_gotoxy(center_pix.x - 2, center_pix.y + 0);
	cout << "¨U¡ï¨U";
	cct_gotoxy(center_pix.x - 2, center_pix.y + 1);
	cout << "¨^¨T¨a";
	cct_setcolor();
}

void eliminateBlockFromCenterPixCoord(struct node center_pix, int bg, int fg)
{
	cct_setcolor(bg, fg);
	cct_gotoxy(center_pix.x - 2, center_pix.y - 1);
	cout << "      ";
	cct_gotoxy(center_pix.x - 2, center_pix.y + 0);
	cout << "      ";
	cct_gotoxy(center_pix.x - 2, center_pix.y + 1);
	cout << "      ";
	cct_setcolor();
}

struct node operator+(struct node p1, struct node p2)
{
	struct node p;
	p.x = p1.x + p2.x;
	p.y = p1.y + p2.y;
	return p;
}

struct node operator-(struct node p1, struct node p2)
{
	struct node p;
	p.x = p1.x - p2.x;
	p.y = p1.y - p2.y;
	return p;
}