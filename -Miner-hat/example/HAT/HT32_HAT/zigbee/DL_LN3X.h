
#ifndef DL_LN3X_H_
#define DL_LN3X_H_

#include "sys.h"
#include "package.h"
//-------------关键函数------------------------
//理解接收程序稍微复杂,但使用接收程序只需要熟练掌握这两个函数

//这个函数用于发送一个包,如果发送成功返回done,否则返回fail,
//done和fail为lee.h中定义的宏
u8 sendPkg(sPkg* pkg);

//调用这个函数说明上一个包已经处理完了,并尝试接收一个包,
//如果已经收到包,则返回一个指向包的指针,否则返回null
//在处理完上一个包时请尽快调用此函数.以便释放上一个包的空间.
//这个程序需要被周期性的循环调用,否则可能错过待接收的包.
sPkg* getNextPkg(void);

#endif /* DL_LN3X_H_ */
