/*
********************************************************************************************************
文件名：bsp_GPIO.c
功  能：配置需要的普通IO
备  注：最好把所有的IO在这里配置，方便管理
淘  宝：https://shop60670850.taobao.com
作  者：lilianhe
日  期: 2017-2-6
*********************************************************************************************************
*/

/*
********************************************************************************************************
GPIO更新日志：
1.2017-2-6，编写LED初始化程序

*********************************************************************************************************
*/

#include "bsp_GPIO.h"   

/*
********************************************************************************************************
函数名称：void LED_GPIO_Config(void)
函数功能：初始化LED1的GPIO功能
硬件连接：LED1----PC13
备    注：
日    期: 2017-2-6
*********************************************************************************************************
*/
void LED_GPIO_Init(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( LED1_CLK, ENABLE); 	/*开启GPIO的外设时钟*/																   
		GPIO_InitStructure.GPIO_Pin = LED1_Pin;	/*选择要控制的GPIO引脚*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /*设置引脚模式为通用推挽输出*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /*设置引脚速率为50MHz */ 
		GPIO_Init(LED1_PORT, &GPIO_InitStructure);/*调用库函数，初始化GPIOC13*/	
}

//LED闪烁
void LED1_Flash(int time)
{
			 static int temp;
		if(++temp==time)
		{
			GPIO_WriteBit(LED1_PORT, LED1_Pin, (BitAction) (1 - GPIO_ReadInputDataBit(LED1_PORT, LED1_Pin)))	;//LED 蓝色色状态翻转
			temp=0;
		}
}

//------------------End of File----------------------------
