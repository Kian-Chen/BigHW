/* 2250951 光电 陈麒安 */
#include<iostream>
#include<fstream>
#include<iomanip>
#include"txt_compare.h"
using namespace std;

// 去除字符串左侧的空白字符
string trimLeft(const string& str)
{
    size_t start = str.find_first_not_of(" \t\r");
    return start == string::npos ? "" : str.substr(start);
}

// 去除字符串右侧的空白字符
string trimRight(const string& str)
{
    size_t end = str.find_last_not_of(" \t\r\n");
    return end == string::npos ? "" : str.substr(0, end + 1);
}

// 判断一行是否为空行
bool isBlankLine(const string& line, bool ignoreBlank, bool trimmed) 
{
    if (!ignoreBlank) 
        return false;
    return trimmed ? line.empty() : line.find_first_not_of(" \t\r\n") == string::npos;
}

void highlightDifferences(const string& str1, const string& str2) 
{
    size_t len = max(str1.length(), str2.length());
    for (size_t i = 0; i < len; ++i) {
        char ch1 = i < str1.length() ? str1[i] : '\0';
        char ch2 = i < str2.length() ? str2[i] : '\0';

        if (ch1 != ch2) 
        {
            cct_setcolor(14, 4); // 设置高亮颜色
            if (ch1 == '\n' || ch1 == '\r' || ch1 == '\v' || ch1 == '\b' || ch1 == '\a')
            {
                cout << 'X'; // 用 'X' 表示特殊不可见字符的差异
            }
            else if (ch1 != '\0')
            {
                cout << ch1; // 正常显示字符
            }
        }
        else 
        {
            cct_setcolor(0, 7); // 设置普通显示颜色
            cout << ch1;
        }
    }

    cct_setcolor(0, 7); // 重置颜色为默认
    cout << "<CR>" << endl; // 每行末尾的标记
}

void highlightstr(string str)
{
    cct_setcolor(14, 4); // 设置高亮颜色
    cout << str;
    cct_setcolor(0, 7); // 重置颜色为默认
    cout << "<CR>" << endl; // 每行末尾的标记
}

void print_bar(int num, char ch)
{
    for (int i = 0; i < num; i++)
        cout << ch;
    cout << endl;
}

void print_hint(int diff)
{
    cout << "在指定检查条件下共" << diff << "行有差异." << endl;
    cout << "阅读提示：" << endl;
    cout <<
        "        1、空行用<EMPTY>标出\n\
        2、文件结束用<EOF>标出\n\
        3、两行相同列位置的差异字符用亮色标出\n\
        4、每行中的CR / LF / VT / BS / BEL用X标出(方便看清隐含字符)\n\
        5、每行尾的多余的字符用亮色标出，CR / LF / VT / BS / BEL用亮色X标出(方便看清隐含字符)\n\
        6、每行最后用<CR>标出(方便看清隐含字符)\n\
        7、中文因为编码问题，差异位置可能报在后半个汉字上，但整个汉字都亮色标出\n\
        8、用--display detailed可以得到更详细的信息" << endl;
}

void print_ruler(int len)
{
    cout << "        ";
    print_bar(len, '-');
    cout << "        ";
    for (int i = 0; i < len / 10; i++)
        cout << i << "         ";
    cout << endl;
    cout << "        ";
    for (int i = 0; i < len / 10; i++)
        cout << "0123456789";
    cout << endl;
    cout << "        ";
    print_bar(len, '-');
}

void offset(int bias)
{
    cout << setw(8) << setfill('0') << hex << uppercase << bias << " : ";
}

void hex(unsigned char* line, int size)
{
    for (int i = 0; i < 16; i++)
    {
        if (i == 8)
            if (i < size)
                cout << "- ";
            else
                cout << "  ";
        if (i < size)
            cout << setw(2) << setfill('0') << hex << uppercase << (int)(line[i]) << " ";
        else
            cout << "   ";
    }
    cout << " ";
}

void dec(unsigned char* line, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (line[i] >= 33 && line[i] <= 126)
            cout << line[i];
        else
            cout << ".";
    }
}

void showHex(const string str)
{
    int bias = 0;
    for (; bias < str.length(); bias += 16)
    {
        unsigned char temp[16] = { 0 };
        offset(bias);
        int len = (str.length() - bias >= 16) ? 16 : (str.length() - bias);
        for (int i = 0; i < len; i++)
            temp[i] = str[i + bias];
        hex(temp, len);
        dec(temp, len);
        cout << endl;
    }
}


int firstDiff(const string& str1, const string& str2) 
{
    int minLength = str1.size() < str2.size() ? str1.size() : str2.size();

    for (int i = 0; i < minLength; ++i) 
    {
        if (str1[i] != str2[i]) 
        {
            return i; // 返回第一个不匹配的位置
        }
    }

    if (str1.size() != str2.size()) 
    {
        return minLength; // 如果一个字符串比另一个长，返回较短字符串的长度
    }

    return -1; // 如果完全匹配，返回 -1
}

