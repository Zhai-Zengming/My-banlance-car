#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f10x.h"

extern int Fore, Back, Left, Right;
extern int uart_receive;           //蓝牙接收相关变量


void uart3_init(u32 bound);



#endif
