#include "./pid/pid.h"


/**
  * @brief  小车平衡环，PD控制
	*
  * @param  @target: 目标给定值，一般为小车的机械零点
	*       @Angle:  小车当前角度
	*       @gyro_Y: 角速度
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
  * @brief  小车速度环，PI控制
  * @param  左右轮子速度
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
//	//1.计算速度偏差
//	Encoder_Err=((Encoder_Left+Encoder_Right)-Tgrget_Speed);//舍去误差--我的理解：能够让速度为"0"的角度，就是机械中值。
//	//2.对速度偏差进行低通滤波
//	//low_out=(1-a)*Ek+a*low_out_last;
//	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//使得波形更加平滑，滤除高频干扰，防止速度突变。
//	EnC_Err_Lowout_last=EnC_Err_Lowout;//防止速度过大的影响直立环的正常工作。
//	//3.对速度偏差积分，积分出位移
//	Encoder_S+=EnC_Err_Lowout;
//	//4.积分限幅
//	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
//	
//	//5.速度环控制输出计算
//	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
//	return PWM_out;
}

/**
  * @brief  小车转向环，P控制
  * @param  角速度，目标转向速度
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


