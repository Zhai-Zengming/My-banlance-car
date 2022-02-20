#include "sys.h"
/*********************************************
���ߣ���������
�Ա����̣�http://shop180997663.taobao.com/
*********************************************/
void Key(void);
int TIM1_UP_IRQHandler(void);
void Set_Pwm(int motor_a);
float A_velocity_Control(float encoder,float target);
float Position_Control(float encoder,float target);
u32 Get_Adc_average(u8 ch,u32 frequency);
u8 show_flag;
u8 delay_flag;
u8 delay_30;
u8 Start_Flag;
u8 MODE_Flag=1;
u16 Count_A;

int Voltage;
int count_sum,count_size;
int   Encoder_A,Encoder_B; //����������             
int   Motor_A; //�����PWM����
float Velocity_KP=30,Velocity_KI=200;//�ٶȿ���PI����
float Position_KP=50 ,Position_KI=15 ,Position_KD=1;
int Position_A,Position_B;
int Position_target;
int Velocity_target;
int Direc;

void TIM3_Init(u16 arr,u16 psc)  //
{  
	RCC->APB1ENR|=1<<1;       //ʹ��TIM3ʱ��    
 	TIM3->ARR=arr;      //�趨�������Զ���װֵ   
	TIM3->PSC=psc;      //Ԥ��Ƶ����Ƶ
	TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->DIER|=1<<6;   //�������ж�	   
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��
	MY_NVIC_Init(1,1,TIM3_IRQn,2);
}     
int main(void)
{ 
  Stm32_Clock_Init(9);               //ϵͳʱ������
	delay_init(72);                    //��ʱ��ʼ��
	JTAG_Set(JTAG_SWD_DISABLE);        //�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);              //��SWD�ӿ� �������������SWD�ӿڵ���
 
	LED_Init();                        //��ʼ���� LED ���ӵ�Ӳ���ӿ�
	KEY_Init();                        //������ʼ��
	usart1_init(72,115200);            //����1��ʼ��	
  usart3_init(36,115200);            //����3��ʼ��


	delay_ms(20);                      //��ʱ	
	Encoder_Init_TIM2();               //��ʱ��2������ģʽ����
	Encoder_Init_TIM4();               //��ʱ��4������ģʽ����
  Motor_PWM_Init(7199,0);	           //���PWM���� ��10KHZ ��Ƶ�� = 72000000 /(7199+1)/(1+0)
	delay_ms(10);                      //��ʱ
  OLED_Init();                       //OLED��ʼ��
	Adc_Init();                        //adc��ʼ��

	TIM3_Init(99,7199);                //��ʱ��3 10�����ж� , 100HZ/s
	delay_ms(50);                      //��ʱ
	
	while(1)
		{			
		
			  show_flag=!show_flag;                  //��OLED����λ��
				if(show_flag==0) oled_show();          //oled��ʾ 
      				
			  delay_flag=1;	                         //30ms�жϱ�־λ
				while(delay_flag);                     //30ms�ж�
		} 
}
/*********************************************
���ߣ���������
�Ա����̣�http://shop180997663.taobao.com/
*********************************************/
int TIM3_IRQHandler(void)  
{     
	if(TIM3->SR&0X0001)
	{     
		TIM3->SR&=~(1<<0);      //�����ʱ��2�жϱ�־λ
		
		Encoder_A = Encoder_Read(2);//��ȡA������������ݣ���ʱ��2
		Encoder_B = Encoder_Read(4);//��ȡB������������ݣ���ʱ��2
		Position_A += Encoder_A;

				
    Motor_A =A_velocity_Control(Encoder_A,20);		//����Ϊ����������ʽ�ٶȿ��ƣ�ת��Ϊÿ5���� 20������������4��Ƶ��


		

		
		     if(Motor_A>+7100)Motor_A=+7100; //PWM�޷�
		else if(Motor_A<-7100)Motor_A=-7100; 
		
		if(Start_Flag==1){Set_Pwm(Motor_A);} //���������־Ϊ1������
		else Set_Pwm(0); //����ر�
		

//************************************����**************************************************//	
    Key();//�������
				 
		count_sum+=Get_battery_voltage();//��ѹ�����ۼ�
	  count_size++;
		if(count_size==100) Voltage=count_sum/count_size,count_sum=0,count_size=0;//��ƽ����ѹ 
		static int time_in;
    if((Voltage<720&&Voltage>600)||(Voltage<1120&&Voltage>1000)||(Voltage<2230&&Voltage>2100))time_in++;
		else time_in=0;
		if(time_in>500)Start_Flag=0;

	  if( (Voltage<760&&Voltage>600)||(Voltage<1130&&Voltage>1000)||(Voltage<2250&&Voltage>2100))Led_Flash(20);//������ʱ������˸
	  else Led_Flash(100);//led��˸
		if(delay_flag==1)
		 {
			 if(++delay_30==6)	 delay_30=0,delay_flag=0;
		 }
		 
 }
	 return 0;	 
}

