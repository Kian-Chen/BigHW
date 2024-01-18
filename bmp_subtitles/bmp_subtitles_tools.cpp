#include "bmp_subtitles.h"
#include<fstream>
#include<iomanip>
#include<iostream>
#include<sstream>
using namespace std;

/***************************************************************************
  函数所属：杂项函数
  功    能：分配和处理命令行参数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void args_distribute(const args_analyse_tools* args)
{
	string bmp_name = args[OPT_ARGS_BMP].get_string();
	bitmap_image bmp(bmp_name.c_str());
	string output_name = args[OPT_ARGS_OUTPUT].get_string();
	bool hori = args[OPT_ARGS_STYLE].get_string() == "horizontal";
	string font_name = args[OPT_ARGS_TRADITION].existed() ? "HZK16F" : "HZK16";
	struct node start;
	start.x = args[OPT_ARGS_POSX].get_int();
	start.y = bmp.get_dbi().biHeight - args[OPT_ARGS_POSY].get_int();
	string color = args[OPT_ARGS_COLOR].get_string();
	string content = args[OPT_ARGS_SUBTITLES].get_string();

	Subtitles subtitles(content, font_name, hori, start);
	subtitles.process(bmp_name, output_name, color);
}

/***************************************************************************
  函数所属：Color
  功    能：分配和处理命令行参数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

Color::Color(int color_bit, string color)
{
	b1 = false;
	b4 = 0x0F;
	b8 = 0x00;
	b24 = str2color(color);

	switch (color_bit)
	{
	case 1:
		type = MONO;
		break;
	case 4:
		type = RGB4;
		break;
	case 8:
		type = RGB8;
		break;
	default:
		type = RGB24;
		break;
	}
}

unsigned int Color::str2color(string str)
{

	if (str.length() != 6)
	{
		return 0x000000;
	}

	unsigned int color;
	std::stringstream ss;
	ss << hex << str;
	ss >> color;

	return color;
}

template <int N>
typename TypeSelector<N>::type Color::get_color()
{
	switch (type)
	{
	case MONO:
		return b1;
		break;
	case RGB4:
		return b4;
		break;
	case RGB8:
		return b8;
		break;
	case RGB24:
		return b24;
		break;
	default:
		break;
	}
}

ColorType Color::get_type()
{
	return type;
}

/***************************************************************************
  函数所属：Font
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

Font::Font(Glyph glyph, struct node start)
{
	x_start = start.x;
	y_start = start.y;
	x_end = start.x + SBCSize;
	y_end = (start.y - SBCSize > 0) ? start.y - SBCSize : 0;

	int index = 0;
	for (int i = 0; i < SBCSize; i++) 
	{
		for (int j = 0; j < SBCSize; j++) 
		{
			lattice.data[i][j] = (glyph.data[index / 8] & (1 << (7 - index % 8))) != 0;
			index++;
		}
	}
}

unsigned char Font::read_byte(const char* filename, int offset)
{
	ifstream fin(filename, ios::binary);
	if (!fin.is_open())
	{
		cerr << "文件" << filename << "打不开" << endl;
		return -1;
	}

	fin.seekg(offset);
	unsigned char tmp = 0;
	fin >> tmp;
	return tmp;
	fin.close();
}

unsigned char Font::update(unsigned char origin, int bit_offset)
{
	origin &= ~(1 << bit_offset);
	return origin;
}

unsigned char Font::update(unsigned char origin, unsigned char value, bool is_upper)
{
	unsigned char tmp;
	if (!is_upper)
		tmp = (origin & 0xF0) | (value <<4);
	else
		tmp = (origin & 0x0F) | (value >>4);
	return tmp;
}

void Font::write(string bmpname, string filepath, Color& front_color, bitmap_image& bmp)
{
	ofstream fout(filepath.c_str(), ios::in | ios::out);
	if (!fout.is_open())
	{
		cerr << "文件" << filepath << "打不开" << endl;
		return;
	}

	PixelLocation location;

	for (int i = 0; i < x_end - x_start; i++)
	{
		for (int j = 0; j < y_start - y_end; j++)
		{
			if (lattice.data[i][j] != 0)
			{
				PixelLocation location = bmp.getPixelLocation(j + x_start, y_start - i);
				fout.seekp(location.byteOffset);
				unsigned char tmp = read_byte(bmpname.c_str(), location.byteOffset);

				switch (front_color.get_type())
				{
				case MONO:
					fout << update(tmp, location.bitOffset);
					break;
				case RGB4:
					fout << update(tmp, front_color.get_color<RGB4>(), location.isUpper);
					break;
				case RGB8:
					fout << front_color.get_color<RGB8>();
					break;
				case RGB24:
					fout << front_color.get_color<RGB24>();
					break;
				default:
					break;
				}
			}
		}
	}
}

void Font::print_glyph(Glyph glyph)
{
	for (int i = 0; i < 32; i++)
	{
		if (i % 8 == 0 && i != 0)
			cout << endl;
		cout << hex << setw(2) << (unsigned int)glyph.data[i] << " ";
	}
	cout << dec;
}

void Font::print()
{
	for (int i = 0; i < SBCSize; i++)
	{
		for (int j = 0; j < SBCSize; j++)
			cout << (lattice.data[i][j] ? "1" : " ") << " ";
		cout << endl;
	}
}

/***************************************************************************
  函数所属：Subtitles
  功    能：分配和处理命令行参数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

void Subtitles::update(string str, string font_name)
{
	for (int i = 0; i + 1 < (int)str.length(); i += 2)
	{
		SBC temp;
		temp.data[0] = (unsigned int)str[i] % 0x100;
		temp.data[1] = (unsigned int)str[i + 1] % 0x100;
		content.push_back(temp);
	}

	ifstream fin(font_name);
	if (fin.is_open() == 0)
	{
		cout << "字库" << font_name << "打开失败" << endl;
		return;
	}

	for (int i = 0; i < (int)content.size(); i++)
	{
		// 计算区号和位号对应的数值
		int area = (int)(content[i].data[0]) - 0xA1;
		int position = (int)(content[i].data[1]) - 0xA1;

		int offset = (area * 94 + position);

		fin.seekg(offset * sizeof(Glyph), ios::beg);
		Glyph temp;
		fin.read((char*)(temp.data), sizeof(Glyph));

		glyph.push_back(temp);
	}
}

void Subtitles::create_font(bool hori, struct node start)
{
	struct node leftup = start;
	struct node delta;
	delta.x = !hori ? INTERVAL : 0;
	delta.y = !hori ? 0 : INTERVAL;
	for (int i = 0; i < (int)glyph.size(); i++)
	{
		Font temp(glyph[i],leftup);
		fonts.push_back(temp);
		leftup = !hori ? (leftup + delta): (leftup - delta);
	}
	/*
	for (int i = 0; i < fonts.size(); i++)
	{
		fonts[i].print_glyph(glyph[i]);
		cout << endl;
		fonts[i].print();
		cout << endl;
	}
	*/
}

Subtitles::Subtitles(string str, string font_name, bool hori, struct node start)
{
	update(str, font_name);
	create_font(hori, start);
}

void Subtitles::copy(string bmp_name, string output_name)
{
	ifstream fin(bmp_name, ios::binary);
	if (!fin.is_open())
	{
		cerr << "文件" << bmp_name << "打不开" << endl;
		return;
	}
	ofstream fout(output_name, ios::out | ios::binary);
	if (!fout.is_open())
	{
		cerr << "文件" << output_name << "打不开" << endl;
		return;
	}

	char ch;
	while (fin.get(ch))
		fout.put(ch);
	fout.seekp(0, std::ios::beg);

	fin.close();
	fout.close();
}

void Subtitles::process(string bmp_name, string output_name, string color)
{
	copy(bmp_name, output_name);
	bitmap_image bmp(bmp_name.c_str());

	Color front_color(bmp.get_dbi().biBitCount, color);

	for (int i = 0; i < (int)fonts.size(); i++)
	{
		fonts[i].write(bmp_name, output_name, front_color, bmp);

	}

}
