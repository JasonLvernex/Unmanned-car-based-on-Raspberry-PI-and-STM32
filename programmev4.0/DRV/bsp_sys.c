/*
********************************************************************************************************
�ļ�����bsp_sys.c
��  �ܣ���ʼ����������
��  ע��
��  ����https://shop60670850.taobao.com
��  �ߣ�lilianhe
��  ��: 2017-2-6
*********************************************************************************************************
*/

#include "bsp_sys.h"
#include "UART2.h"
#include "UART1.h"
#include "UART3.h"

void STM32_System_Init(void)
{	
	//STM32�ڲ���ʼ��
	Delay_Init(72);        //�δ�ʱ�ӳ�ʼ��
	NVIC_Config();         //�ж��������ó�ʼ��
//	LED_GPIO_Init();     //LED��ʼ��
	//USART1_Init(115200);   //���ڳ�ʼ��
	Initial_UART2(9600);
 // Initial_UART1(9600);
	 Initial_UART3(9600);
	
	//����豸��ʼ��

	MOTOR_GPIO_Init();     //�������IO����
  TIM8_PWM_Init(3600,1);      //PWMƵ�ʳ�ʼ��20KHz
	
//Encoder_Init_TIM2();TIM2 -> CNT=30000;
Encoder_Init_TIM3();TIM3 -> CNT=30000;
Encoder_Init_TIM4();TIM4 -> CNT=30000;
//Encoder_Init_TIM5();TIM5 -> CNT=30000;
/*	OLED_Init();
	OLED_Clear();
	OLED_ShowCHinese(0,0,2);  //��ʾ���� ��
	OLED_ShowCHinese(16,0,3); //��ʾ���� ��
	OLED_ShowString(88,0,"m/min",16);          //OLED��һ����ʾ���峵�� ��λm/min
	OLED_ShowString(0,2,"A:      B:      ",12);          //��ʾ����ת  R��λRPM ��ʾÿ���ӳ��ֵ�Ȧ��
	OLED_ShowString(0,3,"D:      C:      ",12);         //��ʾ����ת  R��λRPM ��ʾÿ���ӳ��ֵ�Ȧ��
	OLED_ShowString(0,4,"P:   I:   D:   ",12); //��ʾPID����
	OLED_ShowString(0,6,"Set_C:    RPM",12);      //�����õĳ���ת�ٵ�λRPM*/
	
	//Timer6_Init();

	
	Delay_ms(100);         //�ȴ���ʼ�����
}


