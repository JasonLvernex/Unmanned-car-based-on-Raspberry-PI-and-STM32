#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"
#include "bsp_sys.h"
int PID_Calc_A(int NextPoint,int Setpoint);
int PID_Calc_B(int NextPoint,int Setpoint);
int PID_Calc_C(int NextPoint,int Setpoint);
int PID_Calc_D(int NextPoint,int Setpoint);
int PID_Calc_MPU(int NextPoint,int SetPoint);
void showPID(void);
#endif

