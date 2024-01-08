#include "auxiliary_function.h"

#include <fstream>

#include "file_input.h"
#include "shared_data.h"

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

void auxiliaryfunction::getResultFile(const int &cur_folder_flag) {
    char *double_buffer = new char[20];

    fileinput::FileInput fin;
    fin.updateData(cur_folder_flag, 0);
    auxiliaryfunction::FileOutput fout;

    long long temp2 = 0;
    unsigned long long temp1 = 0;
    
    int i = 0, exponent = 0;
    long long power = 1000000000000000ll;

    while (fin.inputData(temp1)) {
        // 得到该浮点数原整合数字
        temp2 = (long long) temp1 - 3000000000000000ll;
        if (temp2 < 0) temp2 = -temp2;

        double_buffer[0] = (temp2 / power) % 10 == 2 ? '+' : '-';
        for (i = 1; i <= 4; ++ i) {
            power /= 10;
            exponent = exponent * 10 + (temp2 / power) % 10;
        }
        exponent -= 1001;

        for (i = 2; i <= 12; ++ i) {
            power /= 10;
            if (power) double_buffer[i] = (temp2 / power) % 10 + '0';
            else double_buffer[i] = '0';
        }
        double_buffer[1] = double_buffer[2];
        double_buffer[2] = '.';

        if (double_buffer[1] == double_buffer[3] && double_buffer[1] == '0') exponent = 0;

        double_buffer[13] = 'E';
        if (exponent < 0) double_buffer[14] = '-', exponent *= -1;
        else double_buffer[14] = '+';

        i = 17;
        while (i >= 15) {
            if (exponent) double_buffer[i -- ] = exponent % 10 + '0';
            else double_buffer[i -- ] = '0';
            exponent /= 10;
        }

        double_buffer[18] = '\n';
        double_buffer[19] = '\0';

        fout.OutputData(double_buffer);

        exponent = 0;
        power = 1000000000000000ll;
    }
    delete[] double_buffer;

    double_buffer = nullptr;
}