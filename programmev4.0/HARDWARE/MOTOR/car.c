#include "car.h"
#include "pid.h"
#include "bsp_sys.h"
#include "all_define.h"
#include "bsp_usart.h"

int Encoder_C,Encoder_D;  //编码器脉冲数
int Moto_C,Moto_D;//PWM 
int para_C,para_D;//增量
int mpu_data;
int q=0,k=0;

//这个设置很重要
int SetPoint=470;//设置目标值单位RPM，如果转换成线速度  = setopint*轮子周长 = setpoint*3.14*0.065(轮子直径65mm) 单位就是m/min  一定要搞清楚车轮转速和线速的关系

//使用减速比是1：120的减速箱。根据实际电机的减速比的配置修改以下参数
//6240=13*4*120：霍尔编码器13线，STM32编码器模式 4倍频，减速箱1：120
//2496=13*4*48：霍尔编码器13线，STM32编码器模式 4倍频，减速箱1：48
//11*4*17=748
//#define SetPointA SetPoint*6240/600                 //换算成编码器脉冲数，因为最终pid控制的是编码器的脉冲数量  #define SetPointC SetPoint*884/600
//#define SetPointB SetPoint*6240/600                 //换算成编码器脉冲数，因为最终pid控制的是编码器的脉冲数量
#define SetPointC SetPoint*748/600                 //换算成编码器脉冲数，因为最终pid控制的是编码器的脉冲数量
#define SetPointD SetPoint*748/600                 //换算成编码器脉冲数，因为最终pid控制的是编码器的脉冲数量
//在这里更改pid需求值
int Want_Yaw=-122;//-113;
extern int YAW_ang;
int get_it2;
short get_it1;


//各种变量
u32 temp1,temp2,temp3,temp4;
float sudu;
char set_speed[5];//车速显示小数
char speed[5];//车速显示小数

u8 speed11[5];//车速显示小数

//Time1定时器1中断服务函数
//1ms定时
void TIM6_IRQHandler(void)
{
	static uint8_t cnt_3ms = 0;
	static uint8_t cnt_100ms = 0,cnt_6ms;	 

	cnt_3ms++;
	cnt_100ms++;
	cnt_6ms++;
	if(TIM_GetFlagStatus(TIM6, TIM_IT_Update) != RESET) //时间到了
	{
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);     //清中断


    

		if(cnt_6ms == 6)  //6ms更新一次
		{
			cnt_6ms = 0;
			
		  mpu_data=PID_Calc_MPU(YAW_ang,Want_Yaw);	  //D电机，计数得到增量式PID的增量数值 
			if(mpu_data>3||mpu_data<-3)
			{Moto_C +=  mpu_data;
			Moto_D += -mpu_data; }
			else {Moto_C=Moto_D=3600;}
		}	
     	
				

//if(cnt_100ms == 100)
//{     cnt_100ms=0;q++;k++;
//  		
//			k=0;
//			Encoder_C=Read_Encoder(3)-30000;                   //读取编码器 
//			Encoder_D=Read_Encoder(4)-30000;                  //读取编码器
//	    get_it1=(short)TIM3 -> CNT;
//	    get_it2=TIM3 -> CNT;
//				 
//			//小车的移动速度，即轮子线速度
//			sudu = (Encoder_C+Encoder_D)/4*6000/748 * 0.032*3.1415;//计算车速，左轮和右轮速度和除以2 单位 m/min

//			if(Encoder_C<0)//如果电机反转了
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

//  //角度偏差过大的时候不做速度闭环

//			para_C=PID_Calc_C(Encoder_C,SetPointC);	  //C电机，计数得到增量式PID的增量数值 
//		  para_D=PID_Calc_D(Encoder_D,SetPointD);	  //D电机，计数得到增量式PID的增量数值 
//		
//	

//			if((para_C<-3)||(para_C>3))                     //不做 PID 调整，避免误差较小时频繁调节引起震荡。
//			{
//				Moto_C +=para_C;  
//			}   
//			if((para_D<-3)||(para_D>3))                     //不做 PID 调整，避免误差较小时频繁调节引起震荡。
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
			

			TIM8->CCR3=Moto_C;  //更新pwm
			TIM8->CCR4=Moto_D; //更新pwm


			
		

	
	
		
}

}

