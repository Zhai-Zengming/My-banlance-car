#include "sys.h"
/*********************************************
作者：星洛智能
淘宝店铺：http://shop180997663.taobao.com/
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
int   Encoder_A,Encoder_B; //编码器读数             
int   Motor_A; //电机的PWM参数
float Velocity_KP=30,Velocity_KI=200;//速度控制PI参数
float Position_KP=50 ,Position_KI=15 ,Position_KD=1;
int Position_A,Position_B;
int Position_target;
int Velocity_target;
int Direc;

void TIM3_Init(u16 arr,u16 psc)  //
{  
	RCC->APB1ENR|=1<<1;       //使能TIM3时钟    
 	TIM3->ARR=arr;      //设定计数器自动重装值   
	TIM3->PSC=psc;      //预分频器分频
	TIM3->DIER|=1<<0;   //允许更新中断				
	TIM3->DIER|=1<<6;   //允许触发中断	   
	TIM3->CR1|=0x01;    //使能定时器
	MY_NVIC_Init(1,1,TIM3_IRQn,2);
}     
int main(void)
{ 
  Stm32_Clock_Init(9);               //系统时钟设置
	delay_init(72);                    //延时初始化
	JTAG_Set(JTAG_SWD_DISABLE);        //关闭JTAG接口
	JTAG_Set(SWD_ENABLE);              //打开SWD接口 可以利用主板的SWD接口调试
 
	LED_Init();                        //初始化与 LED 连接的硬件接口
	KEY_Init();                        //按键初始化
	usart1_init(72,115200);              //串口1初始化	
  usart3_init(36,115200);               //串口3初始化
	delay_ms(100);
	Dma_Init(1,DMA1_Channel4,(u32)&USART1->DR,(u32)send_buf,53);//初始化DMA1：(通道4，数据源send_buf[]，传输目标：外设地址 &USART1->DR	，传输数据量),非循环模式	 
	USART1->CR3|=1<<7;                 //串口1DMA中断使能
 	Dma_Enable(DMA1_Channel4,53);      //DMA搬运使能，(通道4 ,传输数据量)
	delay_ms(20);                   //延时	
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
  Motor_PWM_Init(7199,0);	
	delay_ms(10);                      //延时
  OLED_Init();                       //OLED初始化
	Adc_Init();                        //adc初始化

	TIM3_Init(99,7199);                //定时器3 10毫秒中断
	delay_ms(50);                      //延时
	
	while(1)
		{			
		
			  show_flag=!show_flag;                  //错开OLED与上位机
				if(show_flag==0) oled_show();          //oled显示 
      				
			  delay_flag=1;	                         //30ms中断标志位
				while(delay_flag);                     //30ms中断
		} 
}
/*********************************************
作者：星洛智能
淘宝店铺：http://shop180997663.taobao.com/
*********************************************/
int TIM3_IRQHandler(void)  
{     
	if(TIM3->SR&0X0001)
	{     
		TIM3->SR&=~(1<<0);      //清除定时器2中断标志位
		
		Encoder_A = Encoder_Read(2);
		Encoder_B = Encoder_Read(4);
		Position_A += Encoder_A;

				
		if(MODE_Flag==0){
			Velocity_target+=Encoder_B ;
			Position_A = Position_A%1400;
			Position_B = Position_B%1400;
		}
		else Position_B += +(float)Encoder_B*18.0F;
		

		if(MODE_Flag==0)Motor_A =A_velocity_Control(Encoder_A,Velocity_target),Position_Control(0,0);		

    else Motor_A = -Position_Control(Position_A,Position_B + Direc), A_velocity_Control(0,0);
		

		
		     if(Motor_A>+5268)Motor_A=+5268;
		else if(Motor_A<-5268)Motor_A=-5268;
		
		if(Start_Flag==1){Set_Pwm(Motor_A);}
		else Set_Pwm(0);
		
		if(KEY_2==1)Direc=-200;
		else if(KEY_2==0)Direc=200;
		Position_target =	Position_B +Direc;
//************************************其它**************************************************//	
    Key();//按键检测
    if(MODE_Flag==1){    		
       static u8 flag_in_1=0;
		   flag_in_1++;		
		        if(flag_in_1==1)Position_KP = Get_Adc_average(3,150)*0.1f;
		   else if(flag_in_1==2)Position_KI = Get_Adc_average(8,150)*0.1f;
		   else if(flag_in_1==3)Position_KD = Get_Adc_average(9,150)*0.1f,flag_in_1=0;
    }
    else {    		
       static u8 flag_in_1=0;
		   flag_in_1++;		
		        if(flag_in_1==1)Velocity_KP = Get_Adc_average(3,150)*0.1f;
		   else if(flag_in_1==2)Velocity_KI = Get_Adc_average(8,150)*0.1f,flag_in_1=0;
    }
//**********************************串口通讯收发********************************************************************************//

//	   if(FLAG_USART==1)//如果串口标志位置1，表示已完成一组数据传输
//	     { 
				   
				  if(DMA1->ISR&(1<<13))//通道4传输完成标志(（通道号-1）*4+1)
			      {				 
			        Dma_Disenable(DMA1_Channel4);//DMA搬运除能，通道2 	

              Send_data(Motor_A,Encoder_A,Position_A,Velocity_target,Position_target,0,0,0,0,0,0,0);

								//发送数据（分别为三轴角度，三轴角速度，ABC电机编码器数值，ABC电机PWM值大小和方向）						
			        DMA1->IFCR|=1<<13;//清空通道4传输完成标志
			        Dma_Enable(DMA1_Channel4,53);//DMA搬运使能，通道1                    
			      }
						
//					FLAG_USART=0;//串口标志位置0
//			 }		
			 
		count_sum+=Get_battery_voltage();//电压采样累计
	  count_size++;
		if(count_size==100) Voltage=count_sum/count_size,count_sum=0,count_size=0;//求平均电压 
		static int time_in;
    if((Voltage<720&&Voltage>600)||(Voltage<1120&&Voltage>1000)||(Voltage<2230&&Voltage>2100))time_in++;
		else time_in=0;
		if(time_in>500)Start_Flag=0;

	  if( (Voltage<760&&Voltage>600)||(Voltage<1130&&Voltage>1000)||(Voltage<2250&&Voltage>2100))Led_Flash(20);//电量低时快速闪烁
	  else Led_Flash(100);//led闪烁
		if(delay_flag==1)
		 {
			 if(++delay_30==6)	 delay_30=0,delay_flag=0;
		 }
		 
 }
	 return 0;	 
}

