#pragma once
#include "../include/class_aat.h"
#include "../include/utilities.h"
#include "../include/class_bmp.h"

const int SBCSize = 16;
const int INTERVAL = 16;

enum ColorType
{
	MONO,
	RGB4,
	RGB8,
	RGB24
};

template <int N>
struct TypeSelector
{
    using type = void;
};

template <>
struct TypeSelector<MONO> {
    using type = bool;
};

template <>
struct TypeSelector<RGB4>
{
    using type = unsigned char;
};

template <>
struct TypeSelector<RGB8>
{
    using type = unsigned char;
};

template <>
struct TypeSelector<RGB24>
{
    using type = unsigned int;
};

enum OPT_ARGS
{
	OPT_ARGS_HELP = 0,
	OPT_ARGS_DEBUG,
	OPT_ARGS_BMP,
	OPT_ARGS_OUTPUT,
	OPT_ARGS_TRADITION,
	OPT_ARGS_SUBTITLES,
	OPT_ARGS_POSX,
	OPT_ARGS_POSY,
	OPT_ARGS_COLOR,
	OPT_ARGS_STYLE	
};

struct SBC
{
	unsigned char data[2];
};

struct Glyph
{
	unsigned char data[32];
};

struct Lattice
{
	bool data[SBCSize][SBCSize];
};

class Subtitles;

class Color
{
private:
    ColorType type;
    bool b1;
    unsigned char b4;
    unsigned char b8;
    unsigned int b24;

public:
	Color(int color_bit, string color);
    unsigned int str2color(string str);
    template <int N>
    typename TypeSelector<N>::type get_color();
	ColorType get_type();
};

class Font
{
private:
	int x_start;
	int y_start;
	int x_end;
	int y_end;
	Lattice lattice;

public:

	Font(Glyph glyph, struct node start);
	void print_glyph(Glyph glyph);
	void print();
	unsigned char read_byte(const char* filename, int offset);
	unsigned char update(unsigned char origin, int bit_offset);
	unsigned char update(unsigned char origin, unsigned char value, bool is_upper);
	void write(string bmpname, string filepath, Color& front_color, bitmap_image& bmp);
	friend Subtitles;
};

class Subtitles
{
private:
	List<SBC> content;
	List<Glyph> glyph;
	List<Font> fonts;

public:
	void update(string str, string font_name);
	void create_font(bool hori, struct node start);
	Subtitles(string str, string font_name, bool hori, struct node start);
	void copy(string bmp_name, string output_name);
	void process(string bmp_name, string output_name,string color);
};

// 分配和处理命令行参数
void args_distribute(const args_analyse_tools* args);