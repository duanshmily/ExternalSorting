#ifndef INCLUDE_AUXILIARY_FUNCTION_H_
#define INCLUDE_AUXILIARY_FUNCTION_H_

namespace auxiliaryfunction {

// 判断一个浮点数是否符合要求
bool isFloatingPoint(const char *s, const int &n);
// 对一个缓冲区的整合后的浮点数字进行基数排序
void sort(unsigned long long *a, unsigned long long *b, const int &n);
// 将二进制文件转化为文本文件同时对输出进行格式化
void getResultFile(const int &cur_folder_flag);
// 将指数格式化输出到数组s中
void putDigitIntoChar(char *s, const int &st, const int &ed, int &digit);
// 将底数格式化输出到数组s中
void putDigitIntoChar(char *s, const int &st, const int &ed, long long &digit, const int &digit_length);

class FileOutput {
public : 
    FileOutput();
    ~FileOutput();
    // 需要向文件中输出一个数据
    void OutputData(const char *s);

private :
    // buffer_size_表示的是缓冲区的当前大小
    int buffer_size_;
    // 定义缓冲区指针
    char *buffer_;
    // 将缓冲区内的所有数据输出到文件中
    void BufferOutputToFile();
    // 将缓冲区清空
    void Flush();
};

}// stringoperator

#endif