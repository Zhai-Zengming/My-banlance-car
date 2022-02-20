#include "key.h"
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   	 
	GPIOA->CRL&=0XFF0FFFFF; 
	GPIOA->CRL|=0X00800000;//PA5��������
  GPIOA->ODR|=1<<5; // ����	

	RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��	   	 
	GPIOB->CRL&=0XFF00FFFF; 
	GPIOB->CRL|=0X00880000;//PB4 5����

} 
/**************************************************************************
�������ܣ�����ɨ��
��ڲ�����˫���ȴ�ʱ��
����  ֵ������״̬ 0���޶��� 1������ 2��˫�� 
**************************************************************************/
u8 click_N_Double (u8 time,u8 key)
{
		static	u8 flag_key,count_key,double_key;	
		static	u16 count_single,Forever_count;
	  if(key==0)  Forever_count++;   //������־λδ��1
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
					return 2;//˫��ִ�е�ָ��
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
			return 1;//����ִ�е�ָ��
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
�������ܣ�����ɨ��
��ڲ����������ȴ�ʱ��
����  ֵ������״̬ 0���޶��� 1������
**************************************************************************/
u8 click_M(u16 time,u8 key)
{
	  static u16 count_2,count_3,flag_key;
	  
	  if(key==1 && flag_key==0)flag_key=1;//�ߵ�ƽ��־һ��
	
	  if(key==0 &&flag_key==2)count_2++;//��¼�͵�ƽ����ʱ��
	  if(key==1 &&flag_key==1)count_3++;//��¼�ߵ�ƽ����ʱ��	
	
    if(count_3>time)flag_key=2;	//�ߵ�ƽ����ʱ��ﵽԤ��ֵ ��־һ��
	
		     if(count_2>time  && flag_key==2){count_2=0;count_3=0;flag_key=0; return 1;}
	  
 	  
    return 0;
}

