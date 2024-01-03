#include "shared_data.h"

namespace shareddata {

// 得到文件的数量
int FILE_COUNT = 0;
// 判断文件是否为第一次打开
int IS_THE_FIRST_OPEN = 1;


// 定义浮点数缓冲区大小
const int NUMBER_LENGTH = 50;
// 定义缓冲区大小
const int MAX_BUFFER_SIZE = 5000000;
// 定义最大归并路数
const int MAX_PUT_TOGETHER_WAY = 6;

// 定义输入数据位置
const char INPUT_POSITION[] = "./in.txt";
// 定义合法的数据输出位置
const char LEAGAL_OUTPUT_POSITION[] = "legal.txt";
// 定义非法输出数据位置
const char ILLEGAL_OUTPUT_POSIONT[] = "illegal.txt";

} // sharedddata