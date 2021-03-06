#include "exti.h"

/**************************************************************************
函数功能：外部中断初始化
入口参数：无
返回  值：无 
**************************************************************************/
void EXTI_Init(void)
{
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	 
	GPIOB->CRH&=0XF0FFFFFF; 
	GPIOB->CRH|=0X08000000;//PB14上拉输入
  GPIOB->ODR|=1<<14;      //PB14上拉	
	Ex_NVIC_Config(GPIO_B,14,FTIR);		//下降沿触发
	MY_NVIC_Init(2,1,EXTI15_10_IRQn,2);  	//抢占2，子优先级1，组2
}










