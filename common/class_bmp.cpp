/* 2250951 陈麒安 光电 */
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include<fstream>
#include<iomanip>
#include "../include/class_bmp.h"
using namespace std;

bitmap_image::bitmap_image(const char* inputname)
{
    filename = inputname;
    read_headers();
    //print_headers();
}

void bitmap_image::print_headers()
{
    cout << hex << uppercase;
    cout << "bfType : " << header.bfType << endl;
    cout << "bfSize : " << header.bfSize << endl;
    cout << "bfReserved1 : " << header.bfReserved1 << endl;
    cout << "bfReserved2 : " << header.bfReserved2 << endl;
    cout << "bfOffBits : " << header.bfOffBits << endl;
    cout << endl;

    cout << "biSize : " << dbi.biSize << endl;
    cout << "biWidth : " << dbi.biWidth << endl;
    cout << "biHeight : " << dbi.biHeight << endl;
    cout << "biPlanes : " << dbi.biPlanes << endl;
    cout << "biBitCount : " << dbi.biBitCount << endl;
    cout << "biCompression : " << dbi.biCompression << endl;
    cout << "biSizeImage : " << dbi.biSizeImage << endl;
    cout << "biXPelsPerMeter : " << dbi.biXPelsPerMeter << endl;
    cout << "biYPelsPerMeter : " << dbi.biYPelsPerMeter << endl;
    cout << "biClrUsed : " << dbi.biClrUsed << endl;
    cout << "biClrImportant : " << dbi.biClrImportant << endl;
    cout << endl;

    for (int i = 0; i < color_table.size(); i++)
        cout << "Blue : " << setfill('0') << setw(2) << (int)color_table[i].rgbBlue
        << "  Green : " << setfill('0') << setw(2) << (int)color_table[i].rgbGreen
        << "  Red : " << setfill('0') << setw(2) << (int)color_table[i].rgbRed << endl;
}

void bitmap_image::read_headers()
{
    ifstream fin(filename, ios::binary);
    if (!fin.is_open())
    {
        cerr << "无法打开文件: " << filename << endl;
    }
    // 读取BitmapFileHeader
    fin.read((char*)(&header.bfType), sizeof(short));
    fin.read((char*)(&header.bfSize), sizeof(header.bfSize));
    fin.read((char*)(&header.bfReserved1), sizeof(header.bfReserved1));
    fin.read((char*)(&header.bfReserved2), sizeof(header.bfReserved2));
    fin.read((char*)(&header.bfOffBits), sizeof(header.bfOffBits));

    header.bfType[2] = 0;
    // 读取BitmapInfoHeader
    fin.read((char*)(&dbi.biSize), sizeof(dbi.biSize));
    fin.read((char*)(&dbi.biWidth), sizeof(dbi.biWidth));
    fin.read((char*)(&dbi.biHeight), sizeof(dbi.biHeight));
    fin.read((char*)(&dbi.biPlanes), sizeof(dbi.biPlanes));
    fin.read((char*)(&dbi.biBitCount), sizeof(dbi.biBitCount));
    fin.read((char*)(&dbi.biCompression), sizeof(dbi.biCompression));
    fin.read((char*)(&dbi.biSizeImage), sizeof(dbi.biSizeImage));
    fin.read((char*)(&dbi.biXPelsPerMeter), sizeof(dbi.biXPelsPerMeter));
    fin.read((char*)(&dbi.biYPelsPerMeter), sizeof(dbi.biYPelsPerMeter));
    fin.read((char*)(&dbi.biClrUsed), sizeof(dbi.biClrUsed));
    fin.read((char*)(&dbi.biClrImportant), sizeof(dbi.biClrImportant));

    if (dbi.biBitCount == 1 || dbi.biBitCount == 4 || dbi.biBitCount == 8)
    {
        int num_colors = dbi.biClrUsed == 0 ? (1 << dbi.biBitCount) : dbi.biClrUsed;
        color_table.resize(num_colors);
        for (int i = 0; i < num_colors; ++i)
        {
            fin.read((char*)(&color_table[i].rgbBlue), sizeof(color_table[i].rgbBlue));
            fin.read((char*)(&color_table[i].rgbGreen), sizeof(color_table[i].rgbGreen));
            fin.read((char*)(&color_table[i].rgbRed), sizeof(color_table[i].rgbRed));
            fin.read((char*)(&color_table[i].rgbReserved), sizeof(color_table[i].rgbReserved));
        }
    }

    fin.close();
}

