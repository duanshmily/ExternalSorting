#include <chrono>
#include <cstdio>

#include "auxiliary_function.h"
#include "file_operator.h"
#include "shared_data.h"

void solve() {
    puts("File initialization.....");
    auto start = std::chrono::high_resolution_clock::now();
    fileoperator::fileInitialize();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    printf("Program runtime : %lld seconds\n", duration.count());

    puts("Merge files......");
    shareddata::IS_THE_FIRST_OPEN = 0;
    start = std::chrono::high_resolution_clock::now();
    int res_folder_flag = fileoperator::fileMerge();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    printf("Program runtime : %lld seconds\n", duration.count());

    puts("Processing results......");
    start = std::chrono::high_resolution_clock::now();
    auxiliaryfunction::getResultFile(res_folder_flag);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    printf("Program runtime : %lld seconds\n", duration.count());
}

signed main() {
    auto start = std::chrono::high_resolution_clock::now();
    solve();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    printf("Program runtime : %lld seconds\n", duration.count());
    return 0;
}