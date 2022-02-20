#include "usart.h"	  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	x = x; 
	return 0;
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      

	while((USART3->SR&0X40)==0);
	USART3->DR = (u8) ch;      
  return ch;
}
#endif 

//******************************����1******************************************//
/////////////////////////////////////////////////////////////////////////////////
//****************usart1����һ���ֽ�************************************//
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
}
//****************����1��ʼ��************************************//
void usart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬����
	GPIOA->ODR|=1<<9;	  
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
	USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(2,2,USART1_IRQn,2);//��ռ���ȼ�2,��Ӧ���ȼ�2,��2
}

//****************************�������ܣ�����1�����ж�***************************************//
 u8 data1_top[55];
 u8 sum,i_u;
 u8 FLAG_USART=0;	
int break_count=0;
int USART1_IRQHandler(void)
{	
	if(USART1->SR&(1<<5))//���յ�����
	{	      	
   					data1_top[i_u] = USART1->DR;//��ȡ����
            break_count=0;		
			switch(i_u)
			{
				case 0:
					if( data1_top[0]==0XAA)//����֡ͷ����
					{
						i_u++;//��һ���ֽ�����
					}
					break;
				case 1:
					if( data1_top[1]==0XAA)//����֡ͷ����
					{
						i_u++;//��һ���ֽ�����
		
					}

					break;				
				case 2:
					if( data1_top[2]==0XF1)//����֡ͷ����
					{
						i_u++;//��һ���ֽ�����
		
					}
					break;	
				case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12: 
				case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 22:  
				case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:case 31:case 32:  
				case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42: 
				case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:   					
				case 51:
          i_u++;
					break;
				case 52://��У��	
          FLAG_USART=1;							
					break;				
				default:
					break;				
			}	
		
   }
return 0;	
}
/*********************************************
���ߣ���������
�Ա����̣�http://shop180997663.taobao.com/
*********************************************/
//************************����12������**************************// 
void Send_data(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8,float data9,float data10,float data11,float data12)
{
	
    u8 tbuf[50];
    unsigned char *p;
    p=(unsigned char *)&data1;
    tbuf[0]=(unsigned char)(*(p+3));
    tbuf[1]=(unsigned char)(*(p+2));
    tbuf[2]=(unsigned char)(*(p+1));
    tbuf[3]=(unsigned char)(*(p+0));

    p=(unsigned char *)&data2;
    tbuf[4]=(unsigned char)(*(p+3));
    tbuf[5]=(unsigned char)(*(p+2));
    tbuf[6]=(unsigned char)(*(p+1));
    tbuf[7]=(unsigned char)(*(p+0));
     
    p=(unsigned char *)&data3;
    tbuf[8]=(unsigned char)(*(p+3));
    tbuf[9]=(unsigned char)(*(p+2));
    tbuf[10]=(unsigned char)(*(p+1));
    tbuf[11]=(unsigned char)(*(p+0));
	
	  p=(unsigned char *)&data4;
    tbuf[12]=(unsigned char)(*(p+3));
    tbuf[13]=(unsigned char)(*(p+2));
    tbuf[14]=(unsigned char)(*(p+1));
    tbuf[15]=(unsigned char)(*(p+0));
		
		p=(unsigned char *)&data5;
    tbuf[16]=(unsigned char)(*(p+3));
    tbuf[17]=(unsigned char)(*(p+2));
    tbuf[18]=(unsigned char)(*(p+1));
    tbuf[19]=(unsigned char)(*(p+0));
     
		p=(unsigned char *)&data6;
    tbuf[20]=(unsigned char)(*(p+3));
    tbuf[21]=(unsigned char)(*(p+2));
    tbuf[22]=(unsigned char)(*(p+1));
    tbuf[23]=(unsigned char)(*(p+0));
		
		p=(unsigned char *)&data7;
    tbuf[24]=(unsigned char)(*(p+3));
    tbuf[25]=(unsigned char)(*(p+2));
    tbuf[26]=(unsigned char)(*(p+1));
    tbuf[27]=(unsigned char)(*(p+0));
     
    p=(unsigned char *)&data8;
    tbuf[28]=(unsigned char)(*(p+3));
    tbuf[29]=(unsigned char)(*(p+2));
    tbuf[30]=(unsigned char)(*(p+1));
    tbuf[31]=(unsigned char)(*(p+0));
     
    p=(unsigned char *)&data9;
    tbuf[32]=(unsigned char)(*(p+3));
    tbuf[33]=(unsigned char)(*(p+2));
    tbuf[34]=(unsigned char)(*(p+1));
    tbuf[35]=(unsigned char)(*(p+0));
	
	  p=(unsigned char *)&data10;
    tbuf[36]=(unsigned char)(*(p+3));
    tbuf[37]=(unsigned char)(*(p+2));
    tbuf[38]=(unsigned char)(*(p+1));
    tbuf[39]=(unsigned char)(*(p+0));
		
		p=(unsigned char *)&data11;
    tbuf[40]=(unsigned char)(*(p+3));
    tbuf[41]=(unsigned char)(*(p+2));
    tbuf[42]=(unsigned char)(*(p+1));
    tbuf[43]=(unsigned char)(*(p+0));
     
		p=(unsigned char *)&data12;
    tbuf[44]=(unsigned char)(*(p+3));
    tbuf[45]=(unsigned char)(*(p+2));
    tbuf[46]=(unsigned char)(*(p+1));
    tbuf[47]=(unsigned char)(*(p+0));
		
    usart3_niming_report(0XF1,tbuf,48);//�Զ���֡,������0XF1
}
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���48�ֽ�!!
//len:data����Ч���ݸ���

