#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"

#define MPU6050_INT PBin(14)   //PB5���ӵ�MPU6050���ж�����
void EXTI_Init(void);	//�ⲿ�жϳ�ʼ��	
int EXTI15_10_IRQHandler(void);//�жϺ���
#endif





















