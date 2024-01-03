#ifndef INCLUDE_AUXILIARY_FUNCTION_H_
#define INCLUDE_AUXILIARY_FUNCTION_H_

namespace auxiliaryfunction {

bool isFloatingPoint(const char *s, const int &n);
void sort(unsigned long long *a, unsigned long long *b, const int &n);
void getResultFile(const int &cur_folder_flag);
void putDigitIntoChar(char *s, const int &st, const int &ed, int &digit);
void putDigitIntoChar(char *s, const int &st, const int &ed, long long &digit, const int &digit_length);

class FileOutput {
public : 
    FileOutput();
    ~FileOutput();
    // 需要向文件中输出一个数据
    void OutputData(const char *a);

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