// 检查文件格式（换行符类型）
FileFormat checkFileFormat(const string& filename) 
{
    ifstream fin(filename, std::ios::binary);
    if (!fin) 
    {
        cerr << "文件打开失败" << endl;
        return Confuse;
    }

    char ch;
    bool lastCharWasCR = false;
    bool hasCRLF = false;
    bool hasLF = false;

    while (fin.get(ch))
    {
        if (ch == '\x0A') 
        {
            if (lastCharWasCR) 
            {
                hasCRLF = true;
            }
            else 
            {
                hasLF = true;
            }
        }
        lastCharWasCR = (ch == '\x0D');
    }

    fin.close();

    if (hasCRLF && hasLF) 
    {
        return Confuse;
    }
    else if (hasCRLF)
    {
        return Win;
    }
    else 
    {
        return Linux;
    }
}

void outputDiff(int lineNum1, int lineNum2, string str1, string str2, bool detailed,struct info info1={0,0}, struct info info2 = { 0,0 })
{
    cout << "第[" << lineNum1 << " / " << lineNum2 << "]行 - ";
    if ((info1.empty && info1.eof && info2.empty && info2.eof))
        cout<<"第[" << firstDiff(str1, str2) << "]个字符开始有差异：" << endl;
    else
    {
        cout << "文件1" << (info1.eof ? "已结束" : "仍有内容") << " / 文件2" << (info2.eof ? "已结束" : "仍有内容") << "：" << endl;
    }

    if (detailed)
        print_ruler((max((int)str1.length(), (int)str2.length()) / 10 + 1) * 10);
    cout << "文件1 : ";
    if (info1.eof)
        cout << "<EOF>" << endl;
    else if (info1.empty&&str1.empty())
        highlightstr("<EMPTY>");
    else
        highlightDifferences(str1, str2);
    cout << "文件2 : ";
    if (info2.eof)
        cout << "<EOF>" << endl; 
    else if (info2.empty&&str2.empty())
        highlightstr("<EMPTY>");
    else
        highlightDifferences(str2, str1);

    if (detailed)
    {
        cout << "文件1(HEX) :" << endl;
        if (info1.eof)
            cout << "<EOF>" << endl;
        else if (info1.empty)
            cout << "<EMPTY>" << endl;
        else
            showHex(str1);
        cout << "文件2(HEX) :" << endl;
        if (info2.eof)
            cout << "<EOF>" << endl;
        else if (info2.empty)
            cout << "<EMPTY>" << endl;
        else
            showHex(str2);
    }
    cout << endl;
}

int file_len(const string& file)
{
    ifstream fin(file);
    int len = 0;
    while (true)
    {
        string temp;
        if (!getline(fin, temp))
            break;
        len = max(len, (int)temp.length());
    }
    return len;
}

