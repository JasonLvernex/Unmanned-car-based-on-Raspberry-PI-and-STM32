//超声波例程
//蓝牙模块只需要接四个接口，TXD，RXD，5V，GND
//使用HC05蓝牙模块，串口透传功能，和使用串口差不多，使用stm32 的uart2
//PA2----蓝牙RXD  
//PA3----蓝牙TXD

#include "bluetooth.h"
#include <stdio.h>

volatile unsigned char *rx2_address;
volatile unsigned int rx2_count;
volatile unsigned int rx2_length;

/*
********************************************************************************************************
函数名称：void USART2_Init(u32 bound)
函数功能：串口1初始化函数
硬件连接：PA2----RXD，PA3----TXD
备    注：bound为设置波特率
日    期:  2017-2-8
*********************************************************************************************************
*/
void Bluetooth_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 使能串口1接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
}


//蓝牙发送到手机
void USART2_Send(unsigned char *tx_buf, int len)
{
		USART_ClearFlag(USART2, USART_FLAG_TC);
		USART_ClearITPendingBit(USART2, USART_FLAG_TXE);
	while(len--)
	{
		USART_SendData(USART2, *tx_buf);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != 1);
		USART_ClearFlag(USART2, USART_FLAG_TC);
		USART_ClearITPendingBit(USART2, USART_FLAG_TXE);
		tx_buf++;
	}
	
}

//蓝牙接受，从手机接收
void USART2_Receive(unsigned char *rx_buf, int len)
{
	rx2_count = 0;
	rx2_length = len;
	rx2_address = rx_buf;
}


//串口2中断函数
void USART2_IRQHandler(void)
{
	unsigned char Res;
	//u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		
		Res = USART_ReceiveData(USART2);

		if(rx2_length > rx2_count)
		{
			*rx2_address = Res;
			rx2_address++;
			rx2_count++;	
	}
			USART_ClearITPendingBit(USART2, USART_IT_RXNE);
			 
    } 	
}
