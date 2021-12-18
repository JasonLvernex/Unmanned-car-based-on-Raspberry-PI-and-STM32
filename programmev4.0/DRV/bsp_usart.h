#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include <stdio.h>


void USART1_Init(u32 bound);
void NVIC_Configuration(void);
extern int fputc(int ch, FILE *f);
//int fgetc(FILE *f);
void USART1_Send(unsigned char *tx_buf, int len);
void USART1_Receive(unsigned char *rx_buf, int len);


#endif /* __USART1_H */
