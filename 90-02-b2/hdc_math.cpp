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

const int MaxX = 1000;
const int MaxY = 1000;
const double eps = 1e-8;
const double PI = 3.4159265358979323846;

static double parseExpression(char*& expr, double x, double y);
static double parsePrimary(char*& expr, double x, double y);
static double parsePower(char*& expr, double x, double y);
static double parseTerm(char*& expr, double x, double y);
static double evaluateExpression(const char* expression, double x, double y);

static double evaluateEquation(double x, double y)
{
    return x * x + y * y - 10;
}

static void refinePixel(int x, int y, int size) 
{
    double result = evaluateEquation(x + size / 2, y + size / 2);

    if (fabs(result) < eps) 
    {
        for (int i = 0; i < 2; i++) 
        {
            for (int j = 0; j < 2; j++) 
            {
                refinePixel(x + i * size / 2, y + j * size / 2, size / 2);
            }
        }
    }
    else if (result > eps) 
    {
        hdc_rectangle(x, y, size, size, 0, false, 0, RGB(255, 255, 255));
    }
    else
    {
        hdc_rectangle(x, y, size, size, 0, false, 0, RGB(0, 0, 0));
    }
}

static void propmt()
{
    cout << "该程序参考GrafEq软件算法完成，可以根据一个二元函数对正负区域进行黑白划分，即绘制F(x,y)>0和F(x,y)<0的区域（默认坐标轴均由-10到10），支持+, -, *, /, ^, sin, cos, tan, asin, acos, atan, exp, log, sqrt算符。" << endl;
    cout << "本程序一定程度上也可以展示GrafEq的优势，对于较为复杂的隐函数也可保持较高精度。有兴趣的同学可以将GrafEq与本程序所绘的图像与Wolfram Mathematica、Maple、GeoGeBra等数学软件进行对比。" << endl;
    cout << "下面推荐几个较为复杂的二元函数，可供参考:" << endl;
    cout << "   1.x/sin(x)+y/sin(y)-(x*y)/sin(x*y)" << endl;
    cout << "   2.sin(x^2+y^2)-cos(x*y)" << endl;
    cout << "   3.sin(sin(x)+cos(y))-cos(sin(x*y)+cos(x))" << endl;
    cout << "参考文献见:" << endl;
    cout << "Jeff Tupper.Reliable two-dimensional graphing methods for mathematical formulae with two free variables[A].SIGGRAPH '01: Proceedings of the 28th annual conference on Computer graphics and interactive techniques[C],2001" << endl;
    cout << "输入带有 x 和 y 的二元函数表达式: ";
}

void hdc_draw_Mathematical_curve_2250951() 
{
    char expression[256];  // 假设表达式不超过 255 个字符
    propmt();
    cin.getline(expression, sizeof(expression));
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
            double xPos = xMin + (x + pixelSize / 2.0) * (xMax - xMin) / MaxX;
            double yPos = yMin + (y + pixelSize / 2.0) * (yMax - yMin) / MaxY;

            double result = /*evaluateEquation(xPos, yPos);*/evaluateExpression(expression, xPos, yPos);

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
    cct_setconsoleborder(120, 30, 120, 9001);
}


