#ifndef __PIDCAL_H
#define	__PIDCAL_H

#include "stm32f10x.h"
#include <stdio.h>

int PID_output (void);
int PID1_output (void);
float myabs(float a);
float Set_Pwm(float moto1);
int combine(void);



#endif /* __USART1_H */

