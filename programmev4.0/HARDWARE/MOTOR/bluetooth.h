#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "bsp_sys.h"
#include "stm32f10x.h"

void Bluetooth_Init(u32 bound);
void USART2_Send(unsigned char *tx_buf, int len);
void USART2_Receive(unsigned char *rx_buf, int len);

#endif