static double parsePrimary(char*& expr, double x, double y)
{
    if (*expr == 'x')
    {
        ++expr;
        return x;
    }
    else if (*expr == 'y')
    {
        ++expr;
        return y;
    }
    else if (isdigit(*expr) || (*expr == '.'))
    {
        double value = strtod(expr, &expr);
        return value;
    }
    else if (*expr == '(')
    {
        ++expr;
        double value = parseExpression(expr, x, y);
        if (*expr == ')')
        {
            ++expr;
            return value;
        }
        else
        {
            throw runtime_error("语法错误");
        }
    }
    else if (strncmp(expr, "sin(", 4) == 0)
    {
        expr += 4;
        double arg = parseExpression(expr, x, y);
        if (*expr == ')')
        {
            ++expr;
            return sin(arg);
        }
        else
        {
            throw runtime_error("语法错误");
        }
    }
    else if (strncmp(expr, "cos(", 4) == 0) {
        expr += 4;
        double arg = parseExpression(expr, x, y);
        if (*expr == ')')
        {
            ++expr;
            return cos(arg);
        }
        else
        {
            throw runtime_error("语法错误");
        }
    }
    else if (strncmp(expr, "tan(", 4) == 0)
    {
        expr += 4;
        double arg = parseExpression(expr, x, y);
        if (*expr == ')')
        {
            ++expr;
            return tan(arg);
        }
        else
        {
            throw runtime_error("语法错误");
        }
    }
    else if (strncmp(expr, "asin(", 5) == 0)
    {
        expr += 5;
        double arg = parseExpression(expr, x, y);
        if (*expr == ')')
        {
            ++expr;
            return asin(arg);
        }
        else
        {
            throw runtime_error("语法错误");
        }
    }
    else if (strncmp(expr, "acos(", 5) == 0)
    {
        expr += 5;
        double arg = parseExpression(expr, x, y);
        if (*expr == ')')
        {
            ++expr;
            return acos(arg);
        }
        else
        {
            throw runtime_error("语法错误");
        }
    }
    else if (strncmp(expr, "atan(", 5) == 0)
    {
        expr += 5;
        double arg = parseExpression(expr, x, y);
        if (*expr == ')')
        {
            ++expr;
            return atan(arg);
        }
        else
        {
            throw runtime_error("语法错误");
        }
    }
    else if (strncmp(expr, "exp(", 4) == 0)
    {
        expr += 4;
        double arg = parseExpression(expr, x, y);
        if (*expr == ')')
        {
            ++expr;
            return exp(arg);
        }
        else {
            throw runtime_error("语法错误");
        }
    }
    else if (strncmp(expr, "sqrt(", 5) == 0)
    {
        expr += 5;
        double arg = parseExpression(expr, x, y);
        if (*expr == ')')
        {
            ++expr;
            return sqrt(arg);
        }
        else {
            throw runtime_error("语法错误");
        }
    }
    else if (strncmp(expr, "pow(", 4) == 0) {
        expr += 4;
        double base = parseExpression(expr, x, y);
        if (*expr == ',')
        {
            ++expr;
            double exponent = parseExpression(expr, x, y);
            if (*expr == ')')
            {
                ++expr;
                return pow(base, exponent);
            }
            else
            {
                throw runtime_error("语法错误");
            }
        }
        else
        {
            throw runtime_error("语法错误");
        }
    }
    else if (strncmp(expr, "log(", 4) == 0)
    {
        expr += 4;
        double arg = parseExpression(expr, x, y);
        if (*expr == ')')
        {
            ++expr;
            return log(arg);
        }
        else
        {
            throw runtime_error("语法错误");
        }
    }
    else if (strncmp(expr, "log10(", 6) == 0)
    {
        expr += 6;
        double arg = parseExpression(expr, x, y);
        if (*expr == ')')
        {
            ++expr;
            return log10(arg);
        }
        else
        {
            throw runtime_error("语法错误");
        }
    }
    else
    {
        throw runtime_error("语法错误");
    }
}

static double parsePower(char*& expr, double x, double y)
{
    double left = parsePrimary(expr, x, y);
    while (*expr == '^')
    {
        ++expr;
        double right = parsePrimary(expr, x, y);
        left = pow(left, right);
    }
    return left;
}

static double parseTerm(char*& expr, double x, double y)
{
    double left = parsePower(expr, x, y);
    while (*expr == '*' || *expr == '/')
    {
        char op = *expr++;
        double right = parsePower(expr, x, y);
        if (op == '*')
        {
            left *= right;
        }
        else
        {
            if (right == 0)
            {
                throw runtime_error("除零错误");
            }
            left /= right;
        }
    }
    return left;
}

static double parseExpression(char*& expr, double x, double y)
{
    double left = parseTerm(expr, x, y);
    while (*expr == '+' || *expr == '-')
    {
        char op = *expr++;
        double right = parseTerm(expr, x, y);
        if (op == '+')
        {
            left += right;
        }
        else
        {
            left -= right;
        }
    }
    return left;
}

static double evaluateExpression(const char* expression, double x, double y)
{
    char* expr = (char*)(expression);
    return parseExpression(expr, x, y);
}
