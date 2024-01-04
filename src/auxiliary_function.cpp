#include "auxiliary_function.h"

#include <fstream>

#include "file_input.h"
#include "shared_data.h"

namespace auxiliaryfunction {

auxiliaryfunction::FileOutput::FileOutput() {
    this->buffer_size_ = 0;
    this->buffer_ = new char[shareddata::MAX_BUFFER_SIZE];
}

auxiliaryfunction::FileOutput::~FileOutput() {
    this->Flush();

    delete[] this->buffer_;
    this->buffer_ = nullptr;
}

void auxiliaryfunction::FileOutput::Flush() {
    if (this->buffer_size_ > 0) this->BufferOutputToFile();
}

void auxiliaryfunction::FileOutput::BufferOutputToFile() {
    std::ofstream fout(shareddata::LEAGAL_OUTPUT_POSITION, std::ios::app);
    fout.write(this->buffer_, this->buffer_size_);
    fout.close();

    this->buffer_size_ = 0;
}

void auxiliaryfunction::FileOutput::OutputData(const char *s) {
    for (int i = 0; s[i]; ++ i) {
        this->buffer_[this->buffer_size_ ++ ] = s[i];
        if (this->buffer_size_ >= shareddata::MAX_BUFFER_SIZE) {
            this->BufferOutputToFile();
        }
    }
}

bool auxiliaryfunction::isFloatingPoint(const char *s, const int &n) {
    int i = 0;
    auto consumeDigits = [&]() { 
        while (i < n && isdigit(s[i])) ++ i; 
    };
    if (i < n && (s[i] == '+' || s[i] == '-')) ++ i;
    consumeDigits();
    if (i < n && s[i] == '.') {
        ++ i;
        consumeDigits();
    }
    if (i < n && (s[i] == 'e' || s[i] == 'E')) {
        ++ i;
        if (i < n && (s[i] == '+' || s[i] == '-')) ++ i;
        consumeDigits();
    }
    return i == n;
}

void auxiliaryfunction::sort(unsigned long long *a, unsigned long long *b, const int &n) {
    int s1[65536] = {0}, s2[65536] = {0}, s3[65536] = {0}, s4[65536] = {0}, i = 0;
    for (i = 0; i < n; ++ i) {
        ++ s1[a[i] & 65535];
        ++ s2[a[i] >> 16 & 65535];
        ++ s3[a[i] >> 32 & 65535];
        ++ s4[a[i] >> 48 & 65535];
    }
    for (i = 1; i < 65536; ++ i) {
        s1[i] += s1[i - 1];
        s2[i] += s2[i - 1];
        s3[i] += s3[i - 1];
        s4[i] += s4[i - 1];
    }
    for (i = n - 1; i >= 0; -- i) {
        b[-- s1[a[i] & 65535]] = a[i];
    }
    for (i = n - 1; i >= 0; -- i) {
        a[-- s2[b[i] >> 16 & 65535]] = b[i];
    }
    for (i = n - 1; i >= 0; -- i) {
        b[-- s3[a[i] >> 32 & 65535]] = a[i];
    }
    for (i = n - 1; i >= 0; -- i) {
        a[-- s4[b[i] >> 48 & 65535]] = b[i];
    }
}

void auxiliaryfunction::putDigitIntoChar(char *s, const int &st, const int &ed, long long &digit, const int &digit_length) {
    int i = st + digit_length - 1;
    while (digit) {
        s[i -- ] = digit % 10 + '0';
        digit /= 10;
    }
    i = st + digit_length;
    while (i <= ed) s[i ++ ] = '0';
}

void auxiliaryfunction::putDigitIntoChar(char *s, const int &st, const int &ed, int &digit) {
    int i = ed;
    while (digit) {
        s[i -- ] = digit % 10 + '0';
        digit /= 10;
    }
    while (i >= st) s[i -- ] = '0';
}

void auxiliaryfunction::getResultFile(const int &cur_folder_flag) {
    char *double_buffer = new char[20];
    char *t_double_buffer = new char[20];

    fileinput::FileInput fin;
    fin.updateData(cur_folder_flag, 0);
    auxiliaryfunction::FileOutput fout;

    long long temp2 = 0;
    unsigned long long temp1 = 0;
    
    int i = 0, cnt = 0, cur_part_flag = 0;
    int bottom_fraction_length = 0;
    
    int exponent = 0;
    long long bottom_fraction = 0;

    while (fin.inputData(temp1)) {
        // 得到该浮点数原整合数字
        temp2 = (long long) temp1 - 2200099999999999ll;
        if (temp2 < 0) temp2 = -temp2;

        i = 16;
        while (temp2) {
            double_buffer[-- i] = temp2 % 10 + '0';
            temp2 /= 10;
        }

        t_double_buffer[1] = '0';
        t_double_buffer[0] = double_buffer[0] != '1' ? '+' : '-';
        for (i = 1; i < 16; ++ i) {
            if (!cur_part_flag) {
                exponent = exponent * 10 + double_buffer[i] - '0';
                ++ cnt;
            } else {
                if (!cnt && double_buffer[i] - '0' != 0) {
                    t_double_buffer[1] = double_buffer[i];
                    ++ cnt;
                } else if (cnt) {
                    bottom_fraction = bottom_fraction * 10 + double_buffer[i] - '0';
                    if (bottom_fraction) ++ bottom_fraction_length;
                    ++ cnt;
                }
            }
            
            if (!cur_part_flag && cnt == 4) cnt = 0, cur_part_flag = 1;
        }
        // 得到映射之前的指数值
        exponent -= 1001;
        
        if (t_double_buffer[1] == '0' && bottom_fraction == 0) exponent = 0;

        t_double_buffer[2] = '.';
        putDigitIntoChar(t_double_buffer, 3, 12, bottom_fraction, bottom_fraction_length);
        t_double_buffer[13] = 'E';
        t_double_buffer[14] = exponent >= 0 ? '+' : '-';
        if (exponent < 0) exponent = -exponent;
        putDigitIntoChar(t_double_buffer, 15, 17, exponent);
        t_double_buffer[18] = '\n';
        t_double_buffer[19] = '\0';

        fout.OutputData(t_double_buffer);

        cnt = cur_part_flag = 0;
        exponent = bottom_fraction = 0;
        bottom_fraction_length = 0;
    }
    delete[] double_buffer;
    delete[] t_double_buffer;

    double_buffer = nullptr;
    t_double_buffer = nullptr;
}

} // auxiliaryfunction