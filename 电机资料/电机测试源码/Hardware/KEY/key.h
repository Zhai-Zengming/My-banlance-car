#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY PAin(5)
#define KEY_2 PBin(4)
#define KEY_3 PBin(5)
void KEY_Init(void);          //按键初始化
u8 click_N_Double (u8 time,u8 key);  //单击按键扫描和双击按键扫描
u8 click_M (u16 time,u8 key);  //单击按键扫描
u8 click(void);               //单击按键扫描
u8 Long_Press(void);
#endif 