/**************************************************************************
����ָ��
**************************************************************************/
void Key(void)
{	
static u8 flag=0;
flag=click_N_Double(70,KEY);	
if(flag==1&&Start_Flag==0)Start_Flag=1,flag=0; //����һ��������־λ��ת
if(flag==1&&Start_Flag==1)Start_Flag=0,flag=0; //����һ��������־λ��ת	
	
static u8 flag1=0;
flag1=click_M(9,KEY_3);	
if(flag1==1&&MODE_Flag==0)MODE_Flag=1,flag1=0; //����һ��������־λ��ת
if(flag1==1&&MODE_Flag==1)MODE_Flag=0,flag1=0; //����һ��������־λ��ת	
}
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
*************************************************************************/
void Set_Pwm(int motor_a)
{
	  if(motor_a<0)			  INA1=0,INA2=1,PWMA=-(motor_a);//PWM��ֵ��TB6612FNG����оƬ
  	else	              INA1=1,INA2=0,PWMA=+(motor_a);//PWM��ֵ��TB6612FNG����оƬ

}
/**************************************************************************
�������ܣ�PI�ٶȿ���
*************************************************************************/
float A_velocity_Control(float encoder,float target)
{
	  static float velocity,bias,Last_bias; 	
		bias = target - encoder;       //�ٶ��˲�  	
		velocity += Velocity_KP*(bias-Last_bias)/100 + bias*Velocity_KI/100;//�ٶȿ���
		     if(velocity>+7199)velocity=+7199;
		else if(velocity<-7199)velocity=-7199;	
    Last_bias = bias;
	  if(Start_Flag==0)Last_bias = 0,velocity = 0;//��������
	  return velocity;
}
/**************************************************************************
�������ܣ�PIDλ�ÿ���
*************************************************************************/
float Position_Control(float encoder,float target)
{   
    static float  error ,Last_Bias,PWM ,Bias ,Differential;

  	 Bias = encoder -target;     //��ȡƫ��		
     error += Bias;             //�ۼӳ�λ�� 
          if(error>+1000)error=+1000;
     else if(error<-1000)error=-1000;	
	   Differential=Bias - Last_Bias;    //���ƫ���΢�� 	
	   PWM=Position_KP*Bias/10 + Position_KI*error/100 + Position_KD*Differential/10;  //��ȡ������ֵ
	  if(Start_Flag==0)Last_Bias = 0,error = 0;//��������	
	   Last_Bias=Bias; //����Ϊ�ϴ�ƫ��
	   return PWM;
}
/**************************************************************************
�������ܣ���ȡADֵ
*************************************************************************/
u32 Get_Adc_average(u8 ch,u32 frequency)
{
	u32 t=0,sum=0;
	for(t=0;t<frequency;t++)
	{
		sum+=Get_Adc(ch);
		delay_us(1);
	}
	return (sum/frequency);
} 
