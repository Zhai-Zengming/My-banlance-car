#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY PAin(5)
#define KEY_2 PBin(4)
#define KEY_3 PBin(5)
void KEY_Init(void);          //������ʼ��
u8 click_N_Double (u8 time,u8 key);  //��������ɨ���˫������ɨ��
u8 click_M (u16 time,u8 key);  //��������ɨ��
u8 click(void);               //��������ɨ��
u8 Long_Press(void);
#endif 
