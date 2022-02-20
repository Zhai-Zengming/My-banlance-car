#include "./motor/motor.h"
#include "delay.h"

/* L298N��������� B12->BIN2       B13->BIN1      B14->AIN1     B15->AIN2
 *    PWM     :    A8 ->PWMA       A11->PWMB     ��ʱ��1ͨ��1,4
                   A6  ��ʱ��3ͨ��1
 */
		
void Motor_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);// ����ʱ��
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;// ��ʼ��GPIO--PB12��PB13��PB14��PB15Ϊ�������
	GPIO_InitStruct.GPIO_Pin = Motor_BIN2_Pin | Motor_BIN1_Pin | Motor_AIN1_Pin | Motor_AIN2_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Motor_GPIO_PORT, &GPIO_InitStruct);	
}

static void PWM_TIM1_Config(u16 pulse) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

    // ����Ƚ�ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN | ADVANCE_TIM_CH4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ADVANCE_TIM_PORT, &GPIO_InitStructure);

	
	/* ��ʱ�� */
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK, ENABLE);
	/* ʱ���ṹ�� */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/* ����ȽϽṹ�� */
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
	
	// ʹ�ܼ�����
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	// �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

//static void PWM_TIM3_Config(u16 pulse)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	
//    // ����Ƚ�ͨ�� GPIO ��ʼ��
//	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);
//	GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);
//	
//	/* ��ʱ�� */
//	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);
//	/* ʱ���ṹ�� */
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;
//	TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

//	/* ����ȽϽṹ�� */
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
//	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	/* ͨ��1 */
//	TIM_OCInitStructure.TIM_Pulse = pulse;
//	TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
//	
//	// ʹ�ܼ�����
//	TIM_Cmd(GENERAL_TIM, ENABLE);	
//}

#if 0
/* TIM1����PWM */
void PWM_Init_TIM1(u16 pulse,u16 Per)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);//����ʱ��
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;        // ��ʼ��GPIO--PA10��PA11Ϊ�����������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);  // ��ʼ����ʱ����
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = Per;
	TIM_TimeBaseInitStruct.TIM_Prescaler = ADVANCE_TIM_PSC;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);   // TIM1
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;      // ��ʼ������Ƚ�
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = pulse;
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);// �߼���ʱ��ר��!!!--MOE�����ʹ��
	
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC3Ԥװ�ؼĴ���ʹ��
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC4Ԥװ�ؼĴ���ʹ��
	TIM_ARRPreloadConfig(TIM1, ENABLE);// TIM1��ARR��Ԥװ�ؼĴ���ʹ��
	
	TIM_Cmd(TIM1, ENABLE);           // ����ʱ����
}

/* TIM3����PWM */
void PWM_Init_TIM3(u16 pulse,u16 Per)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//����ʱ��
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;        // ��ʼ��GPIO--PA10��PA11Ϊ�����������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);  // ��ʼ����ʱ����
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = Per;
	TIM_TimeBaseInitStruct.TIM_Prescaler = ADVANCE_TIM_PSC;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);   // TIM1
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;      // ��ʼ������Ƚ�
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = pulse;
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);// �߼���ʱ��ר��!!!--MOE�����ʹ��
	
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC3Ԥװ�ؼĴ���ʹ��
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);// OC4Ԥװ�ؼĴ���ʹ��
	TIM_ARRPreloadConfig(TIM1, ENABLE);// TIM1��ARR��Ԥװ�ؼĴ���ʹ��
	
	TIM_Cmd(TIM1, ENABLE);           // ����ʱ����
}

#endif

void Motor_Init(u16 pulse)
{
	Motor_GPIO_Config();
    PWM_TIM1_Config(pulse);
}


/**************************************************************************
�������ܣ�PWM�޷�����
��ڲ�������
����  ֵ����
**************************************************************************/	 
void Xianfu_Pwm(void)
{	
	int Amplitude = 7000;    //===PWM������7200 ������6900
	if(PWM1 < -Amplitude)  PWM1 = -Amplitude;	
	if(PWM1 > Amplitude )  PWM1 = Amplitude;	
	if(PWM2 < -Amplitude)  PWM2 = -Amplitude;	
	if(PWM2 > Amplitude )  PWM2 = Amplitude;		
}

/**************************************************************************
�������ܣ�����쳣�رպ���
��ڲ������Ƕ�
����  ֵ��1���رգ�0�����ر�
**************************************************************************/	 
u8 Turn_Off(signed int angle)
{
	u8 temp = 0;
	if(angle < -70 || angle > 60)
	{	                                                 //===��Ǵ���40�ȹرյ��
		temp = 1;                                        //===Flag_Stop��1�رյ��
		AIN1 = 0;                                            
		AIN2 = 0;
		BIN1 = 0;
		BIN2 = 0;
	}
	return temp;
}

/**************************************************************************
�������ܣ�����PWM����
��ڲ�������
����  ֵ����
**************************************************************************/	 
void Set_Pwm(int PWM1, int PWM2)
{
	if(stop_flag == 0)
	{
		if(PWM1 > 0)		  	AIN2=0,			AIN1=1;
		else 	                AIN2=1,			AIN1=0;
		
		if(PWM2 > 0)	        BIN1=0,			BIN2=1;
		else                    BIN1=1,			BIN2=0;
		
		/* ����PWM���壺TIM3ͨ��4 */
		TIM1->CCR1 = myabs(PWM1);
//		TIM_SetCompare4(TIM3,myabs(PWM2 + PWM2/myabs(PWM2)*10));
		
		/* ����PWM���壺TIM1ͨ��4 */
		TIM1->CCR4 = myabs(PWM2);
//	    TIM_SetCompare4(TIM1,myabs(PWM2 + PWM2/myabs(PWM2)*2));     //���������ң�����
	}
}

/**************************************************************************
�������ܣ������������ֵ����
��ڲ���������
����  ֵ������ֵ
**************************************************************************/	 
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

