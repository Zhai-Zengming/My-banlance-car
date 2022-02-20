#ifndef __ENCODE_H
#define __ENCODE_H

#include "stm32f10x.h"

#define Encode_TIM2_PERIOD              (65536-1)
#define Encode_TIM2_PSC                 (1-1)
#define Encode_TIM4_PERIOD              (65536-1)
#define Encode_TIM4_PSC                 (1-1)

void Encode_TIM_Init(void);
int Read_Speed(int TIMx);

#endif
 
