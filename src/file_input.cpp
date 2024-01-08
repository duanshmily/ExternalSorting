#include "file_input.h"

#include <cstdio>
#include <cstdlib>

#include "shared_data.h"

fileinput::FileInput::FileInput() {
    this->fin = nullptr;
    this->buffer_idx_ = 0;
    this->buffer_size_ = 0;
    this->cur_file_name_ = nullptr;
    this->buffer_ = nullptr;
}

fileinput::FileInput::~FileInput() {
    fclose(this->fin);
    
    delete[] this->buffer_;
    delete[] this->cur_file_name_;

    this->buffer_ = nullptr;
    this->cur_file_name_ = nullptr;
}

int fileinput::FileInput::inputData(unsigned long long &in) {
    if (this->buffer_idx_ == this->buffer_size_) {
        // 尝试从当前文件中继续读取
        updateData();
        if (this->buffer_size_ == 0) {
            fclose(fin);
            return 0;
        }
    }
    in = this->buffer_[this->buffer_idx_ ++ ];
    return 1;
}

void fileinput::FileInput::updateData() {
    this->buffer_idx_ = 0;
    this->buffer_size_ = fread(this->buffer_, sizeof(unsigned long long), shareddata::MAX_BUFFER_SIZE, this->fin);
}

void fileinput::FileInput::updateData(const int &folder_idx, const int &file_idx) {
    if (!this->cur_file_name_) this->cur_file_name_ = new char[15];
    sprintf(this->cur_file_name_, "%d/%d.dat", folder_idx, file_idx);

    this->fin = fopen(this->cur_file_name_, "rb");
    if (!fin) {
        printf("There is no file called %s\n", this->cur_file_name_);
        exit(-1);
    }
    
    if (!this->buffer_) this->buffer_ = new unsigned long long[shareddata::MAX_BUFFER_SIZE];
    
    updateData();
}