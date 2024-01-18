/* 2250951 光电 陈麒安 */
#include "../include/cmd_console_tools.h"
#include<Windows.h>
#include<time.h>
#include<iostream>
#include<conio.h>
#include <iomanip>
using namespace std;

extern int indi;
extern int i;
extern int stack[3][11], top[3];
#define indi_t indi%10
#define indi_s indi/10
/* ----------------------------------------------------------------------------------

	 本文件功能：
	1、存放被 hanoi_main.cpp 中根据菜单返回值调用的各菜单项对应的执行函数

	 本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */

void get_hanoi(int data[])
{
    int n;
    while (1) {
        cout << "请输入汉诺塔的层数(1-10)" << endl;
        cin >> n;
        if (n >= 1 && n <= 10 && cin.good() == 1)
            break;
        if (cin.good() == 0)
        {
            cin.clear();//cout << cin.good() << endl;
            cin.ignore(114514, '\n');
        }
    }

    char src, tmp, dst;
    while (1) {
        cout << "请输入起始柱(A-C)" << endl;
        cin >> src;
        if (src >= 97 && src <= 99)
            src -= 97 - 65;
        if ((src >= 65 && src <= 67) && cin.good() == 1)
            break;
        if (cin.good() == 0)
        {
            cin.clear();//cout << cin.good() << endl;
            cin.ignore(114514, '\n');
        }
    }
    while (1) {
        cout << "请输入目标柱(A-C)" << endl;
        cin >> dst;
        if (dst >= 97 && dst <= 99)
            dst -= 97 - 65;
        if ((dst >= 65 && dst <= 67) && cin.good() == 1 && dst != src)
            break;
        if (dst == src)
            cout << "目标柱(" << src << ")不能与起始柱(" << src << ")相同" << endl;
        if (cin.good() == 0)
        {
            cin.clear();//cout << cin.good() << endl;
            cin.ignore(114514, '\n');
        }
    }
    tmp = 65 + 66 + 67 - src - dst;
    data[0] = src, data[1] = dst, data[2] = tmp, data[3] = n;
    for (int i = n; i > 0; i--)
    {
        stack[src - 65][top[src - 65]] = i;
        top[src - 65]++;
    }
}

void get_indi()
{
 
    int ind_t, ind_s;
    while (1) {
        cout << "请输入移动速度(0-5: 0-按回车单步演示 1-延时最长 5-延时最短)" << endl;
        cin >> ind_t;
        if ((ind_t >= 0 && ind_t <= 5) && cin.good() == 1)
            break;
        if (cin.good() == 0)
        {
            cin.clear();//cout << cin.good() << endl;
            cin.ignore(114514, '\n');
        }
    }

    cin.clear();//cout << cin.good() << endl;
    cin.ignore(114514, '\n');

    while (1) {
        cout << "请输入是否显示内部数组值(0-不显示 1-显示)" << endl;
        cin >> ind_s;
        if ((ind_s == 0 || ind_s == 1) && cin.good() == 1)
            break;
        if (cin.good() == 0)
        {
            cin.clear();//cout << cin.good() << endl;
            cin.ignore(114514, '\n');
        }
    }

    indi = ind_t + 10 * ind_s;
}

void stack_move(int src,int dst)
{
    stack[dst - 65][top[dst - 65]] = stack[src - 65][top[src - 65] - 1];
    top[src - 65]--;
    top[dst - 65]++;
}

DWORD my_get_time()
{
    DWORD t;
    switch (indi_t)
    {
    case 1:
        t = 10000;
    case 2:
        t = 7500;
    case 3:
        t = 500;
    case 4:
        t = 200;
    default:
        t = 50;
    }
    return t;
}

static void to_be_continued(const char* prompt, const int X = 0, const int Y = 36)
{
	cct_setcolor(); //恢复缺省颜色
	cct_gotoxy(X, Y);

	cout << prompt;

	/* 忽略除回车键外的所有输入（注意：_getch的回车是\r，而getchar是\n）*/
	while (_getch() != '\r')
		;

	cct_cls();

	return;
}

void pillar()
{
	cct_cls();
	const int bg_color = COLOR_HYELLOW;	//背景为亮黄色
	const int fg_color = COLOR_HBLUE;		//前景为亮蓝色
	const char ch = ' ';

	for (int i = 0; i < 3; i++)
		cct_showch(1 + 34 * i, 16, ch, bg_color, fg_color, 24);

	for (int j = 16; j > 2; j--)
	{
		for(int i = 0; i < 3; i++)
		{
			cct_showch(13 + 34 * i, j, ch, bg_color, fg_color, 1);
			Sleep(25);
		}
		Sleep(25);
	}

}

