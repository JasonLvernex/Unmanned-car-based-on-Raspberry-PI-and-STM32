#ifndef __BSP_TIMER8_H
#define __BSP_TIMER8_H


#include "stm32f10x.h"

#define ENCODER_TIM_PERIOD (u16)(60000)   // number of pulses per revolution

void TIM8_PWM_Init(u16 arr,u16 psc);

void MOTOR_PWM_OutA(int zkb);
void MOTOR_PWM_OutB(int zkb);
void MOTOR_PWM_OutC(int zkb);
void MOTOR_PWM_OutD(int zkb);

void MOTOR_PWM_Out(int zkbA,int zkbB,int zkbC,int zkbD);
void Timer6_Init(void);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
void Encoder_Init_TIM5(void);
int Read_Encoder(u8 TIMX);

#endif

