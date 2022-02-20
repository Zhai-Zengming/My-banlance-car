
#include <PinChangeInt.h>    //外部中断
#include <MsTimer2.h>        //定时中断



/////////TB6612驱动引脚////
#define PWM 9   //PWM引脚
#define IN1 10
#define IN2 11
/////////编码器引脚////////
#define ENCODER_A 4
#define ENCODER_B 2

float Velocity,Position=10000,Motor;             //速度和位置测量值
float Target_Position=10000,Target_Velocity=40;  //目标速度和目标位置


float Position_KP=10,Position_KI=10,Position_KD=10,Velocity_KP=10,Velocity_KI=10;      //PID系数

/**************************************************************************
**************************************************************************/
void Set_Pwm(int motor)
{
  if (motor< 0)      digitalWrite(IN1, HIGH),      digitalWrite(IN2, LOW);  //TB6612的电平控制
  else               digitalWrite(IN1, LOW),       digitalWrite(IN2, HIGH); //TB6612的电平控制
  analogWrite(PWM, abs(motor)); //赋值给PWM寄存器
}
/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI (int Encoder,int Target)
{   
   static float Bias,Pwm,Last_bias;
   Bias=Encoder-Target;                                  //计算偏差
   Pwm+=Velocity_KP*(Bias-Last_bias)/100 +Velocity_KI*Bias/100;   //增量式PI控制器
   if(Pwm>7200)Pwm=7200;                                 //限幅
   Last_bias=Bias;                                       //保存上一次偏差 
   return Pwm;                                           //增量输出
}
/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
int Position_PID (int Encoder,int Target)
{   
   static float Pwm,Integral_bias,Last_Bias;
   float Bias;
   Bias=Encoder-Target;                                  //计算偏差
   Integral_bias+=Bias;                                  //求出偏差的积分
   Pwm=Position_KP*Bias/100 +Position_KI*Integral_bias/100 +Position_KD*(Bias-Last_Bias)/100;       //位置式PID控制器
   Last_Bias=Bias;                                       //保存上一次偏差 
   return Pwm;                                           //增量输出
}
/**************************************************************************
函数功能：10ms控制函数 
**************************************************************************/
void control()
{  

      sei();//全局中断开启
  
        Motor=Incremental_PI(Velocity,Target_Velocity);  //===速度PI控制器
       if(Flag_Stop==0)      Set_Pwm(Motor);    //如果不存在异常  输出电机控制量
       else Set_Pwm(0);

 }
 /**************************************************************************
函数功能：初始化 相当于STM32里面的Main函数 
**************************************************************************/
void setup()   {                

   pinMode(IN1, OUTPUT);          //TB6612方向控制引脚
   pinMode(IN2, OUTPUT);          //TB6612方向控制引脚，
   pinMode(PWM, OUTPUT);          //TB6612速度控制引脚
   digitalWrite(IN1, 0);          //TB6612控制引脚拉低
   digitalWrite(IN2, 0);          //TB6612控制引脚拉低
   digitalWrite(PWM, 0);          //TB6612控制引脚拉低
   pinMode(ENCODER_A, INPUT);       //编码器引脚
   pinMode(ENCODER_B, INPUT);       //编码器引脚
   Serial.begin(128000);           //开启串口
   delay(200);                      //延时等待初始化完成

   MsTimer2::set(10, control);       //使用Timer2设置10ms定时中断
   MsTimer2::start();               //中断使能
  attachInterrupt(0, READ_ENCODER_A, CHANGE);           //开启外部中断 
  attachPinChangeInterrupt(4, READ_ENCODER_B, CHANGE);  //开启外部中断 
}
/**************************************************************************
函数功能：主循环程序体
入口参数：无
返回  值：无
**************************************************************************/
void loop()                     
{

}
/**************************************************************************
函数功能：外部中断读取编码器数据，具有4倍频功能 注意外部中断是跳变沿触发
入口参数：无
返回  值：无
**************************************************************************/
void READ_ENCODER_A() {
    if (digitalRead(ENCODER_A) == HIGH) {     
    if (digitalRead(ENCODER_B) == LOW)      Position++;  //根据另外一相电平判定方向
    else      Position--;
  }
    else {    
    if (digitalRead(ENCODER_B) == LOW)      Position--; //根据另外一相电平判定方向
    else     Position++;
  }
}
/**************************************************************************
函数功能：外部中断读取编码器数据，具有4倍频功能 注意外部中断是跳变沿触发
入口参数：无
返回  值：无
**************************************************************************/
void READ_ENCODER_B() {
    if (digitalRead(ENCODER_B) == LOW) { //如果是下降沿触发的中断
    if (digitalRead(ENCODER_A) == LOW)      Position++;//根据另外一相电平判定方向
    else      Position--;
  }
  else {   //如果是上升沿触发的中断
    if (digitalRead(ENCODER_A) == LOW)      Position--; //根据另外一相电平判定方向
    else     Position++;
  }
}