void ini_plate(int src,int dst,int n)
{
    pillar();
    const char ch = ' ';
    int fg_color, bg_color;
    cct_gotoxy(0, 0);
    cct_setcolor();
    cout << "从 " << (char)(src) << " 移动到 " << (char)(dst) << "，共" << n << "层" << endl;

    for (int i = 0; i < 3; i++)
    {
        for(int j=0;j<top[i];j++)
        {
            fg_color = stack[i][top[i]];
            bg_color = stack[i][top[i]];
            while(!bg_color)
                bg_color = rand() % 16;
            cct_showch(13 + 34 * i - (top[i] - j), 15 - j, ch, bg_color, fg_color, 2 * (top[i] - j) + 1);
            Sleep(25);
        }
    }
}

void move_plate(int src, int dst, int n)
{
    const char ch = ' ';        //字符为*(注意：如果是空格，则前景色无法显示)
    int len = stack[src - 65][top[src - 65] - 1] * 2 + 1; 

    int x = 13 + 34 * (src - 65), y = 16 - top[src - 65];
    x -= stack[src - 65][top[src - 65] - 1];
    int bg_color = stack[src - 65][top[src - 65] - 1], fg_color = stack[src - 65][top[src - 65] - 1];

    for (y= 16 - top[src - 65]; y >0; y--) {
        cct_showstr(x, y, " ", bg_color, fg_color, len); 
        Sleep(my_get_time());

        if (y > 1) {
            /* 清除显示(最后一次保留)，清除方法为用正常颜色+空格重画一遍刚才的位置 */
            cct_showstr(x, y, " ", COLOR_BLACK, COLOR_WHITE, len);//cct_setcolor(); cout << len;
            if (y > 2)
                cct_showch(x + stack[src - 65][top[src - 65] - 1], y, ch, COLOR_HYELLOW, COLOR_HBLUE, 1);
        }
    } //end of for
    y++;

    int dst_x = 13 + 34 * (dst - 65);
    dst_x -= stack[src - 65][top[src - 65] - 1] - 1;
    if (x < dst_x)
    {
        for (; x < dst_x; x++) {
            cct_showstr(x, y, " ", bg_color, fg_color, len);

            Sleep(my_get_time());

            if (x < dst_x - 1) {
                /* 清除显示(最后一次保留)，清除方法为用正常颜色+空格重画一遍刚才的位置 */
                cct_showch(x, y, ' ', COLOR_BLACK, COLOR_WHITE, len);
            }
        } //end of for
        x--;
    }
    else
    {
        dst_x -= 2;
        for (; x > dst_x; x--) {
            cct_showstr(x, y, " ", bg_color, fg_color, len);

            Sleep(25);

            if (x > dst_x + 1) {
                /* 清除显示(最后一次保留)，清除方法为用正常颜色+空格重画一遍刚才的位置 */
                cct_showch(x, y, ' ', COLOR_BLACK, COLOR_WHITE, len);
            }
        } //end of for
        x++;
    }

    int dst_y = 16 - top[dst - 65];
    cct_showch(x, y, ' ', COLOR_BLACK, COLOR_WHITE, len);
    for (y = 2; y < dst_y; y++) {
        cct_showstr(x, y, " ", bg_color, fg_color, len);

        Sleep(my_get_time());

        if (y < dst_y - 1) {
            /* 清除显示(最后一次保留)，清除方法为用正常颜色+空格重画一遍刚才的位置 */
            cct_showch(x, y, ' ', COLOR_BLACK, COLOR_WHITE, len);
            if (y > 2)
                cct_showch(x + stack[src - 65][top[src - 65] - 1], y, ch, COLOR_HYELLOW, COLOR_HBLUE, 1);
        }
    }
    cct_setcolor();
}

void horiz()
{
    cout << "A:";
    for (int i = 0; i < top[0]; i++)
        cout << stack[0][i] << " ";
    for (int i = 0; i < 10 - top[0]; i++)
        cout << "  ";
    cout << "B:";
    for (int i = 0; i < top[1]; i++)
        cout << stack[1][i] << " ";
    for (int i = 0; i < 10 - top[1]; i++)
        cout << "  ";
    cout << "C:";
    for (int i = 0; i < top[2]; i++)
        cout << stack[2][i] << " ";
    for (int i = 0; i < 10 - top[2]; i++)
        cout << "  ";
    cout << endl;
}

