/* 2250951 光电 陈麒安 */

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
using namespace std;

#include "../include/cmd_hdc_tools.h"

const int MaxX = 800;
const int MaxY = 800;
const double eps = 1e-8;
const double PI = 3.4159265358979323846;

static double evaluateEquation(double x, double y)
{
    return -(pow(x, 2) / 64 + pow(y, 2) / 25 - 1) *
        (pow(x + 5, 2) + pow(-y - 5, 2) - 1) *
        (pow(x - 5, 2) + pow(-y - 5, 2) - 1) *
        (pow(x - 3, 2) / 6 + pow(-y - 2, 2) / 3 - 1) *
        (pow(x + 3, 2) / 6 + pow(-y - 2, 2) / 3 - 1) *
        (pow(x, 1000) + pow(-y, 1000) - 1) *
        (pow(x, 2) + pow(-y + 3, 2) - 0.5);

    /*(
    (
        pow(fabs(3 * x - y * sin(pow(y, 2) / 2) + y + 3 / pow(4 - y, 2) - 1), 4) / 16 +
        2 * fabs(x - y * sin(pow(y, 2)) / 3) -
        3 * y / (1 + 16 * exp(-4 * pow(2.2 - y, 2))) +
        1 / pow(4 - y, 2) - 4
        ) *
    pow((fabs(3 * x - y * sin(pow(y, 2)) + y + 3 / pow(4 - y, 2) - 3)), 2) +
    4 * pow(
        y - 4 * fabs(x - y * sin(pow(y, 2)) / 3) - 1 / pow(4 - y, 2) + 1
        , 2)
    - 1
    ) - 4;*/
}

static void refinePixel(int x, int y, int size) 
{
    double result = evaluateEquation(x + size / 2, y + size / 2);

    if (fabs(result) < eps)
        for (int i = 0; i < 2; i++) 
            for (int j = 0; j < 2; j++) 
                refinePixel(x + i * size / 2, y + j * size / 2, size / 2);
    else if (result > eps) 
        hdc_rectangle(x, y, size, size, 0, false, 0, RGB(255, 255, 255));
    else
        hdc_rectangle(x, y, size, size, 0, false, 0, RGB(0, 0, 0));
}

void hdc_draw_cartoon_2250951(const int base_x, const int base_y)
{
	//进入函数后执行的第一个语句必须是 hdc_cls();
	hdc_cls();
    // 初始化绘图库
    hdc_init(COLOR_BLACK, COLOR_WHITE, MaxX, MaxY);

    // 选择要计算方程的x和y值的范围
    double xMin = -10;
    double xMax = 10;
    double yMin = -10;
    double yMax = 10;

    // 选择像素的大小
    int pixelSize = 1;

    // 循环遍历像素并处理
    for (int x = 0; x < MaxX; x += pixelSize) 
    {
        for (int y = 0; y < MaxY; y += pixelSize)
        {
            double xPos = xMin + (x + pixelSize / 2.0) * (xMax - xMin) / MaxX + base_x - 400;
            double yPos = yMin + (y + pixelSize / 2.0) * (yMax - yMin) / MaxY + base_y - 400;

            double result = evaluateEquation(xPos, yPos);

            if (fabs(result) < eps) 
            {
                //hdc_rectangle(x, y, pixelSize, pixelSize, 0, true, 10, RGB(255, 0, 0));
                refinePixel(x, y, pixelSize);
            }
            else if (result > eps)
            {
                hdc_rectangle(x, y, pixelSize, pixelSize, 0, 1, 0, RGB(255, 255, 255));
            }
            else 
            {
                hdc_rectangle(x, y, pixelSize, pixelSize, 0, false, 0, RGB(0, 0, 0));
            }
        }
    }
    _getch();
    hdc_release();
}