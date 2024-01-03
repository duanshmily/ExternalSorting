#ifndef INCLUDE_FILE_INPUT_H_
#define INCLUDE_FILE_INPUT_H_

#include <cstdio>

namespace fileinput {

class FileInput {
public :
    FileInput();
    ~FileInput();
    // 得到当前缓冲区下的文件
    int inputData(unsigned long long &in);
    // 打开新的文件
    void updateData(const int &folder_idx, const int &file_idx);
private :
    // 文件指针
    FILE *fin;
    // 缓冲区指针
    int buffer_idx_;
    // 缓冲区当前可以元素大小
    int buffer_size_;
    // 当前打开的文件的名称
    char *cur_file_name_;
    // 输入缓冲区
    unsigned long long *buffer_;
    // 更新输入缓冲区中的数据
    void updateData();
};

}// fileinput

#endif