void hanoi_hv(int src,int tmp,int dst,int n,int order)
{
    if (order == 9)
        indi += 1;
    int y = 19 + 10 * (order / 8);
    if (indi_s == 1)
    {
        cct_gotoxy(22, 35);
        cout << "第" << setw(4) << (i) << "步（" << setw(2) << n << "#: " << (char)(src) << "-->" << (char)(dst) << "）";
        cout << " A:";
        for (int i = 0; i < top[0]; i++)
            cout << stack[0][i] << " ";
        for (int i = 0; i < 10 - top[0]; i++)
            cout << "  ";
        cout << "B:";
        for (int i = 0; i < top[1]; i++)
            cout << stack[1][i] << " ";
        for (int i = 0; i < 10 - top[1]; i++)
            cout << "  ";
        cout << "C:";
        for (int i = 0; i < top[2]; i++)
            cout << stack[2][i] << " ";
        for (int i = 0; i < 10 - top[2]; i++)
            cout << "  ";
    }

    if (indi_t == 0)
        while (_getch() != 13);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cct_gotoxy(12 + 10 * i, y - j);
            cout << "  ";
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < top[i]; j++)
        {
            cct_gotoxy(12 + 10 * i, y - j);
            cout << stack[i][j];
        }
    }

    if (indi_t == 0)
        while (_getch() != 13);
    else
        Sleep(my_get_time());
}

void bg(int src, int dst, int n,int order)
{
    if (order == 9)
        indi += 1;
    if (order == 4)
    {
        cct_cls();

        cout << "从 " << (char)src << " 移动到 " << (char)dst << "，共 " << n << " 层，延时设置为 " << indi_t << "，" << (indi_s ? "" : "不") << "显示内部数组值" << endl;

        {
            cct_gotoxy(10, 20);
            cout << "=========================" << endl;
            cct_gotoxy(12, 21);
            cout << "A" << endl;
            cct_gotoxy(22, 21);
            cout << "B" << endl;
            cct_gotoxy(32, 21);
            cout << "C" << endl;
        }

        if (indi_s == 1)
        {
            cct_gotoxy(22, 25);
            cout << "初始:                " << "A:";
            for (int i = 0; i < top[0]; i++)
                cout << stack[0][i] << " ";
            for (int i = 0; i < 10 - top[0]; i++)
                cout << "  ";
            cout << "B:";
            for (int i = 0; i < top[1]; i++)
                cout << stack[1][i] << " ";
            for (int i = 0; i < 10 - top[1]; i++)
                cout << "  ";
            cout << "C:";
            for (int i = 0; i < top[2]; i++)
                cout << stack[2][i] << " ";
            for (int i = 0; i < 10 - top[2]; i++)
                cout << "  ";
        }

        if (indi_t == 0)
            while (_getch() != 13);


        for (int i = 0; i < top[src - 65]; i++)
        {
            cct_gotoxy(12 + 10 * (src - 65), 19 - i);
            cout << stack[src - 65][i];
        }
    }
    else
    {
        cct_cls();
        cout << "从 " << (char)src << " 移动到 " << (char)dst << "，共 " << n << " 层，延时设置为 " << indi_t << "，" << (indi_s ? "" : "不") << "显示内部数组值" << endl;
        ini_plate(src, dst, n);
        cct_setcolor();
        cct_gotoxy(10, 30);
        cout << "=========================" << endl;
        cct_gotoxy(12, 31);
        cout << "A" << endl;
        cct_gotoxy(22, 31);
        cout << "B" << endl;
        cct_gotoxy(32, 31);
        cout << "C" << endl;

        if (indi_s == 1)
        {
            cct_gotoxy(22, 35);
            cout << "初始:                " << "A:";
            for (int i = 0; i < top[0]; i++)
                cout << stack[0][i] << " ";
            for (int i = 0; i < 10 - top[0]; i++)
                cout << "  ";
            cout << "B:";
            for (int i = 0; i < top[1]; i++)
                cout << stack[1][i] << " ";
            for (int i = 0; i < 10 - top[1]; i++)
                cout << "  ";
            cout << "C:";
            for (int i = 0; i < top[2]; i++)
                cout << stack[2][i] << " ";
            for (int i = 0; i < 10 - top[2]; i++)
                cout << "  ";
        }

        if (indi_t == 0)
            while (_getch() != 13);

        for (int i = 0; i < top[src - 65]; i++)
        {
            cct_gotoxy(12 + 10 * (src - 65), 29 - i);
            cout << stack[src - 65][i];
        }
    }
}

