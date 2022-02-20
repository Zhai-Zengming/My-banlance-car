#include "motor.h"
void Motor_Init(void)
{

	RCC->APB2ENR|=1<<3;       //PORTBʱ��ʹ�� 
	

	GPIOB->CRH&=0X00FFFFFF;   //
	GPIOB->CRH|=0X22000000;   //PORTB14 15���	
	

	  GPIOB->ODR|=0<<14; // XIA��	
	  GPIOB->ODR|=0<<15; // XIA��	
}
void Motor_PWM_Init(u16 arr,u16 psc)
{		
  Motor_Init();	
	
	RCC->APB2ENR|=1<<11;       //ʹ��TIM1ʱ��
	RCC->APB2ENR|=1<<2;        //PORTAʱ��ʹ��
	GPIOA->CRH&=0XFFFF0FF0;    //PORTA8 11�������
	GPIOA->CRH|=0X0000B00B;    //PORTA8 11�������
		
	TIM1->ARR=arr;             //�趨�������Զ���װֵ 
	TIM1->PSC=psc;             //Ԥ��Ƶ������Ƶ
			
	TIM1->CCMR1|=6<<4;         //CH1 PWM2ģʽ	
	
	TIM1->CCMR2|=6<<12;        //CH4 PWM2ģʽ	
	
	TIM1->CCMR1|=1<<3;         //CH1Ԥװ��ʹ��	  
	  
	TIM1->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	  
	
	TIM1->CCER|=1<<0;         //CH1���ʹ��	
 
	TIM1->CCER|=1<<12;        //CH4���ʹ��
	TIM1->CCR1=0;

	TIM1->CCR4=0;
	TIM1->BDTR |= 1<<15;       //TIM1����Ҫ��仰�������PWM
	TIM1->CR1=0x80;          //ARPEʹ�� 
	TIM1->CR1|=0x01;          //ʹ�ܶ�ʱ��	
} 
void Steering_engine_PWM_Init(u16 arr,u16 psc)
{		 					 
////��ʱ��1 PA8 PA11
//	RCC->APB2ENR|=1<<11;       //ʹ��TIM1ʱ��
//	RCC->APB2ENR|=1<<2;        //PORTAʱ��ʹ��
//	GPIOA->CRH&=0XFFFF0FF0;    //PORTA8 11�������
//	GPIOA->CRH|=0X0000B00B;    //PORTA8 11�������
//		
//	TIM1->ARR=arr;             //�趨�������Զ���װֵ 
//	TIM1->PSC=psc;             //Ԥ��Ƶ������Ƶ
//			
//	TIM1->CCMR1|=6<<4;         //CH1 PWM2ģʽ	
//	
//	TIM1->CCMR2|=6<<12;        //CH4 PWM2ģʽ	
//	
//	TIM1->CCMR1|=1<<3;         //CH1Ԥװ��ʹ��	  
//	  
//	TIM1->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	  
//	
//	TIM1->CCER|=1<<0;         //CH1���ʹ��	
// 
//	TIM1->CCER|=1<<12;        //CH4���ʹ��
//	TIM1->CCR1=0;

//	TIM1->CCR4=0;
//	TIM1->BDTR |= 1<<15;       //TIM1����Ҫ��仰�������PWM
//	TIM1->CR1=0x8000;          //ARPEʹ�� 
//	TIM1->CR1|=0x01;          //ʹ�ܶ�ʱ��	
//	
////��ʱ��4 PB6 PB7
//	RCC->APB1ENR|=1<<2;       //ʹ��TIM4ʱ��
//	RCC->APB2ENR|=1<<3;        //PORTBʱ��ʹ��
//	GPIOB->CRL&=0X00FFFFFF;    //PORTB6 7�������
//	GPIOB->CRL|=0XBB000000;    //PORTB6 7�������
//		
//	TIM4->ARR=arr;             //�趨�������Զ���װֵ 
//	TIM4->PSC=psc;             //Ԥ��Ƶ������Ƶ
//			
//	TIM4->CCMR1|=6<<4;         //CH1 PWM1ģʽ	
//	TIM4->CCMR1|=6<<12;        //CH2 PWM1ģʽ	
//	
//	TIM4->CCMR1|=1<<3;         //CH1Ԥװ��ʹ��	  
//	TIM4->CCMR1|=1<<11;        //CH2Ԥװ��ʹ��		  
//	
//	TIM4->CCER|=1<<0;         //CH1���ʹ��	
//	TIM4->CCER|=1<<4;         //CH2���ʹ��	  
//	
//	TIM4->CCR1=0;
//	TIM4->CCR2=0;
//	TIM4->CR1=0x80;          //ARPEʹ�� 
//	TIM4->CR1|=0x01;          //ʹ�ܶ�ʱ��		
//	
////��ʱ��3 PA6 PA7 PB0 PB1
//	RCC->APB1ENR|=1<<1;       //TIM3ʱ��ʹ��    
//	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 
//	RCC->APB2ENR|=1<<3;       //PORTBʱ��ʹ�� 
//	GPIOA->CRL&=0X00FFFFFF;   //PORTA6 7�������
//	GPIOA->CRL|=0XBB000000;   //PORTB6 7�������
//	GPIOB->CRL&=0XFFFFFF00;   //PORTB0 1�������
//	GPIOB->CRL|=0X000000BB;   //PORTB0 1�������
//	TIM3->ARR=arr;//�趨�������Զ���װֵ 
//	TIM3->PSC=psc;//Ԥ��Ƶ������Ƶ
//	TIM3->CCMR1|=6<<4;         //CH1 PWM1ģʽ	
//	TIM3->CCMR1|=6<<12;        //CH2 PWM1ģʽ	
//	TIM3->CCMR2|=6<<4;         //CH3 PWM1ģʽ	
//	TIM3->CCMR2|=6<<12;        //CH4 PWM1ģʽ	
//	
//	TIM3->CCMR1|=1<<3;         //CH1Ԥװ��ʹ��	  
//	TIM3->CCMR1|=1<<11;        //CH2Ԥװ��ʹ��	 
//	TIM3->CCMR2|=1<<3;         //CH3Ԥװ��ʹ��	  
//	TIM3->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	  
//	
//	TIM3->CCER|=1<<0;         //CH1���ʹ��	
//	TIM3->CCER|=1<<4;         //CH2���ʹ��	   
//	TIM3->CCER|=1<<8;         //CH3���ʹ��	 
//	TIM3->CCER|=1<<12;        //CH4���ʹ��

//	TIM3->CCR1=0;
//	TIM3->CCR2=0;
//	TIM3->CCR3=0;
//	TIM3->CCR4=0;
//	
//	TIM3->CR1=0x80;          //ARPEʹ�� 
//	TIM3->CR1|=0x01;          //ʹ�ܶ�ʱ��3
} 
