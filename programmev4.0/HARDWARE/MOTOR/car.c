#include "car.h"
#include "pid.h"
#include "bsp_sys.h"
#include "all_define.h"
#include "bsp_usart.h"

int Encoder_C,Encoder_D;  //������������
int Moto_C,Moto_D;//PWM 
int para_C,para_D;//����
int mpu_data;
int q=0,k=0;

//������ú���Ҫ
int SetPoint=470;//����Ŀ��ֵ��λRPM�����ת�������ٶ�  = setopint*�����ܳ� = setpoint*3.14*0.065(����ֱ��65mm) ��λ����m/min  һ��Ҫ���������ת�ٺ����ٵĹ�ϵ

//ʹ�ü��ٱ���1��120�ļ����䡣����ʵ�ʵ���ļ��ٱȵ������޸����²���
//6240=13*4*120������������13�ߣ�STM32������ģʽ 4��Ƶ��������1��120
//2496=13*4*48������������13�ߣ�STM32������ģʽ 4��Ƶ��������1��48
//11*4*17=748
//#define SetPointA SetPoint*6240/600                 //����ɱ���������������Ϊ����pid���Ƶ��Ǳ���������������  #define SetPointC SetPoint*884/600
//#define SetPointB SetPoint*6240/600                 //����ɱ���������������Ϊ����pid���Ƶ��Ǳ���������������
#define SetPointC SetPoint*748/600                 //����ɱ���������������Ϊ����pid���Ƶ��Ǳ���������������
#define SetPointD SetPoint*748/600                 //����ɱ���������������Ϊ����pid���Ƶ��Ǳ���������������
//���������pid����ֵ
int Want_Yaw=-122;//-113;
extern int YAW_ang;
int get_it2;
short get_it1;


//���ֱ���
u32 temp1,temp2,temp3,temp4;
float sudu;
char set_speed[5];//������ʾС��
char speed[5];//������ʾС��

u8 speed11[5];//������ʾС��

//Time1��ʱ��1�жϷ�����
//1ms��ʱ
void TIM6_IRQHandler(void)
{
	static uint8_t cnt_3ms = 0;
	static uint8_t cnt_100ms = 0,cnt_6ms;	 

	cnt_3ms++;
	cnt_100ms++;
	cnt_6ms++;
	if(TIM_GetFlagStatus(TIM6, TIM_IT_Update) != RESET) //ʱ�䵽��
	{
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);     //���ж�


    

		if(cnt_6ms == 6)  //6ms����һ��
		{
			cnt_6ms = 0;
			
		  mpu_data=PID_Calc_MPU(YAW_ang,Want_Yaw);	  //D����������õ�����ʽPID��������ֵ 
			if(mpu_data>3||mpu_data<-3)
			{Moto_C +=  mpu_data;
			Moto_D += -mpu_data; }
			else {Moto_C=Moto_D=3600;}
		}	
     	
				

//if(cnt_100ms == 100)
//{     cnt_100ms=0;q++;k++;
//  		
//			k=0;
//			Encoder_C=Read_Encoder(3)-30000;                   //��ȡ������ 
//			Encoder_D=Read_Encoder(4)-30000;                  //��ȡ������
//	    get_it1=(short)TIM3 -> CNT;
//	    get_it2=TIM3 -> CNT;
//				 
//			//С�����ƶ��ٶȣ����������ٶ�
//			sudu = (Encoder_C+Encoder_D)/4*6000/748 * 0.032*3.1415;//���㳵�٣����ֺ������ٶȺͳ���2 ��λ m/min

//			if(Encoder_C<0)//��������ת��
//			{
//				Encoder_C = -Encoder_C;
//			}
//				if(Encoder_D<0)
//			{
//				Encoder_D = -Encoder_D;
//			}
//			
//	if(mpu_data<100) // mpu_data<100	
//	{		

//  //�Ƕ�ƫ������ʱ�����ٶȱջ�

//			para_C=PID_Calc_C(Encoder_C,SetPointC);	  //C����������õ�����ʽPID��������ֵ 
//		  para_D=PID_Calc_D(Encoder_D,SetPointD);	  //D����������õ�����ʽPID��������ֵ 
//		
//	

//			if((para_C<-3)||(para_C>3))                     //���� PID ��������������СʱƵ�����������𵴡�
//			{
//				Moto_C +=para_C;  
//			}   
//			if((para_D<-3)||(para_D>3))                     //���� PID ��������������СʱƵ�����������𵴡�
//			{
//				Moto_D +=para_D;  
//			}
//		}
//			

//			}
		if(Moto_C>3600) Moto_C=3600;
		if(Moto_C<100) Moto_C=100;
		if(Moto_D>3600) Moto_D=3600;		
		if(Moto_D<100) Moto_D=100;
			

			TIM8->CCR3=Moto_C;  //����pwm
			TIM8->CCR4=Moto_D; //����pwm


			
		

	
	
		
}

}

