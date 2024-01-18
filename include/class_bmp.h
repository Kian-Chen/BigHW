/* 光电 2250951 陈麒安 */
#pragma once
using namespace std;
/* 根据需要加入自己的定义 */

#include <iostream>
#include "utilities.h"
using namespace std;

struct BitmapFileHeader
{
    char bfType[3];        // 文件类型，必须为'BM'
    int bfSize;            // 文件大小，字节为单位
    short bfReserved1;     // 保留字段1，必须为0
    short bfReserved2;     // 保留字段2，必须为0
    int bfOffBits;         // 数据偏移量，从文件头到图像数据的字节偏移量
};

struct BitmapInfoHeader
{
    int biSize;          // 结构体大小
    int biWidth;         // 图像宽度
    int biHeight;        // 图像高度
    short biPlanes;      // 平面数，必须为1
    short biBitCount;    // 每个像素的位数
    int biCompression;   // 压缩类型
    int biSizeImage;     // 图像数据大小，字节为单位
    int biXPelsPerMeter; // 水平分辨率
    int biYPelsPerMeter; // 垂直分辨率
    int biClrUsed;       // 使用的颜色索引数
    int biClrImportant;  // 对图像显示有重要影响的颜色索引数
};

struct RGBQuad
{
    unsigned char rgbBlue;      // 蓝色分量
    unsigned char rgbGreen;     // 绿色分量
    unsigned char rgbRed;       // 红色分量
    unsigned char rgbReserved;  // 保留字节
};

struct PixelLocation 
{
    int byteOffset;
    int bitOffset; 
    bool isUpper;
};

class bitmap_image
{
protected:
    /* 根据需要加入自己的定义 */
    string filename;
    BitmapFileHeader header;
    BitmapInfoHeader dbi;
    List<RGBQuad> color_table;
public:
    int width() const;	//返回图片的宽度
    int height() const; //返回图片的高度
    unsigned int get_pixel(int row, int col) const; //返回指定点的RGB颜色
    void read_headers();
    void print_headers();
    bitmap_image(const char* inputname);
    /* 根据需要加入自己的定义 */
    BitmapFileHeader get_head();
    BitmapInfoHeader get_dbi();
    PixelLocation getPixelLocation(int x, int y);
};
