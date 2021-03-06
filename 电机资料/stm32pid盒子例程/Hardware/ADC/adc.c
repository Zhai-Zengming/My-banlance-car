#include "adc.h"
void  Adc_Init(void)//初始化ADC
{     
 	RCC->APB2ENR|=1<<2;    //使能PORTA口时钟 
	RCC->APB2ENR|=1<<3;    //使能PORTB口时钟
	
	GPIOA->CRL&=0XFFFF00FF;//PA2 PA3 anolog输入 
	GPIOB->CRL&=0XFFFFFF00;//PB0 PB1 anolog输入 
	
	RCC->APB2ENR|=1<<9;    //ADC1时钟使能	  
	RCC->APB2RSTR|=1<<9;   //ADC1复位
	RCC->APB2RSTR&=~(1<<9);//复位结束	    
	RCC->CFGR&=~(3<<14);   //分频因子清零	
	//SYSCLK/DIV2=12M ADC时钟设置为12M,ADC最大时钟不能超过14M!
	//否则将导致ADC准确度下降! 
	RCC->CFGR|=2<<14;      	 
	ADC1->CR1&=0XF0FFFF;   //工作模式清零
	ADC1->CR1|=0<<16;      //独立工作模式  
	ADC1->CR1&=~(1<<8);    //非扫描模式	  
	ADC1->CR2&=~(1<<1);    //单次转换模式
	ADC1->CR2&=~(7<<17);	   
	ADC1->CR2|=7<<17;	   //软件控制转换  
	ADC1->CR2|=1<<20;      //使用用外部触发(SWSTART)!!!	必须使用一个事件来触发
	ADC1->CR2&=~(1<<11);   //右对齐	 
	ADC1->SQR1&=~(0XF<<20);
	ADC1->SQR1&=0<<20;     //1个转换在规则序列中 也就是只转换规则序列1 			   

	ADC1->SMPR2&=0X00000000; //通道0-9采样时间清空	
	//设置通道2 3 8 9的采样时间
//	ADC1->SMPR2|=7<<0;      // 239.5周期,提高采样时间可以提高精确度	
//	ADC1->SMPR2|=7<<3;      // 239.5周期,提高采样时间可以提高精确度	  
	ADC1->SMPR2|=7<<6;      // 239.5周期,提高采样时间可以提高精确度	
	ADC1->SMPR2|=7<<9;      // 239.5周期,提高采样时间可以提高精确度
//	ADC1->SMPR2|=7<<12;      // 239.5周期,提高采样时间可以提高精确度	
//	ADC1->SMPR2|=7<<15;      // 239.5周期,提高采样时间可以提高精确度		
//	ADC1->SMPR2|=7<<18;      // 239.5周期,提高采样时间可以提高精确度	
//	ADC1->SMPR2|=7<<21;      // 239.5周期,提高采样时间可以提高精确度		 
	ADC1->SMPR2|=7<<24;      // 239.5周期,提高采样时间可以提高精确度
	ADC1->SMPR2|=7<<27;      // 239.5周期,提高采样时间可以提高精确度
	
	//ADC1->SMPR1&=0XFFFC0000; //通道10-15采样时间清空

//	ADC1->SMPR1|=7<<0;      // 239.5周期,提高采样时间可以提高精确度	
//	ADC1->SMPR1|=7<<3;      // 239.5周期,提高采样时间可以提高精确度	
//	ADC1->SMPR1|=7<<6;      // 239.5周期,提高采样时间可以提高精确度		 
//	ADC1->SMPR1|=7<<9;      // 239.5周期,提高采样时间可以提高精确度
//	ADC1->SMPR1|=7<<12;      // 239.5周期,提高采样时间可以提高精确度
//	ADC1->SMPR1|=7<<15;      // 239.5周期,提高采样时间可以提高精确度	
	
	ADC1->CR2|=1<<0;	    //开启AD转换器	 
	ADC1->CR2|=1<<3;        //使能复位校准  
	while(ADC1->CR2&1<<3);  //等待校准结束 			 
    //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 		 
	ADC1->CR2|=1<<2;        //开启AD校准	   
	while(ADC1->CR2&1<<2);  //等待校准结束 
}		

/**************************************************************************
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	//设置转换序列	  		 
	ADC1->SQR3&=0XFFFFFFE0;//规则序列1 通道ch
	ADC1->SQR3|=ch;		  			    
	ADC1->CR2|=1<<22;       //启动规则转换通道 
	while(!(ADC1->SR&1<<1));//等待转换结束	 	   
	return ADC1->DR;		//返回adc值	
}

/**************************************************************************
**************************************************************************/
int Get_battery_voltage(void)   
{  
	int voltage;//电池电压
	voltage=Get_Adc(voltage_Ch)*3.3*11.0*100/1.0/4096;	//电阻分压
	return voltage;
}
