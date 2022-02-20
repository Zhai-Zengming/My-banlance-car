#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"

#define MPU6050_INT PBin(14)   //PB5连接到MPU6050的中断引脚
void EXTI_Init(void);	//外部中断初始化	
int EXTI15_10_IRQHandler(void);//中断函数
#endif





















