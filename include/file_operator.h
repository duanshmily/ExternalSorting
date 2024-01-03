#ifndef INCLUDE_FILE_OPERATOR_H_
#define INCLUDE_FILE_OPERATOR_H_

namespace fileoperator {

// 将大文件拆分为小文件的函数
void fileInitialize();
// 将小文件进行归并操作
int fileMerge();

} // fileoperator

#endif