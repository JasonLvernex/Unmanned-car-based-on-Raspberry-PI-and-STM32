#include "bsp_sys.h"
#include "stdio.h"
#include "all_define.h"
#include "jy901.h"
#include "string.h"
#include "UART2.h"
#include "uart1.h"
#include "UART3.h"
#include "car.h"



volatile uint32_t SysTick_count; //??????
_st_Mpu MPU6050;   //MPU6050????
_st_Mag AK8975;
_st_AngE Angle;    //???????
_st_Remote Remote; //?????


_st_ALL_flag ALL_flag; //?????,????????


PidObject pidRateX; //??PID??
PidObject pidRateY;
PidObject pidRateZ;

PidObject pidPitch; //??PID??
PidObject pidRoll;
PidObject pidYaw;

PidObject pidHeightRate;
PidObject pidHeightHigh;

//ע���������ڵ��жϷּ�����û�����ã�Ȼ�󣬲���һ��ģ�飡�Լ�timer�жϵ����ȼ�����
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

char ACCCALSW[5] = {0XFF,0XAA,0X01,0X01,0X00};//������ٶ�У׼ģʽ
char SAVACALSW[5]= {0XFF,0XAA,0X00,0X00,0X00};//���浱ǰ����
char SETREBACK[5]= {0XFF,0XAA,0X02,0X0E,0X00};//���浱ǰ����

	static unsigned char ucRxBuffer[250],uccRxBuffer[250];
 unsigned char ucRxCnt = 0,uccRxCnt=0,ucccRxCnt=0;	
  static unsigned short ucccRxBuffer[250];

  float test1=0;
 int YAW_ang;

//�ô���2��JYģ�鷢��ָ��
void sendcmd(char cmd[])
{
	char i;
	for(i=0;i<5;i++)
		UART2_Put_Char(cmd[i]);
}
//CopeSerialDataΪ����2�жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
void CopeSerial2Data(unsigned char ucData)
{

	
	
	
	//LED_REVERSE();					//���յ����ݣ�LED����˸һ��
	ucRxBuffer[ucRxCnt++]=ucData;	//���յ������ݴ��뻺������
	
}

void CopeSerial1Data(unsigned char ucData)
{	
	uccRxBuffer[uccRxCnt++]=ucData;	//���յ������ݴ��뻺������
}

void CopeSerial3Data(unsigned short ucData)
{	
	ucccRxBuffer[ucccRxCnt++]=ucData;	//���յ������ݴ��뻺������
	ucccRxCnt=0;
}


//pidobject�Ķ���

//extern int mpu_data;

void pid_param_Init(void); //PID???????,??PID???????,?????????????? ??????

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	

	//unsigned char i = 0,pp=0;

//***************************���þֲ�����***************************//

		
//*****************************************************************//
	
//*****************************ϵͳ��ʼ*****************************//
	
	STM32_System_Init();//����ϵͳ������������������


	
	pid_param_Init(); 


	//USART1_Init(9600);
	
/*	while(!MpuInit())
	{
		Car_Go();
	}*/
	
//	MpuGetData();
	//FlightPidControl(0.03f);
	
	//TIM_Cmd(TIM6,DISABLE);//������ʱ�� 
		
//*****************************************************************//

	
//*****************************************************************//

