/*
��д�ߣ�Kevin
��ַ��http://RobotControl.taobao.com
����E-mail��1609370741@qq.com
���뻷����MDK-Lite  Version: 5.17
����ʱ��: 2016-1-31
���ԣ� ���������ڡ������ǿء���STM32Coreƽ̨����ɲ���
���ܣ�
��STM32Coreƽ̨����2��ȡJY901�����ݣ�Ȼ��ͨ������1��ӡ����������,�������ֲ�����ҪѡΪ9600��
JY-901�Ĳ�����Ҫ�޸�Ϊ9600.
ע�⣺ʾ�������������ASCLL�룬��16���ƣ�HEX����ʾ�ǲ��ܿ���׼ȷ���ݵġ�
Ӳ�����ߣ�
USB-TTL����                 STM32Core               JY901
VCC          -----           VCC        ----         VCC
TX           -----           RX1���ܽ�10��     
RX           -----           TX1���ܽ�9��
GND          -----           GND        ----          GND
                             RX2 ���ܽ�3��       ----  TX
							 TX2  ���ܽ�2��      ----  RX
------------------------------------
 */
#include <string.h>
#include <stdio.h>
#include "Main.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "UART1.h"
#include "UART2.h"
#include "delay.h"
#include "IOI2C.h"
#include "hw_config.h"
#include "JY901.h"
#include "DIO.h"

struct STime		stcTime;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
struct SMag 		stcMag;
struct SDStatus stcDStatus;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SGPSV 		stcGPSV;
struct SQ       stcQ;

//CopeSerialDataΪ����2�жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
void CopeSerial2Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	LED_REVERSE();					//���յ����ݣ�LED����˸һ��
	USB_TxWrite(&ucData,1);			//��USB-HID�˿�ת���յ��Ĵ������ݣ���ֱ���ý���λ������ģ����������ݡ�
	ucRxBuffer[ucRxCnt++]=ucData;	//���յ������ݴ��뻺������
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);break;
			case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);break;
			case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8);break;
			case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8);break;
			case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8);break;
			case 0x59:	memcpy(&stcQ,&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;//��ջ�����
	}
}

void CopeSerial1Data(unsigned char ucData)
{	
	UART2_Put_Char(ucData);//ת������1�յ������ݸ�����2��JYģ�飩
}


int main(void)
{  
	char str[100];
	unsigned char len,i;
		
	USB_Config();		//����USB-HID
	SysTick_init(72,10);//����ʱ��Ƶ��
	Initial_UART1(9600);//��PC�Ĵ���
	Initial_UART2(9600);//��JY-901ģ��Ĵ���	
	
	LED_ON();
	delay_ms(1000);//�ȵ�JY-91��ʼ�����
	while(1)
	{			
			delay_ms(500);
		//���ʱ��
		sprintf(str,"Time:20%d-%d-%d %d:%d:%.3f\r\n",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);
			UART1_Put_String(str);		
			delay_ms(10);
		//������ٶ�
		sprintf(str,"Acc:%.3f %.3f %.3f\r\n",(float)stcAcc.a[0]/32768*16,(float)stcAcc.a[1]/32768*16,(float)stcAcc.a[2]/32768*16);
			UART1_Put_String(str);
			delay_ms(10);
		//������ٶ�
		sprintf(str,"Gyro:%.3f %.3f %.3f\r\n",(float)stcGyro.w[0]/32768*2000,(float)stcGyro.w[1]/32768*2000,(float)stcGyro.w[2]/32768*2000);
			UART1_Put_String(str);
			delay_ms(10);
		//����Ƕ�
		sprintf(str,"Angle:%.3f %.3f %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
			UART1_Put_String(str);
			delay_ms(10);
		//����ų�
		sprintf(str,"Mag:%d %d %d\r\n",stcMag.h[0],stcMag.h[1],stcMag.h[2]);
			UART1_Put_String(str);		
			delay_ms(10);
		//�����ѹ���߶�
		sprintf(str,"Pressure:%ld Height%.2f\r\n",stcPress.lPressure,(float)stcPress.lAltitude/100);
			UART1_Put_String(str); 
			delay_ms(10);
		//����˿�״̬
		sprintf(str,"DStatus:%d %d %d %d\r\n",stcDStatus.sDStatus[0],stcDStatus.sDStatus[1],stcDStatus.sDStatus[2],stcDStatus.sDStatus[3]);
			UART1_Put_String(str);
			delay_ms(10);
		//�����γ��
		sprintf(str,"Longitude:%ldDeg%.5fm Lattitude:%ldDeg%.5fm\r\n",stcLonLat.lLon/10000000,(double)(stcLonLat.lLon % 10000000)/1e5,stcLonLat.lLat/10000000,(double)(stcLonLat.lLat % 10000000)/1e5);
			UART1_Put_String(str);
			delay_ms(10);
		//�������
		sprintf(str,"GPSHeight:%.1fm GPSYaw:%.1fDeg GPSV:%.3fkm/h\r\n",(float)stcGPSV.sGPSHeight/10,(float)stcGPSV.sGPSYaw/10,(float)stcGPSV.lGPSVelocity/1000);
			UART1_Put_String(str);
			delay_ms(10);
		//�����Ԫ��
		sprintf(str,"Four elements:%.5f %.5f %.5f %.5f\r\n\r\n",(float)stcQ.q[0]/32768,(float)stcQ.q[1]/32768,(float)stcQ.q[2]/32768,(float)stcQ.q[3]/32768);
			UART1_Put_String(str);
		    delay_ms(10);//�ȴ��������
	}//��ѭ��
}



