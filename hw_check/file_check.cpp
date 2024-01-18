#include<iostream>
#include<fstream>
#include"hw_check.h"
using namespace std;

File::File(string name, Student stu, Assignment assignment) : Student(stu), Assignment(assignment)
{
    this->filename = name;
    this->studentID = studentID;
    this->assignmentID = assignmentID;
    this->fileID = hash(studentID, assignmentID);
}

long long int File::hash(int stu_id, int assign_id)
{
    long long int stu = stu_id;
    long long int assign = assign_id;
    long long int factor = 1E7;
    return assign * factor + stu;
}

bool File::exist()
{
    ifstream fin(filename);
    if (fin.is_open() == 0)
        return false;
    else
        return true;
}


bool File::is_ansi()
{
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    char buffer[2];
    file.read(buffer, sizeof(buffer));

    for (int i = 0; i < file.gcount(); ++i) {
        unsigned char byte = static_cast<unsigned char>(buffer[i]);
        if (byte >= 0x80)
        {
            return false;
        }
    }

    return true;
}

bool File::is_pdf() 
{
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    const int buffer_size = 8;
    char buffer[buffer_size];
    file.read(buffer, buffer_size);

    string fileHeader(buffer, file.gcount());

    size_t pdfPos = fileHeader.find("%PDF-1.");
    if (pdfPos != string::npos) {

        return true;
    }

    return false;
}

ErrorInfo File::firstline()
{
    ErrorInfo label;
    ifstream file(filename, ios::binary);

    string line;
    getline(file, line);
    string origin = line.substr(0, line.length() - 1);

    // 检查第一行是否是注释
    if (line.empty() || (line.substr(0, 2) != "//" && line.substr(0, 2) != "/*"))
    {
        label.message = "第一行[" + origin + "]不是注释语句";
        label.type = FirstLineNotComment;
        return label;
    }

    // 去掉注释符号
    if (line.substr(0, 2) == "//") {
        line = line.substr(2);
    }
    else if (line.substr(0, 2) == "/*") {
        line = line.substr(2);
        // 取 /**/ 之间的部分
        line = line.substr(0, line.find("*/"));
    }

    // 解析注释内容
    istringstream ss(line);
    string part;
    vector<string> parts;

    while (ss >> part)
    {
        parts.push_back(part);
    }

    // 检查是否有三个部分
    if (parts.size() != 3) {
        label.message = "注释语句组成缺失";
        label.type = FirstLineCommentMismatch;
        return label;
    }

    
    // 检查学号、姓名、班级是否匹配
    if (find(parts.begin(), parts.end(), this->no) == parts.end()) 
    {
        label.message = "注释[" + origin + "]中学号不匹配";
        label.type = FirstLineCommentMismatch;
        return label;
    }
    if (find(parts.begin(), parts.end(), this->name) == parts.end())
    {
        label.message = "注释[" + origin + "]中姓名不匹配";
        label.type = FirstLineCommentMismatch;
        return label;
    }
    if (find(parts.begin(), parts.end(), this->sclass) == parts.end())
    {
        label.message = "注释[" + origin + "]中班级不匹配";
        label.type = FirstLineCommentMismatch;
        return label;
    }

    label.message = "正确";
    label.type = Correct;
    return label;
}

void File::check_pair(vector<File> files)
{
    vector<int> mate = this->checkmate;

    for (auto& file : files)
    {
        auto it = find(mate.begin(), mate.end(), file.studentID);
        if (it != mate.end())
        {
            vector<int> other = file.checkmate;
            string label = file.no + "  " + file.name;
            if (intersect(mate, other) == false)
            {
                label += "   抛弃了你";
            }
            this->pairans.push_back(label);
        }
    }

}

void File::get_mate()
{
    ifstream file(filename, ios::binary);

    string line;
    getline(file, line);
    getline(file, line);

    this->checkmate = extractInt(line);
    file.close();
}

void File::check(string action)
{
    ErrorInfo label;
    label.type = Correct;
    label.message = "正确";

    string exten = get_exten(this->filename);
    if (exist() == false)
    {
        label.type = NotSubmitted;
        label.message = "未提交";
        this->error = label;
        return;
    }
    if (exten == "pdf" && is_pdf() == false)
    {
        label.type = ReportNotPDF;
        label.message = "报告格式不是PDF";
        this->error = label;
        return;
    }
    if (exten != "rar" && exten != "pdf" && is_ansi() == false)
    {
        label.type = CodeFormatIncorrect;
        label.message = "源文件格式不正确(非GB编码)";
        this->error = label;
        return;
    }

    if (exten != "rar" && exten != "pdf" && (action == "firstline" || action == "secondline"))
    {
        label = firstline();
        this->error = label;
        return;
    }
    this->error = label;
}

void File::print_error() const
{
    cout << setw(8)<<setiosflags(ios::left) << this->name << " 的 " << this->assignmentID << " " << error.message << endl;
}

void File::print_mateno() const
{
    for (auto& mate : checkmate)
        cout << "    " << mate << endl;
}

void File::print_pair_ans() const
{
    for (auto& ans : pairans)
        cout << "    " << ans << endl;
}
