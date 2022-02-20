#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
#include "sys.h"

/* 定时器1 */
#define ADVANCE_TIM                     TIM1
#define ADVANCE_TIM_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define ADVANCE_TIM_CLK                 RCC_APB2Periph_TIM1

#define ADVANCE_TIM_GPIO_CLK            RCC_APB2Periph_GPIOA
#define ADVANCE_TIM_PORT                GPIOA
#define ADVANCE_TIM_CH1_PIN             GPIO_Pin_8
#define ADVANCE_TIM_CH4_PIN             GPIO_Pin_11

///* 定时器3 */
//#define GENERAL_TIM                     TIM3
//#define GENERAL_TIM_APBxClock_FUN       RCC_APB1PeriphClockCmd
//#define GENERAL_TIM_CLK                 RCC_APB1Periph_TIM3

//#define GENERAL_TIM_CH4_GPIO_CLK        RCC_APB2Periph_GPIOB
//#define GENERAL_TIM_CH4_PORT            GPIOB
//#define GENERAL_TIM_CH4_PIN             GPIO_Pin_1



/* 控制正反转GPIO */
#define Motor_BIN2_Pin                  GPIO_Pin_12
#define Motor_BIN1_Pin                  GPIO_Pin_13
#define Motor_AIN2_Pin                  GPIO_Pin_15
#define Motor_AIN1_Pin                  GPIO_Pin_14
#define AIN2                            PBout(15)
#define AIN1                            PBout(14)
#define BIN1                            PBout(13)
#define BIN2                            PBout(12)
#define Motor_GPIO_PORT                 GPIOB

/* TIM1产生PWM */
#define ADVANCE_TIM_PERIOD              (7200-1)
#define ADVANCE_TIM_PSC                 (1-1)

extern int PWM1,PWM2;
extern uint8_t stop_flag;

void Motor_Init(u16 pulse);
void Xianfu_Pwm(void);
u8 Turn_Off(signed int angle);
void Set_Pwm(int PWM1, int PWM2);
int myabs(int a);

#endif