u8 send_buf[54];
void usart3_niming_report(u8 fun,u8*data,u8 len)
{
    
    u8 i;
    if(len>48)return;    //���48�ֽ�����
    send_buf[len+4]=0;  //У��������
    send_buf[0]=0XAA;   //֡ͷ
	  send_buf[1]=0XAA;   //֡ͷ
    send_buf[2]=fun;    //������
    send_buf[3]=len;    //���ݳ���
    for(i=0;i<len;i++)send_buf[4+i]=data[i];         //��������
    for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];//����У���
//  for(i=0;i<len+5;i++)usart3_send(send_buf[i]);   //���ڵ��ֽڷ���
}
//4��byte����ת��Ϊһ��float��ֵ
float b2f(byte m0, byte m1, byte m2, byte m3)
{
//�����λ
    float sig = 1.;
    if (m0 >=128.)
        sig = -1.;
  
//�����
    float jie = 0.;
     if (m0 >=128.)
    {
        jie = m0-128.  ;
    }
    else
    {
        jie = m0;
    }
    jie = jie * 2.;
    if (m1 >=128.)
        jie += 1.;
  
    jie -= 127.;
//��β�� 
    float tail = 0.;
    if (m1 >=128.)
        m1 -= 128.;
    tail =  m3 + (m2 + m1 * 256.) * 256.;
    tail  = (tail)/8388608;   //   8388608 = 2^23

    float f = sig * pow(2., jie) * (1+tail);
 
    return f;
}
//******************************����2********************************//
///////////////////////////////////////////////////////////////////////
//****************************usart2����һ���ֽ�*********************//
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}
//******************************����2��ʼ��**************************//
void usart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB1ENR|=1<<17;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO״̬����PA2 PA3
  
	RCC->APB1RSTR|=1<<17;   //��λ����1
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; // ����������	 
	USART2->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж�
	USART2->CR1|=1<<8;    //PE�ж�ʹ��
	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART2_IRQn,2);//��2��������ȼ� 
}

