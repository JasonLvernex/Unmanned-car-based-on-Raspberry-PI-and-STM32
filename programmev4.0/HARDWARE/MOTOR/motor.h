#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
/*
********************************************************************************************************

			//四驱底盘及四轮麦克纳姆轮底盘
			//硬件连接说明：
			timer8 控制ABCD四个轮子的PWM
			TIM8_CH1--PC6--A轮 //左前 ---- 电机驱动1-ENA
			TIM8_CH2--PC7--B轮 //右前 ---- 电机驱动1-ENB
			TIM8_CH3--PC8--C轮 //右后 ---- 电机驱动2-ENA
		  TIM8_CH4--PC9--D轮 //左后 ---- 电机驱动2-ENB
			
			
			A轮：PC1 PC0 控制前后运动 PC1----IN2，PC0----IN1
			B轮：PC3 PC2 控制前后运动 PC3----IN4，PC2----IN3
			
			C轮：PC10，PC11控制前后运动 PC10 --- IN1，PC11---IN2
			D轮：PC12，PD2控制前后运动  PC12 --- IN3，PD2 ---IN4
			D轮：PC12，PD2控制前后运动  PC12 --- IN3，PD2 ---IN4
*********************************************************************************************************
*/
#define GO    0//定义电机状态 前进
#define BACK  1//后退
#define STOP  2//停车



//配置前部电机驱动的四个引脚
#define MOTOR_CLK      RCC_APB2Periph_GPIOC
#define MOTOR_PORT     GPIOC
#define MOTOR_A1       GPIO_Pin_0  //A轮
#define MOTOR_A2       GPIO_Pin_1  
#define MOTOR_B1       GPIO_Pin_2  //B轮
#define MOTOR_B2       GPIO_Pin_3
#define MOTOR_C1       GPIO_Pin_11  //C轮
#define MOTOR_C2       GPIO_Pin_10
#define MOTOR_D1       GPIO_Pin_12  //D轮

//配置后部电机驱动的四个引脚
#define MOTOR_CLK2     RCC_APB2Periph_GPIOD
#define MOTOR_PORT2    GPIOD
#define MOTOR_D2       GPIO_Pin_2   //PD2是D轮第二个控制口
    

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