/**************************************************************************
按键指令
**************************************************************************/
void Key(void)
{	
static u8 flag=0;
flag=click_N_Double(70,KEY);	
if(flag==1&&Start_Flag==0)Start_Flag=1,flag=0; //单击一次启动标志位反转
if(flag==1&&Start_Flag==1)Start_Flag=0,flag=0; //单击一次启动标志位反转	
	
static u8 flag1=0;
flag1=click_M(9,KEY_3);	
if(flag1==1&&MODE_Flag==0)MODE_Flag=1,flag1=0; //单击一次启动标志位反转
if(flag1==1&&MODE_Flag==1)MODE_Flag=0,flag1=0; //单击一次启动标志位反转	
}
/**************************************************************************
函数功能：赋值给PWM寄存器
*************************************************************************/
void Set_Pwm(int motor_a)
{
	  if(motor_a<0)			  INA1=0,INA2=1,PWMA=-(motor_a);//PWM赋值，TB6612FNG驱动芯片
  	else	              INA1=1,INA2=0,PWMA=+(motor_a);//PWM赋值，TB6612FNG驱动芯片

}
/**************************************************************************
函数功能：PI速度控制
*************************************************************************/
float A_velocity_Control(float encoder,float target)
{
	  static float velocity,bias,Last_bias; 	
		bias = target - encoder;       //速度滤波  	
		velocity += Velocity_KP*(bias-Last_bias)/100 + bias*Velocity_KI/100;//速度控制
		     if(velocity>+7199)velocity=+7199;
		else if(velocity<-7199)velocity=-7199;	
    Last_bias = bias;
	  if(Start_Flag==0)Last_bias = 0,velocity = 0;//参数清零
	  return velocity;
}
/**************************************************************************
函数功能：PID位置控制
*************************************************************************/
float Position_Control(float encoder,float target)
{   
    static float  error ,Last_Bias,PWM ,Bias ,Differential;

  	 Bias = encoder -target;     //获取偏差		
     error += Bias;             //累加出位移 
          if(error>+1000)error=+1000;
     else if(error<-1000)error=-1000;	
	   Differential=Bias - Last_Bias;    //求出偏差的微分 	
	   PWM=Position_KP*Bias/10 + Position_KI*error/100 + Position_KD*Differential/10;  //获取最终数值
	  if(Start_Flag==0)Last_Bias = 0,error = 0;//参数清零	
	   Last_Bias=Bias; //保存为上次偏差
	   return PWM;
}
/**************************************************************************
函数功能：获取AD值
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
