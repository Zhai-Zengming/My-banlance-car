#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 
#define PWMA   TIM1->CCR1 
#define INA1     PBout(14)  
#define INA2     PBout(15)
void Motor_PWM_Init(u16 arr,u16 psc);
void Steering_engine_PWM_Init(u16 arr,u16 psc);
void Motor_Init(void);
#endif
