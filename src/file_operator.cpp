#include "file_operator.h"

#include <fstream>
#include <queue>

#include "file_input.h"
#include "file_output.h"
#include "shared_data.h"
#include "auxiliary_function.h"

namespace fileoperator {

void fileoperator::fileInitialize() {
    std::ifstream fin(shareddata::INPUT_POSITION, std::ios::in);
    if (!fin.is_open()) {
        puts("There is no file called \"in.txt\"");
        exit(-1);
    }
    fileoutput::FileOutput fout;
    fout.updateData(1, 0);
    FILE *illegalOutput = fopen(shareddata::ILLEGAL_OUTPUT_POSIONT, "w");

    // 表示当前数字是指数还是小数
    int cur_part_flag = 0, i = 0;
    // 表示浮点数缓冲区下标
    int bottom_idx = 0, exponent_idx = 0, double_buffer_idx = 0;
    // 表示读入缓冲区
    char *buffer = new char[shareddata::MAX_BUFFER_SIZE];
    // 表示浮点数保存缓冲区
    char *bottom = new char[shareddata::NUMBER_LENGTH];
    char *exponent = new char[shareddata::NUMBER_LENGTH];
    char *double_buffer = new char[shareddata::NUMBER_LENGTH];
    while (!fin.eof()) {
        fin.read(buffer, shareddata::MAX_BUFFER_SIZE - 1);
        buffer[fin.gcount()] = '\0';
        // 开始处理每一个浮点数
        for (i = 0; buffer[i]; ++ i) {
            // 遇到换行符说明遇到了一个浮点数
            if (buffer[i] == '\n') {
                bottom[bottom_idx] = '\0';
                exponent[exponent_idx] = '\0';
                double_buffer[double_buffer_idx] = '\0';
                if (auxiliaryfunction::isFloatingPoint(double_buffer, double_buffer_idx)) {
                    fout.OutputData(bottom, bottom_idx, exponent, exponent_idx);
                } else {
                    fprintf(illegalOutput, "%s\n", double_buffer);
                }
                cur_part_flag = 0;
                bottom_idx = exponent_idx = double_buffer_idx = 0;
            } else {
                if (buffer[i] == 'e' || buffer[i] == 'E') cur_part_flag = 1;
                else if (!cur_part_flag) bottom[bottom_idx ++ ] = buffer[i];
                else exponent[exponent_idx ++ ] = buffer[i];
                double_buffer[double_buffer_idx ++ ] = buffer[i];
            }
        }
    }
    delete[] buffer, delete[] double_buffer;
    delete[] bottom, delete[] exponent;
    buffer = double_buffer = nullptr;
    bottom = exponent = nullptr;
    fin.close(), fclose(illegalOutput);
}

int fileoperator::fileMerge() {
    int cur_folder_flag = 0, file_idx = 0, cnt = 0;
    int cur_file_count = shareddata::FILE_COUNT;
    int cur_merge_cnt = shareddata::MAX_PUT_TOGETHER_WAY;
    
    fileoutput::FileOutput fout;
    fileinput::FileInput *fin = new fileinput::FileInput[shareddata::MAX_PUT_TOGETHER_WAY];

    unsigned long long temp;
    std::priority_queue<std::pair<unsigned long long, int>, std::vector<std::pair<unsigned long long, int>>, std::greater<>> q;

    // 如果没有归并结束，就一直归并
    while (cur_file_count > 1) {
        // 新一轮的归并需要更新输出缓冲区的方向和相关信息
        fout.updateData(cur_merge_cnt, cur_folder_flag ^ 1);
        
        // 初始化归并操作，打开相应的归并文件
        cnt = 0;
        while (file_idx < cur_file_count && cnt < shareddata::MAX_PUT_TOGETHER_WAY) {
            fin[cnt].updateData(cur_folder_flag, file_idx ++ );
            fin[cnt].inputData(temp);
            q.emplace(temp, cnt ++ );
        }

        // 当前堆中还存在没有归并完的数据
        while (!q.empty()) {
            fout.OutputData(q.top().first);
            int idx = q.top().second;
            q.pop();
            if (!fin[idx].inputData(temp)) {
                // 如果还有文件可以打开
                if (file_idx < cur_file_count) {
                    fin[idx].updateData(cur_folder_flag, file_idx ++ );
                    fin[idx].inputData(temp);
                } else continue; // 没有文件可以打开了
            }
            q.emplace(temp, idx);
        }

        fout.Flush();
        
        cur_folder_flag ^= 1, file_idx = 0, cnt = 0;
        cur_file_count = shareddata::FILE_COUNT;
        cur_merge_cnt *= shareddata::MAX_PUT_TOGETHER_WAY;
    }
    delete[] fin;
    return cur_folder_flag;
}

} // fileoperator