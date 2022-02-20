#ifndef __USART_H
#define __USART_H
#include "sys.h"
void usart1_send(u8 data);
void usart1_init(u32 pclk2,u32 bound);

void usart3_niming_report(u8 fun,u8*data,u8 len);
void Send_data(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8,float data9,float data10,float data11,float data12);
typedef unsigned char byte;
float b2f(byte m0, byte m1, byte m2, byte m3);

void usart2_send(u8 data);
void usart2_init(u32 pclk2,u32 bound);
void usart3_init(u32 pclk2,u32 bound);
void LANYAO_APP(int data);

#endif	   
