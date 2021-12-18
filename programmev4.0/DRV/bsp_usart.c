/*
********************************************************************************************************
文件名：bsp_usart.c
功  能：串口函数
备  注：
淘  宝：https://shop60670850.taobao.com
作  者：lilianhe
日  期: 2017-2-8
*********************************************************************************************************
*/

/*
********************************************************************************************************
更新日志：
1.2017-2-8，编写uart1函数

*********************************************************************************************************
*/
#include "bsp_usart.h"
#include <stdio.h>

volatile unsigned char *rx_address;
volatile unsigned int rx_count;
volatile unsigned int rx_length;

/*
********************************************************************************************************
函数名称：void USART1_Init(u32 bound)
函数功能：串口1初始化函数
硬件连接：PA9----TXD，PA10----RXD
备    注：bound为设置波特率
日    期:  2017-2-8
*********************************************************************************************************
*/
void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure; 

	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/* 使能串口1接收中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




/// 重定向c库函数printf到USART1
//int fputc(int ch, FILE *f)
//{
		/* 发送一个字节数据到USART1 */
//		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
//		return (ch);
//}

/// 重定向c库函数scanf到USART1
//int fgetc(FILE *f)
//{
		/* 等待串口1输入数据 */
//		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(USART1);
//}

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART3
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
	USART_SendData(USART1, (unsigned char) ch);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

/*
********************************************************************************************************
函数名称：void USART1_Send(unsigned char *tx_buf, int len)
函数功能：串口1发送函数
硬件连接：PA9----TXD，PA10----RXD
备    注：
日    期:  2017-2-8
*********************************************************************************************************
*/
void USART1_Send(unsigned char *tx_buf, int len)
{
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_ClearITPendingBit(USART1, USART_FLAG_TXE);
	while(len--)
	{
		USART_SendData(USART1, *tx_buf);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != 1);
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_ClearITPendingBit(USART1, USART_FLAG_TXE);
		tx_buf++;
	}
	
}

/*
********************************************************************************************************
函数名称：void USART1_Receive(unsigned char *rx_buf, int len)
函数功能：串口1接收函数
硬件连接：PA9----TXD，PA10----RXD
备    注：
日    期:  2017-2-8
*********************************************************************************************************
*/
void USART1_Receive(unsigned char *rx_buf, int len)
{
	rx_count = 0;
	rx_length = len;
	rx_address = rx_buf;
}
/*
********************************************************************************************************
函数名称：void USART1_IRQHandler(void)
函数功能：串口1中断服务函数
硬件连接：PA9----TXD，PA10----RXD
备    注：
日    期:  2017-2-8
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
	unsigned char Res;
	//u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		
		Res = USART_ReceiveData(USART1);
	//	 printf("[USART1_IRQHandler],Rec_data = %x\r\n", Res);

		if(rx_length > rx_count)
		{
			*rx_address = Res;
			rx_address++;
			rx_count++;	
	}
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			 
    } 	
}


/*********************************************END OF FILE**********************/