//***************************��ҡAPPͨѶ*******************//
/*********************************************
���ߣ���������
�Ա����̣�http://shop180997663.taobao.com/
*********************************************/
void LANYAO_APP(int data)//�������ݵ���ҡAPP
{
  static int SEND_DATA[10];	
  int i,j,k,data_c;
if(data==0)usart2_send(0x30);
if(data>0)
	{	
	 i=data;
   while(i)
          {
	        	i=i/10;		
		        j++;
	        }	
			for(k=0;k<j;k++){SEND_DATA[k]=data%10;data=data/10;}
      for(k=j-1;k>=0;k--){usart2_send(SEND_DATA[k]+0x30);}			
   }
	if(data<0)
	{
   data_c=-data; 		
	 i=-data;
   while(i)
          {
	        	i=i/10;		
		        j++;
	        }
   usart2_send(0x2D);				
			for(k=0;k<j;k++){SEND_DATA[k]=data_c%10;data_c=data_c/10;}
      for(k=j-1;k>=0;k--){usart2_send(SEND_DATA[k]+0x30);}			
   }
	usart2_send(0x0A),
	usart2_send(0x0D);
  memset(SEND_DATA, 0, sizeof(int)*10);	
} 
//**************************����2�����ж�***********************//
int Usart2_Receive;
int data_app[10];
u8 flag_mode_app=0;
int anjian_app,huakuai_app,yaogan_app=510;
/*********************************************
���ߣ���������
�Ա����̣�http://shop180997663.taobao.com/
*********************************************/
int USART2_IRQHandler(void)
{	
 static u8 i=0;
	if(USART2->SR&(1<<5))//���յ�����
	{	   
		      Usart2_Receive=USART2->DR;		
					data_app[i] = USART2->DR;//��ȡ����		                              
			switch(i)
			{
				case 0:
					if( data_app[0]==0X79)//����֡ͷ����
					{
						i++;//��һ���ֽ�����
					}
					break;
				case 1:
					if( data_app[1]==0X62)//����֡ͷ����
					{
						i++;//��һ���ֽ�����
						flag_mode_app=0;//��������
					}
			   	else	if( data_app[1]==0X76)//����֡ͷ����
					{
						i++;//��һ���ֽ�����
						flag_mode_app=1;//��������
					}
					else	if( data_app[1]==0X64)//����֡ͷ����
					{
						i++;//��һ���ֽ�����
						flag_mode_app=2;//ҡ������
					}
					break;				
				case 2:
						i++;//��һ���ֽ�����
					break;	
				case 3:            
            i++;//��һ���ֽ�����	
					break;
				case 4:
					 if(flag_mode_app==0&&data_app[3]==0X0A&&data_app[4]==0X0D)
						 anjian_app=data_app[2],i=0;//һ֡�������
					 if(flag_mode_app==1&&data_app[3]==0X0A&&data_app[4]==0X0D)
						 huakuai_app=data_app[2],i=0;//һ֡�������
					 if(flag_mode_app==2)i++;
					break;
				case 5:
					 if(data_app[4]==0X0A&&data_app[5]==0X0D)
					 yaogan_app= data_app[2]+data_app[3],i=0;//һ֡�������				
					break;
				default:
					break;				
			}
   }
return 0;	

}
//*******************����3��ʼ��,PCLK2 ʱ��Ƶ��(Mhz),bound:������************************//
void usart3_init(u32 pclk2,u32 bound)
{  	 
  float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<0;    //��������ʱ��
	RCC->APB2ENR|=1<<3;   //ʹ��PORTB��ʱ��  
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���ʱ�� 
	GPIOB->CRH&=0XFFFF00FF; 
	GPIOB->CRH|=0X00008B00;//IO״̬����PB10 PB11
	GPIOB->ODR|=1<<10;	 
	RCC->APB1RSTR|=1<<18;   //��λ����1
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж�
	USART3->CR1|=1<<8;    //PE�ж�ʹ��
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);//��2��������ȼ� 
}

//****************************�������ܣ�����3�����ж�***************************************//
int data_U3[5];
int USART3_IRQHandler(void)
{	
static u8 i=0;
	if(USART3->SR&(1<<5))//���յ�����
	{	   
	
					data_U3[i] = USART3->DR;//��ȡ����		                              
	
   }
return 0;		
}	