int bitmap_image::width()const
{
    return dbi.biWidth;
}

int bitmap_image::height()const
{
    return dbi.biHeight;
}

unsigned int bitmap_image::get_pixel(int row, int col)const
{
    ifstream fin(filename, ios::binary);

    if (!fin.is_open())
    {
        cerr << "无法打开文件: " << filename << endl;
        return 0;
    }

    // 计算图像数据的偏移量
    int data_offset = header.bfOffBits;

    // 单色图像
    if (dbi.biBitCount == 1)
    {
        // 计算每行的字节数
        int bytes_per_row = (((dbi.biBitCount * dbi.biWidth) / 8) / 4 + 1) * 4;//对其

        // 计算图像数据的行偏移量
        int row_offset = bytes_per_row * (dbi.biHeight - row);

        // 定位到图像数据位置
        fin.seekg(data_offset + row_offset + col / 8, ios::beg);

        // 读取像素值
        unsigned char pixel_byte;
        fin.read((char*)(&pixel_byte), sizeof(pixel_byte));

        // 获取指定列的像素值
        int bit_position = 7 - (col % 8); // 从高位到低位读取
        unsigned char pixel_index = (pixel_byte >> bit_position) & 0x01;

        fin.close();

        // 根据索引从调色板中获取颜色
        struct RGBQuad color = color_table[pixel_index];
        return RGB(color.rgbRed, color.rgbGreen, color.rgbBlue);
    }

    else if (dbi.biBitCount == 4 || dbi.biBitCount == 8)
    {
        // 计算每行的字节数
        int bytes_per_row = (dbi.biBitCount * dbi.biWidth + 7) / 8;

        // 计算图像数据的行偏移量
        int row_offset = bytes_per_row * (dbi.biHeight - row);  

        // 定位到图像数据位置
        fin.seekg(data_offset + row_offset + col / (dbi.biBitCount == 4 ? 2 : 1), ios::beg);

        // 读取像素值
        unsigned char pixel_byte;
        fin.read((char*)(&pixel_byte), sizeof(pixel_byte));

        // 获取指定列的像素值
        unsigned int pixel_index;
        if (col % 2 == 0)
        {
            pixel_index = (pixel_byte >> 4) & 0x0F;
        }
        else
        {
            pixel_index = pixel_byte & 0x0F;
        }
        if(dbi.biBitCount==8)
            pixel_index=pixel_byte;
        fin.close();

        struct RGBQuad color = color_table[pixel_index];
        return RGB(color.rgbRed, color.rgbGreen, color.rgbBlue);
    }
    // 真彩色图像
    else if (dbi.biBitCount == 24)
    {
        // 计算每行的字节数
        int bytes_per_row = (dbi.biBitCount / 8) * dbi.biWidth + (dbi.biWidth % 4);

        // 计算图像数据的行偏移量
        int row_offset = bytes_per_row * (dbi.biHeight - row);

        // 定位到图像数据位置
        fin.seekg(data_offset + row_offset + col * (dbi.biBitCount / 8), ios::beg);

        // 读取像素值
        RGBQuad pixel_data;
        fin.read(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));

        fin.close();
        return RGB(pixel_data.rgbRed, pixel_data.rgbGreen, pixel_data.rgbBlue);
    }
    else
    {
        fin.close();
        return 0;
    }
}

BitmapFileHeader bitmap_image::get_head()
{
    return header;
}
BitmapInfoHeader bitmap_image::get_dbi()
{
    return dbi;
}

PixelLocation bitmap_image::getPixelLocation(int x, int y)
{
    PixelLocation location;

    int offset = header.bfOffBits;

    double bytesPerPixel = dbi.biBitCount / 8; 
    int bytes_per_row = (((dbi.biBitCount * dbi.biWidth) / 8) / 4 + ((dbi.biBitCount == 1) ? 1 : 0)) * 4;

    int x_offset;
    if (dbi.biBitCount == 1)
        x_offset = x / 8;
    else if (dbi.biBitCount == 4)
        x_offset = x / 4;
    else
        x_offset = x * bytesPerPixel;
    location.byteOffset = offset + bytes_per_row * y + x_offset;

    if (dbi.biBitCount == 1)
    {
        location.bitOffset = 7 - (x % 8);
    }
    else if (dbi.biBitCount == 4)
    {
        location.isUpper = (x % 2 == 0);
    }

    return location;
}