#ifndef INCLUDE_FILE_OUTPUT_H_
#define INCLUDE_FILE_OUTPUT_H_

namespace fileoutput {

class FileOutput {
public : 
    FileOutput();
    ~FileOutput();
    // 将缓冲区清空
    void Flush();
    // 需要向文件中输出一个数据
    void OutputData(const unsigned long long &a);
    // 需要想文件中输出一个数据
    void OutputData(const char *bottom, const int &n1, const char *exeponent ,const int &n2);
    // 用来更换输出文件夹
    void updateData(const int &buffer_cnt, const int &cur_folder_idx);

private :
    // pos_表示的是当前输出文件的编号
    int pos_;
    // 表示多少个缓冲区达到一个文件
    int buffer_cnt_;
    // buffer_size_表示的是缓冲区的当前大小
    int buffer_size_;
    // 定义当前输出文件夹的编号
    int cur_folder_flag_;
    // 定义缓冲区指针
    unsigned long long *buffer_;
    // 定义排序临时换种区指针
    unsigned long long *t_buffer_;
    // 表示的是当前应该输出到的文件名字
    char *cur_file_name_;
    // 将缓冲区内的所有数据输出到文件中
    void BufferOutputToFile();
};

} // fileoutput

#endif