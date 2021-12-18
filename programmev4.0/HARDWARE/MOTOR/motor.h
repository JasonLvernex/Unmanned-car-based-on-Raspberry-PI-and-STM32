#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
/*
********************************************************************************************************

			//�������̼����������ķ�ֵ���
			//Ӳ������˵����
			timer8 ����ABCD�ĸ����ӵ�PWM
			TIM8_CH1--PC6--A�� //��ǰ ---- �������1-ENA
			TIM8_CH2--PC7--B�� //��ǰ ---- �������1-ENB
			TIM8_CH3--PC8--C�� //�Һ� ---- �������2-ENA
		  TIM8_CH4--PC9--D�� //��� ---- �������2-ENB
			
			
			A�֣�PC1 PC0 ����ǰ���˶� PC1----IN2��PC0----IN1
			B�֣�PC3 PC2 ����ǰ���˶� PC3----IN4��PC2----IN3
			
			C�֣�PC10��PC11����ǰ���˶� PC10 --- IN1��PC11---IN2
			D�֣�PC12��PD2����ǰ���˶�  PC12 --- IN3��PD2 ---IN4
			D�֣�PC12��PD2����ǰ���˶�  PC12 --- IN3��PD2 ---IN4
*********************************************************************************************************
*/
#define GO    0//������״̬ ǰ��
#define BACK  1//����
#define STOP  2//ͣ��



//����ǰ������������ĸ�����
#define MOTOR_CLK      RCC_APB2Periph_GPIOC
#define MOTOR_PORT     GPIOC
#define MOTOR_A1       GPIO_Pin_0  //A��
#define MOTOR_A2       GPIO_Pin_1  
#define MOTOR_B1       GPIO_Pin_2  //B��
#define MOTOR_B2       GPIO_Pin_3
#define MOTOR_C1       GPIO_Pin_11  //C��
#define MOTOR_C2       GPIO_Pin_10
#define MOTOR_D1       GPIO_Pin_12  //D��

//���ú󲿵���������ĸ�����
#define MOTOR_CLK2     RCC_APB2Periph_GPIOD
#define MOTOR_PORT2    GPIOD
#define MOTOR_D2       GPIO_Pin_2   //PD2��D�ֵڶ������ƿ�
    

void MOTOR_GPIO_Init(void);
void MOTOR_A(char state);
void MOTOR_B(char state);
void MOTOR_C(char state);
void MOTOR_D(char state);
					
void Car_Go(void);
void Car_Back(void);									
void Car_Turn_Right(void);
void Car_Turn_Left(void);
void Car_Stop(void);
void Car_Left(void);
void Car_Right(void);

#endif