//044λ��
                ucccRxCnt=0;
					MOTOR_PWM_Out(3600,3600,3600,3600);  
					       Timer6_Init();
								/* while(1)
								 {
									 Car_Go();
								 }///*test*/
	while (1)
	{
		/*USART_SendData(USART3,0x06);
		Delay_ms(10);*/
		/**Car_Go();*/
		if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
	}
	if (ucRxCnt<11) {;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:			{/*for(pp=0;pp<2;pp++)
		                   {
			                  stcAngle.Angle[pp]=ucRxBuffer[(pp+1)*2]|ucRxBuffer[(pp+1)*2+1]<<8;
		                    };*/
				    stcAngle.Angle[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[2];
						stcAngle.Angle[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[4];
						stcAngle.Angle[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[6];//uint16_t
						stcAngle.T = ((unsigned short)ucRxBuffer[9]<<8)|ucRxBuffer[8];
				
		                 // USART_SendData(USART1,(float)stcAngle.Angle[2]/32768*180);
			                        break;}
			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);break;
			/*case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);break;
			case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8);break;
			case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8);break;
			case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8);break;
			case 0x59:	memcpy(&stcQ,&ucRxBuffer[2],8);break;*/
		}

			/*USART_SendData(USART1,ucRxBuffer[1]);
	    Delay_ms(10);*/
	//	USART_SendData(USART1,stcAngle.Angle[2]/32768*180);
		ucRxCnt=0;//��ջ�����
	}
	
	test1=(float)stcAngle.Angle[2]/32768*180;
	YAW_ang=(int)test1;
/////////////////////////////////////////////////////	
	switch (ucccRxBuffer[0])
	{
		case 0x08:  {
			 ucccRxCnt=0;
				//USART_SendData(USART3,0x09);
				Delay_ms(50);
				Car_Go();
			           break;}
		case 0x06:
		{
			 ucccRxCnt=0;
			//	USART_SendData(USART3,0x0A);
				Delay_ms(50);
				Car_Turn_Right();
			break;
		}
		case 0x05:
		{
			 ucccRxCnt=0;
			//	USART_SendData(USART3,0x0A);
				Delay_ms(50);
				Car_Stop();
			break;
		}		
		case 0x04:
		{
			 ucccRxCnt=0;
			//	USART_SendData(USART3,0x0A);
				Delay_ms(50);
				Car_Turn_Left();
			break;
		}	
				case 0x02:
		{
			 ucccRxCnt=0;
			//	USART_SendData(USART3,0x0A);
				Delay_ms(50);
				Car_Back();
			break;
		}	
						case 0x66:
		{
			 ucccRxCnt=0;
			//	USART_SendData(USART3,0x0A);
				Delay_ms(50);
				TIM_Cmd(TIM6, DISABLE);
			break;
		}	
								case 0x76:
		{
			 ucccRxCnt=0;
			//	USART_SendData(USART3,0x0A);
				Delay_ms(50);
				TIM_Cmd(TIM6, ENABLE);
			break;
		}	
		case 0x44:
		{
			 ucccRxCnt=0;
			//	USART_SendData(USART3,0x0A);
				////Delay_ms(50);
				//MOTOR_PWM_Out(3600,3600,3600,3600); 
       /** Car_Go();
        Delay_ms(300);
        Car_Turn_Right();
        Delay_ms(100);*/
			Car_Right();
			////Delay_ms(100);

//
//Car_Right();
//Car_Turn_Left();
	
	         //TIM_Cmd(TIM6, ENABLE);//������ʱ�� 
       Timer6_Init();
//Car_Go();

//Car_Back();

//        Car_Stop();
//Car_Go(); 
			break;
		}	
		case 0x98:
		{
			ucccRxCnt=0;
      Car_Go();
			break;
		}		
		case 0x52:
		{
			ucccRxCnt=0;
      Car_Left();
			break;
		}			
	}


	}	

}

void pid_param_Init(void)
{
	
///////////////////////////////////////////////	
	pidRateX.kp = 2.0f;
	pidRateY.kp = 2.0f;
	pidRateZ.kp = 4.0f;
	
	pidRateX.ki = 0.0f;
	pidRateY.ki = 0.0f;
	pidRateZ.ki = 0.0f;	
	
	pidRateX.kd = 0.08f;
	pidRateY.kd = 0.08f;
	pidRateZ.kd = 0.5f;	
	
	pidPitch.kp = 7.0f;
	pidRoll.kp = 7.0f;
	pidYaw.kp = 7.0f;	
////////////////////////////////////////////////////
	
//	pidRateX.kp = 2.0f;
//	pidRateY.kp = 2.0f;
//	pidRateZ.kp = 4.0f;
//	
//	pidRateX.ki = 0.0f;
//	pidRateY.ki = 0.0f;
//	pidRateZ.ki = 0.0f;	
//	
//	pidRateX.kd = 0.28f;
//	pidRateY.kd = 0.28f;
//	pidRateZ.kd = 0.4f;	
//	
//	pidPitch.kp = 7.0f;
//	pidRoll.kp = 7.0f;
//	pidYaw.kp = 7.0f;	

}

/*********************************************END OF FILE**********************/
