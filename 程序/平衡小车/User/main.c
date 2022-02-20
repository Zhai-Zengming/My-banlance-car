/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief  OLED����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
	*
  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "oled.h"
#include "./systick/bsp_SysTick.h"
#include "delay.h"
#include "./LED/led.h"
#include "./pid/pid.h"
#include "usart.h"	  
#include "./encode/encode.h"
#include "./bluetooth/bluetooth.h"
#include "./motor/motor.h"
#include "mpu6050.h"
#include "inv_mpu.h"
//#include "exti.h"

int PWM1,PWM2;
uint8_t stop_flag;
float pitch,roll,yaw; 		      // �����ǣ�����ǣ�ƫ����
short gyrox,gyroy,gyroz;        // ���ٶ�
short aacx,aacy,aacz;           // ���ٶ�
int uart_receive=0;           //����������ر���
int encode1,encode2;            // ������������ֵ
int Balance_Out = 0, Velocity_Out = 0, Turn_Out = 0;
int Fore, Back, Left, Right;
float Tgrget_Speed, Turn_Speed;                 //�ٶȻ�Ŀ���ٶ�,ת��Ŀ���ٶ�
uint8_t temperature;

void OLED_Display(void);

int main(void)
{
	delay_init();
	delay_ms(500);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//4����ռ,4����Ӧ��
	
	LED_Init();
	OLED_Init();
	OLED_Clear();
	
	uart_init(115200);
	uart3_init(9600);
	
	Encode_TIM_Init();
	Motor_Init(0);
	
	delay_ms(800);
	while(MPU_Init());
	/* ���MPU6050�Ƿ��ʼ���ɹ� */
	delay_ms(3000);
	while( mpu_dmp_init() )
	{
		char a;
		delay_ms(500);
        LED0= ~LED0;
		a = mpu_dmp_init();
		if(a == 0)
			break;
		OLED_ShowNumber(0, 0, a, 4, 12);
		OLED_ShowString(10, 4, (unsigned char*)"mpu init failure!", 12);
		
	}
	OLED_Clear();
	delay_ms(3000);
//	MPU6050_EXTI_Init();
	
	
	while(1)
	{
		mpu_dmp_get_data(&pitch, &roll, &yaw);
		/* ��ȡMPU��ص�ֵ */
//		temperature = (uint32_t)MPU_Get_Temperature/100;
		MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);
		MPU_Get_Accelerometer(&aacx, &aacy, &aacz);
		
//		printf("\n\rpitch = %f\nroll = %f\nyaw = %f\ntemperature = %d\r\n", pitch, roll, yaw, temperature);
		
		/* ��ȡ��������ֵ */
		encode1 = -Read_Speed(2); //�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
		encode2 = Read_Speed(4);   //���س����ң����Ϊe1
		
		if(Fore == 1)
		{
//			Fore = 0;
			Tgrget_Speed += 0.1;
			if(Tgrget_Speed > 30)
				Tgrget_Speed = 30;
		}
		else if(Back == 1)
		{
//			Back = 0;
			Tgrget_Speed -= 0.1;
			if(Tgrget_Speed < -30)
				Tgrget_Speed = -30;
		}
		else
			Tgrget_Speed = 0;
		if(Left == 1)
		{
//			Left = 0;
			Turn_Speed -= 2;
			if(Turn_Speed < -43)
				Turn_Speed = -43;
		}
		else if(Right == 1)
		{
//			Right = 0;
			Turn_Speed += 2;
			if(Turn_Speed > 43)
				Turn_Speed = 43;
		}
		else
			Turn_Speed = 0;
		
		Balance_Out = Balance(-9.8, pitch, gyroy);
		Velocity_Out = Velocity(encode1, encode2, Tgrget_Speed);
		Turn_Out = Turn(gyroz, Turn_Speed);
		
		PWM1 = Balance_Out + Velocity_Out + Turn_Out;
		PWM2 = Balance_Out + Velocity_Out - Turn_Out;
		
		stop_flag = Turn_Off(pitch);
		if(stop_flag)
		{
			OLED_Clear();
			OLED_ShowString(20, 4, (unsigned char*)"motor turn off!", 12);
			while( mpu_dmp_init() );
			delay_ms(5000);
			OLED_Clear();
		}
		else
		{
			Xianfu_Pwm();
			Set_Pwm(PWM1, PWM2);
//			OLED_Display();
		}
//		delay_ms(5);
	} 
}


void OLED_Display(void)
{
	OLED_Float(0, 0, Tgrget_Speed, 1);
	OLED_Float(1, 0, Turn_Speed, 1);
	
//    OLED_ShowString(0, 0, (unsigned char*)"Kp_B=", 12);
//	OLED_ShowNumber(40, 0, Balance_Kp, 4, 12);
//	
//	OLED_ShowString(0, 1, (unsigned char*)"Kd_B=", 12);
//	OLED_Float(1, 40, Balance_Kd, 1);              
//	
//	OLED_ShowString(0, 2, (unsigned char*)"Kp_V=", 12);
//	OLED_ShowNumber(40, 2, Velocity_Kp, 3, 12);
//	
//	OLED_ShowString(0, 3, (unsigned char*)"Ki_V=", 12);
//	OLED_Float( 3, 40, Velocity_Ki, 1);
//	
//	OLED_ShowString(0, 4, (unsigned char*)"Kp_T=", 12);
//	OLED_ShowNumber(40, 4, Turn_Kp, 3, 12);
//	
//	OLED_ShowString(0, 5, (unsigned char*)"pitch=", 12);
//	//OLED_ShowNumber(48, 5, pitch, 3, 12);
//	OLED_Float(5, 48, pitch, 1);
	
//	OLED_ShowString(0, 6, (unsigned char*)"yaw=", 12);
//	//OLED_ShowNumber(32, 6, yaw, 3, 12);
//	OLED_Float(6, 32, yaw, 1);
	
//	OLED_ShowString(0, 7, (unsigned char*)"pwm2=", 12);
//	//OLED_ShowNumber(32, 7, PWM1, 4, 12);
//	OLED_Float(7, 40, PWM2, 1);
	
//	OLED_ShowString(75, 0, (unsigned char*)"e1", 12);
//	OLED_Float(0, 91, encode1, 1);
//	
//	OLED_ShowString(75, 1, (unsigned char*)"e2", 12);
//	OLED_Float(1, 91, encode2, 1);
}









/*********************************************END OF FILE**********************/
