#include "./pid/pid.h"


/**
  * @brief  С��ƽ�⻷��PD����
	*
  * @param  @target: Ŀ�����ֵ��һ��ΪС���Ļ�е���
	*       @Angle:  С����ǰ�Ƕ�
	*       @gyro_Y: ���ٶ�
	*
  * @retval balance
  */
int Balance(float target, float Angle, float gyro_Y) 
{
    int balance;
	static float error;
	error = Angle - target;
	
	balance = Balance_Kp * error + Balance_Kd * gyro_Y;
  
	return balance;
} 

/**
  * @brief  С���ٶȻ���PI����
  * @param  ���������ٶ�
  * @retval velocity
  */
int Velocity(int Encoder_Left, int Encoder_Right, int Tgrget_Speed)
{
    int velocity;
	static float Error, Error_Integral;
	
	Error = (Encoder_Left + Encoder_Right - Tgrget_Speed);
	Error_Integral += Error;
	Error_Integral=Error_Integral>5300?5300:(Error_Integral<(-5300)?(-5300):Error_Integral);
    velocity = Velocity_Kp * Error + Velocity_Ki * Error_Integral;
	return velocity;

//	static int Encoder_S,EnC_Err_Lowout_last,PWM_out,Encoder_Err,EnC_Err_Lowout;
//	float a=0.7;
//	
//	if(uart_receive == 1)
//		Tgrget_Speed = 200;
//	if(uart_receive == 2)
//		Tgrget_Speed = -200;
//	
//	//1.�����ٶ�ƫ��
//	Encoder_Err=((Encoder_Left+Encoder_Right)-Tgrget_Speed);//��ȥ���--�ҵ���⣺�ܹ����ٶ�Ϊ"0"�ĽǶȣ����ǻ�е��ֵ��
//	//2.���ٶ�ƫ����е�ͨ�˲�
//	//low_out=(1-a)*Ek+a*low_out_last;
//	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//ʹ�ò��θ���ƽ�����˳���Ƶ���ţ���ֹ�ٶ�ͻ�䡣
//	EnC_Err_Lowout_last=EnC_Err_Lowout;//��ֹ�ٶȹ����Ӱ��ֱ����������������
//	//3.���ٶ�ƫ����֣����ֳ�λ��
//	Encoder_S+=EnC_Err_Lowout;
//	//4.�����޷�
//	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
//	
//	//5.�ٶȻ������������
//	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
//	return PWM_out;
}

/**
  * @brief  С��ת�򻷣�P����
  * @param  ���ٶȣ�Ŀ��ת���ٶ�
  * @retval turn
  */
int Turn(float gyroz, float Turn_Speed)
{
	float a = (-0.2), b  = (-20), turn;
//	float Bias,target;
//	
//	if(uart_receive == 3 || uart_receive == 4)
//		target = (encode1 + encode2);
//	Bias = target - gyroz;
	
	turn = a*gyroz + b*Turn_Speed;
	return turn;
}


