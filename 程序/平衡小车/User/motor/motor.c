#include "./motor/motor.h"
#include "delay.h"

/* L298N电机驱动： B12->BIN2       B13->BIN1      B14->AIN1     B15->AIN2
 *    PWM     :    A8 ->PWMA       A11->PWMB     定时器1通道1,4
                   A6  定时器3通道1
 */
		
void Motor_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);// 开启时钟
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;// 初始化GPIO--PB12、PB13、PB14、PB15为推挽输出
	GPIO_InitStruct.GPIO_Pin = Motor_BIN2_Pin | Motor_BIN1_Pin | Motor_AIN1_Pin | Motor_AIN2_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Motor_GPIO_PORT, &GPIO_InitStruct);	
}

static void PWM_TIM1_Config(u16 pulse) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

    // 输出比较通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN | ADVANCE_TIM_CH4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ADVANCE_TIM_PORT, &GPIO_InitStructure);

	
	/* 开时钟 */
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK, ENABLE);
	/* 时基结构体 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/* 输出比较结构体 */
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = pulse;
	
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	TIM_OC4Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	
	// 使能计数器
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	// 主输出使能，当使用的是通用定时器时，这句不需要
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

//static void PWM_TIM3_Config(u16 pulse)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	
//    // 输出比较通道 GPIO 初始化
//	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);
//	GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);
//	
//	/* 开时钟 */
//	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);
//	/* 时基结构体 */
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;
//	TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

//	/* 输出比较结构体 */
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
//	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	/* 通道1 */
//	TIM_OCInitStructure.TIM_Pulse = pulse;
//	TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
//	
//	// 使能计数器
//	TIM_Cmd(GENERAL_TIM, ENABLE);	
//}

#if 0
/* TIM1产生PWM */
void PWM_Init_TIM1(u16 pulse,u16 Per)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);//开启时钟
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;        // 初始化GPIO--PA10、PA11为复用推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);  // 初始化定时器。
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = Per;
	TIM_TimeBaseInitStruct.TIM_Prescaler = ADVANCE_TIM_PSC;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);   // TIM1
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;      // 初始化输出比较
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = pulse;
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);// 高级定时器专属!!!--MOE主输出使能
	
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC3预装载寄存器使能
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC4预装载寄存器使能
	TIM_ARRPreloadConfig(TIM1, ENABLE);// TIM1在ARR上预装载寄存器使能
	
	TIM_Cmd(TIM1, ENABLE);           // 开定时器。
}

/* TIM3产生PWM */
void PWM_Init_TIM3(u16 pulse,u16 Per)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//开启时钟
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;        // 初始化GPIO--PA10、PA11为复用推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);  // 初始化定时器。
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = Per;
	TIM_TimeBaseInitStruct.TIM_Prescaler = ADVANCE_TIM_PSC;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);   // TIM1
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;      // 初始化输出比较
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = pulse;
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);// 高级定时器专属!!!--MOE主输出使能
	
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC3预装载寄存器使能
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC4预装载寄存器使能
	TIM_ARRPreloadConfig(TIM1, ENABLE);// TIM1在ARR上预装载寄存器使能
	
	TIM_Cmd(TIM1, ENABLE);           // 开定时器。
}

#endif

void Motor_Init(u16 pulse)
{
	Motor_GPIO_Config();
    PWM_TIM1_Config(pulse);
}


/**************************************************************************
函数功能：PWM限幅函数
入口参数：无
返回  值：无
**************************************************************************/	 
void Xianfu_Pwm(void)
{	
	int Amplitude = 7000;    //===PWM满幅是7200 限制在6900
	if(PWM1 < -Amplitude)  PWM1 = -Amplitude;	
	if(PWM1 > Amplitude )  PWM1 = Amplitude;	
	if(PWM2 < -Amplitude)  PWM2 = -Amplitude;	
	if(PWM2 > Amplitude )  PWM2 = Amplitude;		
}

/**************************************************************************
函数功能：电机异常关闭函数
入口参数：角度
返回  值：1：关闭，0：不关闭
**************************************************************************/	 
u8 Turn_Off(signed int angle)
{
	u8 temp = 0;
	if(angle < -70 || angle > 60)
	{	                                                 //===倾角大于40度关闭电机
		temp = 1;                                        //===Flag_Stop置1关闭电机
		AIN1 = 0;                                            
		AIN2 = 0;
		BIN1 = 0;
		BIN2 = 0;
	}
	return temp;
}

/**************************************************************************
函数功能：设置PWM函数
入口参数：无
返回  值：无
**************************************************************************/	 
void Set_Pwm(int PWM1, int PWM2)
{
	if(stop_flag == 0)
	{
		if(PWM1 > 0)		  	AIN2=0,			AIN1=1;
		else 	                AIN2=1,			AIN1=0;
		
		if(PWM2 > 0)	        BIN1=0,			BIN2=1;
		else                    BIN1=1,			BIN2=0;
		
		/* 设置PWM脉冲：TIM3通道4 */
		TIM1->CCR1 = myabs(PWM1);
//		TIM_SetCompare4(TIM3,myabs(PWM2 + PWM2/myabs(PWM2)*10));
		
		/* 设置PWM脉冲：TIM1通道4 */
		TIM1->CCR4 = myabs(PWM2);
//	    TIM_SetCompare4(TIM1,myabs(PWM2 + PWM2/myabs(PWM2)*2));     //开关面向我，右轮
	}
}

/**************************************************************************
函数功能：获得整数绝对值函数
入口参数：整数
返回  值：绝对值
**************************************************************************/	 
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

