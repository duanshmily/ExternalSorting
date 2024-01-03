#ifndef INCLUDE_SHARED_DATA_H_
#define INCLUDE_SHARED_DATA_H_

namespace shareddata{

// 得到文件的数量
extern int FILE_COUNT;
// 判断文件是否为第一次打开
extern int IS_THE_FIRST_OPEN;

// 定义浮点数缓冲区大小
extern const int NUMBER_LENGTH;
// 定义缓冲区大小
extern const int MAX_BUFFER_SIZE;
// 定义最大归并路数
extern const int MAX_PUT_TOGETHER_WAY;

// 定义输入数据位置
extern const char INPUT_POSITION[];
// 定义合法的数据输出位置
extern const char LEAGAL_OUTPUT_POSITION[];
// 定义非法输出数据位置
extern const char ILLEGAL_OUTPUT_POSIONT[];

} // shareddata

#endif