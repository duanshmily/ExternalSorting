#include "file_output.h"

#include <cstdio>
#include <filesystem>

#include "auxiliary_function.h"
#include "shared_data.h"

static int input_buffer_number = 0;

fileoutput::FileOutput::FileOutput() {
    this->pos_ = -1;
    this->buffer_cnt_ = 0;
    this->buffer_size_ = 0;
    this->cur_folder_flag_ = 0;
    this->buffer_ = nullptr;
    this->t_buffer_ = nullptr;
    this->cur_file_name_ = nullptr;
}

fileoutput::FileOutput::~FileOutput() {
    this->Flush();

    delete[] this->buffer_;
    delete[] this->t_buffer_;
    delete[] this->cur_file_name_;

    this->buffer_ = nullptr;
    this->t_buffer_ = nullptr;
}

void fileoutput::FileOutput::Flush() {
    if (input_buffer_number > 0 || this->buffer_size_ > 0) {
        input_buffer_number = this->buffer_cnt_;
        this->BufferOutputToFile();
    }
}

void fileoutput::FileOutput::updateData(const int &buffer_cnt, const int &folder_idx) {
    this->pos_ = -1;
    this->buffer_size_ = 0;
    this->buffer_cnt_ = buffer_cnt;
    
    this->cur_folder_flag_ = folder_idx;
    if (!this->cur_file_name_) this->cur_file_name_ = new char[15];
    sprintf(this->cur_file_name_, "%d/%d.dat", this->cur_folder_flag_, ++ this->pos_);

    input_buffer_number = 0;
    
    shareddata::FILE_COUNT = 0;
    
    if (!this->buffer_) this->buffer_ = new unsigned long long[shareddata::MAX_BUFFER_SIZE];
    if (shareddata::IS_THE_FIRST_OPEN) this->t_buffer_ = new unsigned long long[shareddata::MAX_BUFFER_SIZE];
    
    if (this->cur_folder_flag_ == 0) {
        std::filesystem::remove_all("0");
        std::filesystem::create_directory("0");
    } else {
        std::filesystem::remove_all("1");
        std::filesystem::create_directory("1");
    }
}

void fileoutput::FileOutput::BufferOutputToFile() {
    ++ input_buffer_number;

    if (shareddata::IS_THE_FIRST_OPEN) {
        auxiliaryfunction::sort(this->buffer_, this->t_buffer_, this->buffer_size_);
    }
    
    FILE *fout = fopen(this->cur_file_name_, "ab");
    fwrite(this->buffer_, sizeof(unsigned long long), this->buffer_size_, fout);
    fclose(fout);

    if (input_buffer_number >= this->buffer_cnt_) {
        shareddata::FILE_COUNT ++ ;
        sprintf(this->cur_file_name_, "%d/%d.dat", this->cur_folder_flag_, ++ this->pos_);
        input_buffer_number = 0;
    }

    this->buffer_size_ = 0;
}

void fileoutput::FileOutput::OutputData(const unsigned long long &a) {
    this->buffer_[this->buffer_size_ ++ ] = a;
    
    if (this->buffer_size_ >= shareddata::MAX_BUFFER_SIZE) {
        this->BufferOutputToFile();
    }
}

void fileoutput::FileOutput::OutputData(const char *bottom, const int &n1, const char *exponent, const int &n2) {
    // 获得指数符号
    int temp_sign_1 = 1, i = 0;
    if (exponent[0] == '-' || exponent[0] == '+') temp_sign_1 = (exponent[0] == '+') ? 1 : -1;
    
    // 获得指数部分
    int exponent_part_ = 0;
    for (i = exponent[0] == '-' || exponent[0] == '+'; i < n2; ++ i) {
        exponent_part_ = exponent_part_ * 10 + (exponent[i] - '0');
    }
    exponent_part_ *= temp_sign_1;

    // 获得底数符号
    int sign_ = 1;
    if (bottom[0] == '-' || bottom[0] == '+') sign_ = (bottom[0] == '+') ? 1 : -1;

    // 得到底数，找到小数点位置同时得到第一个有效位的位置
    temp_sign_1 = 0;
    long long bottom_part_ = 0;
    int i_no_zero = n1, i_point = n1, temp_sign_2 = 0;
    for (i = bottom[0] == '-' || bottom[0] == '+'; i < n1; ++ i) {
        if (bottom[i] == '.') i_point = i;
        else if (!temp_sign_1 && bottom[i] != '0') {
            i_no_zero = i;
            temp_sign_1 = 1;
        }
        if (!temp_sign_2 && bottom[i] != '.') {
            // 添加到底数中
            bottom_part_ = bottom_part_ * 10ll + (bottom[i] - '0');
            // 如果达到了有效位数，则判断四舍五入
            if (bottom_part_ > 99999999999ll) {
                bottom_part_ = (bottom_part_ + 5) / 10;
                // 判断四舍五入是否产生了进位
                if (bottom_part_ > 99999999999ll) {
                    ++ exponent_part_;
                    bottom_part_ = 10000000000ll;
                }
                temp_sign_2 = 1;
            }
        }
    }
    if (i_no_zero > i_point) exponent_part_ -= i_no_zero - i_point - 1;
    else exponent_part_ += i_point - i_no_zero;
    
    // 将0特判掉
    if (bottom_part_ == 0) exponent_part_ = 0;

    exponent_part_ = std::min(exponent_part_, 1000);
    exponent_part_ = std::max(exponent_part_, -998);

    auto &sort_flag_ = this->buffer_[this->buffer_size_ ++ ];
    sort_flag_ = (sign_ != -1 ? 2 : 1) * 1000000000000000ll + 
                (exponent_part_ + 1000) * 100000000000ll + bottom_part_;
    if (sign_ == -1) sort_flag_ *= -1;
    sort_flag_ += 2200099999999999ll;

    if (this->buffer_size_ >= shareddata::MAX_BUFFER_SIZE) {
        this->BufferOutputToFile();
    }
}