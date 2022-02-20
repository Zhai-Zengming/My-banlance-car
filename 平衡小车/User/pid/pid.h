#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"

#define Balance_Kp        448  //550*0.6       //480*0.6
#define Balance_Kd        2.41//2.5 //2.8*0.6   //2      //3.5*0.6
#define Velocity_Kp       220//420//190
#define Velocity_Ki       1.1//2.1//0.95
//#define Turn_Kp           (-3)

extern int uart_receive;           //蓝牙接收相关变量
extern int encode1,encode2;            // 两个编码器数值


int Balance(float target, float Angle, float gyro_Y);
int Velocity(int Encoder_Left, int Encoder_Right, int Tgrget_Speed);
int Turn(float gyroz, float Turn_Speed);



#endif

