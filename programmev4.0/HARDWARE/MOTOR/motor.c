/*
********************************************************************************************************
文件名：motor.c
功  能：直流减速电机控制
备  注：	
淘  宝：https://shop60670850.taobao.com
作  者：lilianhe
日  期: 2017-2-6

             麦克纳姆轮安装方向

      TIM5  A轮  \\   ------   //  B轮 TIM2
						     \\   ------  //
							        ------
									    ------
									    ------
							    //  ------  \\
			 TIM4 D轮  //   ------   \\  C轮 TIM3
				
			//四驱底盘及四轮麦克纳姆轮底盘
			//硬件连接说明：
			timer8 控制ABCD四个轮子的PWM
			TIM8_CH1--PC6--A轮 //左前 ---- 电机驱动1-ENA
			TIM8_CH2--PC7--B轮 //右前 ---- 电机驱动1-ENB
			TIM8_CH3--PC8--C轮 //右后 ---- 电机驱动2-ENA
		  TIM8_CH4--PC9--D轮 //左后 ---- 电机驱动2-ENB
			
			
			A轮：PC1 PC0 控制前后运动   PC1----IN2，PC0----IN1
			B轮：PC3 PC2 控制前后运动   PC3----IN4，PC2----IN3			
			C轮：PC10，PC11控制前后运动 PC10 --- IN1，PC11---IN2
			D轮：PC12，PD2控制前后运动  PC12 --- IN3，PD2 ---IN4
				
				
			如不加说明：本程序使用L298N电机驱动，如果使用TB6612需要自己修改  底层
		    
*********************************************************************************************************
*/

#include "motor.h"
//***************************配置电机驱动IO口***************************//

void MOTOR_GPIO_Init(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);                                	  /*开启GPIO的外设时钟*/																   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 ;	/*选择要控制的GPIO引脚*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*设置引脚模式为通用推挽输出*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*设置引脚速率为50MHz */ 
		GPIO_Init(GPIOC,&GPIO_InitStructure); 	/*调用库函数，初始化GPIO*/	
	
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);                                	  /*开启GPIO的外设时钟*/																   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	/*选择要控制的GPIO引脚*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*设置引脚模式为通用推挽输出*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*设置引脚速率为50MHz */ 
		GPIO_Init(GPIOD, &GPIO_InitStructure);                                    /*调用库函数，初始化GPIO*/		
}
//左前A电机
void MOTOR_A(char state)
{
	if(state == GO)//左前电机前进
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_1);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_0);	
	}
	if(state == BACK)//左前电机后退
	{
	

		GPIO_SetBits(GPIOC,GPIO_Pin_0);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_1);
		
		
	}
	if(state == STOP)//停转
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	}
		
		
}

//右前B电机
void MOTOR_B(char state)
{
	if(state == GO)//右前电机前进
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_2);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	}
	if(state == BACK)//右前电机后退
	{
		
		GPIO_SetBits(GPIOC,GPIO_Pin_3);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	
	}
	if(state == STOP)//停转
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	}
}

//左后C电机
void MOTOR_C(char state)
{
	if(state == GO)//左后电机前进
	{
			GPIO_SetBits(GPIOC,GPIO_Pin_10);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_11);

	}
	if(state == BACK)//左后电机后退
	{

	
		GPIO_SetBits(GPIOC,GPIO_Pin_11);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_10);
	
	}
	if(state == STOP)//停转
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_10);
	}
		
}

//右后D电机
void MOTOR_D(char state)
{
	if(state == GO)//右后电机前进
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_0);//原来是D2，这里改了一下
	  GPIO_ResetBits(GPIOC,GPIO_Pin_12);
		
	}
	if(state == BACK)//右后电机后退
	{

		GPIO_SetBits(GPIOC,GPIO_Pin_12);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	}
	if(state == STOP)//停转
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_12);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	}
		
}


//***************************前进***************************//
//只要配置INx()的状态就可以改变电机转动方向
void Car_Go(void)
{
/*	//左前电机 前    //右前电机 前
	MOTOR_A(GO);       MOTOR_B(GO);*/

	//左后电机 前   //右后电机 前
	MOTOR_D(GO);       MOTOR_C(GO);
	
	
}

void Car_Back(void)
{
/*		//左前电机 后    //右前电机 后
MOTOR_A(BACK);       MOTOR_B(BACK);*/

	//左后电机 后   //右后电机 后
MOTOR_D(BACK);       MOTOR_C(BACK);
}

//***************************向左转圈***************************//
void Car_Turn_Left(void)
{
	
/*	//左前电机 后    //右前电机 前
MOTOR_A(BACK);       MOTOR_B(GO);*/

	//左后电机 后   //右后电机 前
MOTOR_D(BACK);       MOTOR_C(GO);
	
}
//***************************向右转圈***************************//
void Car_Turn_Right(void)
{
	
/*	//左前电机 前    //右前电机 后
MOTOR_A(GO);       MOTOR_B(BACK);*/

	//左后电机 前   //右后电机 后
MOTOR_D(GO);       MOTOR_C(BACK);
	
}


//***************************向右转***************************//
void Car_Right(void)
{
/*	//左前电机 前    //右前电机 停
MOTOR_A(GO);       MOTOR_B(STOP);*/

	//左后电机 前   //右后电机 停
  MOTOR_D(GO);       MOTOR_C(STOP);
	
}
//***************************向左转***************************//
void Car_Left(void)
{
	
/*	//左前电机 后    //右前电机 前
MOTOR_A(STOP);       MOTOR_B(GO);*/

	//左后电机 后   //右后电机 前
MOTOR_D(STOP);       MOTOR_C(GO);
	
}



//***************************停车***************************//
void Car_Stop(void)
{
/*	//左前电机 停    //右前电机 停
MOTOR_A(STOP);       MOTOR_B(STOP);*/

	//左后电机 停   //右后电机 停
  MOTOR_D(STOP);       MOTOR_C(STOP);
}