void out_put(int order, int src, int dst, int tmp, int n, int i)
{
    switch (order)
    {
    case 1:
        stack_move(src, dst);
        cout << setw(2) << n << "# " << (char)(src) << "-->" << (char)(dst) << endl;
        break;
    case 2:
        stack_move(src, dst);
        cout << "第" << setw(4) << i << "步（" << setw(2) << n << "#:" << (char)(src) << "-->" << (char)(dst) << "）" << endl;
        break;
    case 3:
        stack_move(src, dst);
        cout << "第" << setw(4) << i << "步（" << setw(2) << n << "#:" << (char)(src) << "-->" << (char)(dst) << "）" ;
        horiz();
        break;
    case 4:
        stack_move(src, dst);
        hanoi_hv(src, tmp, dst, n, order);
        break;
    case 8:
        move_plate(src, dst, n);
        stack_move(src, dst);
        hanoi_hv(src, tmp, dst, n, order);
        break;
    }
}

void hanoi(int order, int src, int tmp, int dst, int n)
{
    if (n <= 0)
        return;
    hanoi(order, src, dst, tmp, n - 1);
    out_put(order, src, dst, tmp, n, i);
    i++;
    hanoi(order, tmp, src, dst, n - 1);
}

void read_9(int *src,int *dst,int *flag)
{
    char ch1, ch2;
    cct_setcolor();
    while (1)
    {
        cin >> ch1 >> ch2;
        if(ch1>='a'&&ch1<='z')
            ch1-='a'-'A';
        if (ch2 >= 'a' && ch2 <= 'z')
            ch2 -= 'a' - 'A';
        if (ch1 == 'Q')
        {
            cout << endl << "游戏中止";
            *flag = 0;
            break;
        }
        if (ch1 < 'A' || ch1>'C' || ch2 < 'A' || ch2>'Z')
        {
            *flag = 0;
            continue;
        }
        *src = (ch1 - 65) % 3, *dst = (ch2 - 65) % 3;

        if ((top[*dst] != 0) && (stack[*src][top[*src] - 1] > stack[*dst][top[*dst] - 1]))
        {
            cct_gotoxy(0, 34);
            cout << "大盘压小盘！";
            Sleep(1000);
            cct_gotoxy(0, 34);
            cout << "            ";
            cct_gotoxy(0, 33);
            cout << "                                                                                                                 ";
            cct_gotoxy(0, 33);
            cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：";
            continue;
        }

        if (stack[*src][top[*src] - 1]==0)
        {
            cct_gotoxy(0, 34);
            cout << "源柱为空！";
            Sleep(1000);
            cct_gotoxy(0, 34);
            cout << "            ";
            cct_gotoxy(0, 33);
            cout << "                                                                                                                 ";
            cct_gotoxy(0, 33);
            cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：";
            continue;
        }
        *src += 65, * dst += 65;
        cct_gotoxy(0, 33);
        cout << "                                                                                                                 ";
        cct_gotoxy(0, 33);
        cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：";
        break;
   
    }
}

void op_9(int src,int dst,int tmp,int n)
{
    do
    {
        int flag = 1;
        cct_gotoxy(0, 33);
        cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：";
        read_9(&src, &dst, &flag); //cout << ch1 << ch2;
        move_plate(src, dst, n);
        stack_move(src, dst);
        hanoi_hv(src, 3 - src - dst, dst, n, 9);
    } while (top[dst] == n);
    cct_gotoxy(0, 35);
    cout << endl << "游戏结束";
}

void contr(int order ,int src,int dst, int tmp, int n)
{
	switch (order)
	{
    case 1:
    case 2:
    case 3:
        hanoi(order, src, tmp, dst, n);
        cct_setcolor();
        cout << endl << endl << endl << endl << endl;
        cout << "按回车键继续";
        while (_getch() != '\r');
        cct_cls();
        break;
    case 4:
    case 8:
        get_indi();
        bg(src, dst, n, order);
        hanoi(order, src, tmp, dst, n);
        to_be_continued("按回车键继续");
        break;
	case 5:
		pillar();
		to_be_continued("按回车键继续");
        break;
    case 6:
        ini_plate(src, dst, n);
        to_be_continued("按回车键继续");
        break;
    case 7:
        ini_plate(src, dst, n);
        if (n % 2 != 0)
            move_plate(src, dst, n);
        else
            move_plate(src, tmp, n);
        to_be_continued("按回车键继续");
        break;
    case 9:
        bg(src, dst, n, order);
        op_9(src, dst, tmp, n);
        to_be_continued("按回车键继续");
        break;
    default:
        ;
	}
}
