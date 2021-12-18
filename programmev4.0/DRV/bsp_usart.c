/*
********************************************************************************************************
�ļ�����bsp_usart.c
��  �ܣ����ں���
��  ע��
��  ����https://shop60670850.taobao.com
��  �ߣ�lilianhe
��  ��: 2017-2-8
*********************************************************************************************************
*/

/*
********************************************************************************************************
������־��
1.2017-2-8����дuart1����

*********************************************************************************************************
*/
#include "bsp_usart.h"
#include <stdio.h>

volatile unsigned char *rx_address;
volatile unsigned int rx_count;
volatile unsigned int rx_length;

/*
********************************************************************************************************
�������ƣ�void USART1_Init(u32 bound)
�������ܣ�����1��ʼ������
Ӳ�����ӣ�PA9----TXD��PA10----RXD
��    ע��boundΪ���ò�����
��    ��:  2017-2-8
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
	
	/* ʹ�ܴ���1�����ж� */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




/// �ض���c�⺯��printf��USART1
//int fputc(int ch, FILE *f)
//{
		/* ����һ���ֽ����ݵ�USART1 */
//		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
//		return (ch);
//}

/// �ض���c�⺯��scanf��USART1
//int fgetc(FILE *f)
//{
		/* �ȴ�����1�������� */
//		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(USART1);
//}

/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART3
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
int fputc(int ch, FILE *f)
{
	/* ��Printf���ݷ������� */
	USART_SendData(USART1, (unsigned char) ch);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

/*
********************************************************************************************************
�������ƣ�void USART1_Send(unsigned char *tx_buf, int len)
�������ܣ�����1���ͺ���
Ӳ�����ӣ�PA9----TXD��PA10----RXD
��    ע��
��    ��:  2017-2-8
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
�������ƣ�void USART1_Receive(unsigned char *rx_buf, int len)
�������ܣ�����1���պ���
Ӳ�����ӣ�PA9----TXD��PA10----RXD
��    ע��
��    ��:  2017-2-8
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
�������ƣ�void USART1_IRQHandler(void)
�������ܣ�����1�жϷ�����
Ӳ�����ӣ�PA9----TXD��PA10----RXD
��    ע��
��    ��:  2017-2-8
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
	unsigned char Res;
	//u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
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