// 比较两个文件的内容，并显示差异
void compareFiles(const string& file1, const string& file2, bool trimLeftFlag, bool trimRightFlag, int lineOffset, int lineSkip, bool ignoreBlank, int maxDiff, int maxLine, bool CR_CRLF_not_equal, string displayOption)
{
    bool detailedOutput = displayOption == "normal" || displayOption == "detailed";

    int len = max(file_len(file1), file_len(file2));
    len = (len / 10 + 1) * 10 + 8 + 2;
    if (detailedOutput)
    {
        cout << "比较结果输出：" << endl;
        print_bar(len, '=');
    }


    ifstream f1(file1), f2(file2);
    if (!f1.is_open() || !f2.is_open())
    {
        cerr << "无法打开文件" << endl;
        return;
    }

    string line1, line2;
    int lineNum = 0, lineNum1 = 0, lineNum2 = 0, diffCount = 0, effectiveLines = 0;

    FileFormat format1 = checkFileFormat(file1), format2 = checkFileFormat(file2);

    while (true) 
    {
        bool endOfFile1 = !getline(f1, line1);
        bool endOfFile2 = !getline(f2, line2);

        if (CR_CRLF_not_equal)
        {
            if (format1 == Win)
                line1.push_back('\r');

            if (format2 == Win)
                line2.push_back('\r');
        }

        // 检查文件结束条件
        if (endOfFile1 || endOfFile2) 
        {
            struct info info1, info2;
            info1.eof = endOfFile1;
            info2.eof = endOfFile2;
            if (!endOfFile1) 
            {
                // 如果 f1 没有结束，检查 f1 剩余部分是否都是空行
                do 
                {
                    lineNum1++;
                    string tline1 = (trimLeftFlag ? trimLeft(line1) : line1);
                    tline1 = (trimRightFlag ? trimRight(tline1) : tline1);
                    if (!isBlankLine(tline1, ignoreBlank, true))
                    {
                        diffCount++; // 如果 f1 剩余部分存在非空行，视为不同
                        break;
                    }
                    else
                    {
                        info1.empty = true;
                    }
                } while (getline(f1, line1)); 
            }
            else
            {
                lineNum1++;
            }
            if (!endOfFile2) 
            {
                // 如果 f2 没有结束，检查 f2 剩余部分是否都是空行
                do {
                    lineNum2++;
                    string tline2 = (trimLeftFlag ? trimLeft(line2) : line2);
                    tline2 = (trimRightFlag ? trimRight(tline2) : tline2);
                    if (!isBlankLine(tline2, ignoreBlank, true))
                    {
                        diffCount++; // 如果 f2 剩余部分存在非空行，视为不同
                        break;
                    }
                    else
                    {
                        info2.empty = true;
                    }
                } while (getline(f2, line2));
            }
            else
            {
                lineNum2++;
            }
            if (detailedOutput)
                outputDiff(lineNum1, lineNum2, line1, line2, displayOption == "detailed", info1, info2);
            break;
        }
        
        // 应用去除空白的选项
        string trimmedLine1 = (trimLeftFlag ? trimLeft(line1) : line1);
        trimmedLine1 = (trimRightFlag ? trimRight(trimmedLine1) : trimmedLine1);
        string trimmedLine2 = (trimLeftFlag ? trimLeft(line2) : line2);
        trimmedLine2 = (trimRightFlag ? trimRight(trimmedLine2) : trimmedLine2);

        // 检查空行
        bool isBlank1 = isBlankLine(trimmedLine1, ignoreBlank, true);
        bool isBlank2 = isBlankLine(trimmedLine2, ignoreBlank, true);

        lineNum1++, lineNum2++;
        
        if (isBlank1 && !isBlank2) 
        {
            lineNum2--;
            continue; // 如果文件1是空行而文件2不是，则跳过文件1的这一行
        }
        else if (isBlank2 && !isBlank1) {
            // 如果文件2是空行而文件1不是，跳过文件2的这一行
            lineNum1--;
            f1.seekg(-static_cast<int>(line1.length() + 2), ios_base::cur);
            continue;
        }
        else if (isBlank1 && isBlank2) {
            continue; // 如果两个文件的这一行都是空行，跳过此次循环
        }

        lineNum++; // 实际的行号

        // 应用行偏移逻辑
        if (lineNum <= lineOffset) 
        {
            continue;
        }

        effectiveLines++; // 更新有效行数

        // 应用行跳过逻辑
        if (effectiveLines <= lineSkip) 
        {
            continue;
        }

        // 比较行内容
        if (trimmedLine1 != trimmedLine2) 
        {
            diffCount++; 
            if (detailedOutput)
                outputDiff(lineNum1, lineNum2, trimmedLine1, trimmedLine2, displayOption == "detailed");
        }

        // 检查最大差异和最大行数限制
        if ((maxDiff > 0 && diffCount >= maxDiff) || (maxLine > 0 && lineNum >= maxLine)) 
        {
            break;
        }
    }


    if (!detailedOutput) 
    {
        cout << "文件" << (diffCount == 0 ? "相同." : "不同.") << endl;
    }
    else
    {
        print_bar(len, '=');
        print_hint(diffCount);
        print_bar(len, '=');
    }
    
    f1.close();
    f2.close();
}


// 分配和处理命令行参数
void args_distribute(const args_analyse_tools* args) 
{
    string file1 = args[OPT_ARGS_FILE1].get_string();
    string file2 = args[OPT_ARGS_FILE2].get_string();
    string trimOption = args[OPT_ARGS_TRIM].get_string();
    int lineOffset = args[OPT_ARGS_LINE_OFFSET].get_int();
    int lineSkip = args[OPT_ARGS_LINE_SKIP].get_int();
    bool ignoreBlank = args[OPT_ARGS_IGNORE_BLANK].existed();
    int maxDiff = args[OPT_ARGS_MAX_DIFFNUM].get_int();
    int maxLine = args[OPT_ARGS_MAX_LINENUM].get_int();
    bool CR_CRLF_not_equal = args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed();
    string displayOption = args[OPT_ARGS_DISPLAY].get_string();

    bool trimLeft = trimOption == "left" || trimOption == "all";
    bool trimRight = trimOption == "right" || trimOption == "all";
    bool detailedOutput = displayOption == "normal" || displayOption == "detailed";

    compareFiles(file1, file2, trimLeft, trimRight, 
        lineOffset, lineSkip, ignoreBlank, maxDiff, maxLine, CR_CRLF_not_equal, displayOption);
}

string extractFilename(const char* path)
{
    string fullPath(path);

    size_t pos = fullPath.find_last_of("\\/");
    if (pos != std::string::npos) 
    {
        // 提取分隔符之后的部分
        return fullPath.substr(pos + 1);
    }
    return fullPath; // 如果没有路径分隔符，返回整个字符串
}