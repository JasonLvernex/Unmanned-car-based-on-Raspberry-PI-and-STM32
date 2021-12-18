#ifndef __PID2_H
#define __PID2_H
#include "sys.h"

#define PI 3.14159265
//int TIM1_UP_IRQHandler(void);  
//void Set_Pwm(int moto1);
//void Xianfu_Pwm(void);
int myabs(int a);
int Position_PI (int Encoder,int Target);
int Incremental_PI (int Encoder,int Target);
#endif
