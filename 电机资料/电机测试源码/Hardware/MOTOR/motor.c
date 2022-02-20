#include "motor.h"
void Motor_Init(void)
{

	RCC->APB2ENR|=1<<3;       //PORTB时钟使能 
	

	GPIOB->CRH&=0X00FFFFFF;   //
	GPIOB->CRH|=0X22000000;   //PORTB14 15输出	
	

	  GPIOB->ODR|=0<<14; // XIA拉	
	  GPIOB->ODR|=0<<15; // XIA拉	
}
void Motor_PWM_Init(u16 arr,u16 psc)
{		
  Motor_Init();	
	
	RCC->APB2ENR|=1<<11;       //使能TIM1时钟
	RCC->APB2ENR|=1<<2;        //PORTA时钟使能
	GPIOA->CRH&=0XFFFF0FF0;    //PORTA8 11复用输出
	GPIOA->CRH|=0X0000B00B;    //PORTA8 11复用输出
		
	TIM1->ARR=arr;             //设定计数器自动重装值 
	TIM1->PSC=psc;             //预分频器不分频
			
	TIM1->CCMR1|=6<<4;         //CH1 PWM2模式	
	
	TIM1->CCMR2|=6<<12;        //CH4 PWM2模式	
	
	TIM1->CCMR1|=1<<3;         //CH1预装载使能	  
	  
	TIM1->CCMR2|=1<<11;        //CH4预装载使能	  
	
	TIM1->CCER|=1<<0;         //CH1输出使能	
 
	TIM1->CCER|=1<<12;        //CH4输出使能
	TIM1->CCR1=0;

	TIM1->CCR4=0;
	TIM1->BDTR |= 1<<15;       //TIM1必须要这句话才能输出PWM
	TIM1->CR1=0x80;          //ARPE使能 
	TIM1->CR1|=0x01;          //使能定时器	
} 
void Steering_engine_PWM_Init(u16 arr,u16 psc)
{		 					 
////定时器1 PA8 PA11
//	RCC->APB2ENR|=1<<11;       //使能TIM1时钟
//	RCC->APB2ENR|=1<<2;        //PORTA时钟使能
//	GPIOA->CRH&=0XFFFF0FF0;    //PORTA8 11复用输出
//	GPIOA->CRH|=0X0000B00B;    //PORTA8 11复用输出
//		
//	TIM1->ARR=arr;             //设定计数器自动重装值 
//	TIM1->PSC=psc;             //预分频器不分频
//			
//	TIM1->CCMR1|=6<<4;         //CH1 PWM2模式	
//	
//	TIM1->CCMR2|=6<<12;        //CH4 PWM2模式	
//	
//	TIM1->CCMR1|=1<<3;         //CH1预装载使能	  
//	  
//	TIM1->CCMR2|=1<<11;        //CH4预装载使能	  
//	
//	TIM1->CCER|=1<<0;         //CH1输出使能	
// 
//	TIM1->CCER|=1<<12;        //CH4输出使能
//	TIM1->CCR1=0;

//	TIM1->CCR4=0;
//	TIM1->BDTR |= 1<<15;       //TIM1必须要这句话才能输出PWM
//	TIM1->CR1=0x8000;          //ARPE使能 
//	TIM1->CR1|=0x01;          //使能定时器	
//	
////定时器4 PB6 PB7
//	RCC->APB1ENR|=1<<2;       //使能TIM4时钟
//	RCC->APB2ENR|=1<<3;        //PORTB时钟使能
//	GPIOB->CRL&=0X00FFFFFF;    //PORTB6 7复用输出
//	GPIOB->CRL|=0XBB000000;    //PORTB6 7复用输出
//		
//	TIM4->ARR=arr;             //设定计数器自动重装值 
//	TIM4->PSC=psc;             //预分频器不分频
//			
//	TIM4->CCMR1|=6<<4;         //CH1 PWM1模式	
//	TIM4->CCMR1|=6<<12;        //CH2 PWM1模式	
//	
//	TIM4->CCMR1|=1<<3;         //CH1预装载使能	  
//	TIM4->CCMR1|=1<<11;        //CH2预装载使能		  
//	
//	TIM4->CCER|=1<<0;         //CH1输出使能	
//	TIM4->CCER|=1<<4;         //CH2输出使能	  
//	
//	TIM4->CCR1=0;
//	TIM4->CCR2=0;
//	TIM4->CR1=0x80;          //ARPE使能 
//	TIM4->CR1|=0x01;          //使能定时器		
//	
////定时器3 PA6 PA7 PB0 PB1
//	RCC->APB1ENR|=1<<1;       //TIM3时钟使能    
//	RCC->APB2ENR|=1<<2;       //PORTA时钟使能 
//	RCC->APB2ENR|=1<<3;       //PORTB时钟使能 
//	GPIOA->CRL&=0X00FFFFFF;   //PORTA6 7复用输出
//	GPIOA->CRL|=0XBB000000;   //PORTB6 7复用输出
//	GPIOB->CRL&=0XFFFFFF00;   //PORTB0 1复用输出
//	GPIOB->CRL|=0X000000BB;   //PORTB0 1复用输出
//	TIM3->ARR=arr;//设定计数器自动重装值 
//	TIM3->PSC=psc;//预分频器不分频
//	TIM3->CCMR1|=6<<4;         //CH1 PWM1模式	
//	TIM3->CCMR1|=6<<12;        //CH2 PWM1模式	
//	TIM3->CCMR2|=6<<4;         //CH3 PWM1模式	
//	TIM3->CCMR2|=6<<12;        //CH4 PWM1模式	
//	
//	TIM3->CCMR1|=1<<3;         //CH1预装载使能	  
//	TIM3->CCMR1|=1<<11;        //CH2预装载使能	 
//	TIM3->CCMR2|=1<<3;         //CH3预装载使能	  
//	TIM3->CCMR2|=1<<11;        //CH4预装载使能	  
//	
//	TIM3->CCER|=1<<0;         //CH1输出使能	
//	TIM3->CCER|=1<<4;         //CH2输出使能	   
//	TIM3->CCER|=1<<8;         //CH3输出使能	 
//	TIM3->CCER|=1<<12;        //CH4输出使能

//	TIM3->CCR1=0;
//	TIM3->CCR2=0;
//	TIM3->CCR3=0;
//	TIM3->CCR4=0;
//	
//	TIM3->CR1=0x80;          //ARPE使能 
//	TIM3->CR1|=0x01;          //使能定时器3
} 
