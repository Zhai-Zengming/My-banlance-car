#include "key.h"
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   	 
	GPIOA->CRL&=0XFF0FFFFF; 
	GPIOA->CRL|=0X00800000;//PA5上拉输入
  GPIOA->ODR|=1<<5; // 上拉	

	RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	 
	GPIOB->CRL&=0XFF00FFFF; 
	GPIOB->CRL|=0X00880000;//PB4 5输入

} 
/**************************************************************************
函数功能：按键扫描
入口参数：双击等待时间
返回  值：按键状态 0：无动作 1：单击 2：双击 
**************************************************************************/
u8 click_N_Double (u8 time,u8 key)
{
		static	u8 flag_key,count_key,double_key;	
		static	u16 count_single,Forever_count;
	  if(key==0)  Forever_count++;   //长按标志位未置1
     else        Forever_count=0;
		if(0==key&&0==flag_key)		flag_key=1;	
	  if(0==count_key)
		{
				if(flag_key==1) 
				{
					double_key++;
					count_key=1;	
				}
				if(double_key==2) 
				{
					double_key=0;
					count_single=0;
					return 2;//双击执行的指令
				}
		}
		if(1==key)			flag_key=0,count_key=0;
		
		if(1==double_key)
		{
			count_single++;
			if(count_single>time&&Forever_count<time)
			{
			double_key=0;
			count_single=0;	
			return 1;//单击执行的指令
			}
			if(Forever_count>time)
			{
			double_key=0;
			count_single=0;	
			}
		}	
		return 0;
}

/**************************************************************************
函数功能：按键扫描
入口参数：单击等待时间
返回  值：按键状态 0：无动作 1：单击
**************************************************************************/
u8 click_M(u16 time,u8 key)
{
	  static u16 count_2,count_3,flag_key;
	  
	  if(key==1 && flag_key==0)flag_key=1;//高电平标志一次
	
	  if(key==0 &&flag_key==2)count_2++;//记录低电平持续时间
	  if(key==1 &&flag_key==1)count_3++;//记录高电平持续时间	
	
    if(count_3>time)flag_key=2;	//高电平持续时间达到预设值 标志一次
	
		     if(count_2>time  && flag_key==2){count_2=0;count_3=0;flag_key=0; return 1;}
	  
 	  
    return 0;
}

