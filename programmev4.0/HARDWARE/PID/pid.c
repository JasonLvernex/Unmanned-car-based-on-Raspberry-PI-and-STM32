#include "pid.h"
//1.增量式pid
//定义PID相关宏
// 这三个参数设定对电机运行影响非常大
/*************************************/
float   Proportion=0.6;                               //比例常数 Proportional Const
float   Integral=0.1;                                 //积分常数 Integral Const
float   Derivative=0;                                 //微分常数 Derivative Const

float   Proportion_mpu=0.6;                               //比例常数 Proportional Const
float   Integral_mpu=0.1;                                 //积分常数 Integral Const
float   Derivative_mpu=0;                                 //微分常数 Derivative Const

//显示pid参数到OLED上
void showPID(void)
{
//	float temp1,temp2,temp3;
//	char PID_P[3],PID_I[3],PID_D[3];
//	temp1 = Proportion;
//	sprintf(PID_P,"%1.1f",temp1);
//  OLED_ShowString(16,4, PID_P,16);  
//	
//	temp2 = Integral;
//	sprintf(PID_I,"%1.1f",temp2);
//  OLED_ShowString(56,4, PID_I,16); 
//	
//	temp3 = Derivative;
//	sprintf(PID_D,"%1.1f",temp3);
//  OLED_ShowString(104,4, PID_D,16); 
}

/********************增量式PID控制设计************************************/
//NextPoint当前输出值
//SetPoint设定值

//左前A轮PID
int PID_Calc_A(int NextPoint,int SetPoint) 
{
                             
	static int      LastError;                                //Error[-1]
	static int      PrevError;                                //Error[-2]
  int iError,Outpid;                                        //当前误差
	
  iError=SetPoint-NextPoint;                                //增量计算
  Outpid=(Proportion * iError)                              //E[k]项
              -(Integral * LastError)                       //E[k-1]项
              +(Derivative * PrevError);                    //E[k-2]项
              
  PrevError=LastError;                                      //存储误差，用于下次计算
  LastError=iError;
  return(Outpid);                                           //返回增量值
}


//右前B轮PID
int PID_Calc_B(int NextPoint,int SetPoint) 
{                      
	static int      LastError;                                //Error[-1]
	static int      PrevError;                                //Error[-2]
  int iError,Outpid;                                        //当前误差
	
  iError=SetPoint-NextPoint;                                //增量计算
  Outpid=(Proportion * iError)                              //E[k]项
              -(Integral * LastError)                       //E[k-1]项
              +(Derivative * PrevError);                    //E[k-2]项
              
  PrevError=LastError;                                      //存储误差，用于下次计算
  LastError=iError;
  return(Outpid);                                           //返回增量值
}

//右后C轮PID
int PID_Calc_C(int NextPoint,int SetPoint) 
{
                             
	static int      LastError;                                //Error[-1]
	static int      PrevError;                                //Error[-2]
  int iError,Outpid;                                        //当前误差
	
  iError=SetPoint-NextPoint;                                //增量计算
  Outpid=(Proportion * iError)                              //E[k]项
              -(Integral * LastError)                       //E[k-1]项
              +(Derivative * PrevError);                    //E[k-2]项
              
  PrevError=LastError;                                      //存储误差，用于下次计算
  LastError=iError;
  return(Outpid);                                           //返回增量值
}


//左后D轮PID
int PID_Calc_D(int NextPoint,int SetPoint) 
{                      
	static int      LastError;                                //Error[-1]
	static int      PrevError;                                //Error[-2]
  int iError,Outpid;                                        //当前误差
	
  iError=SetPoint-NextPoint;                                //增量计算
  Outpid=(Proportion * iError)                              //E[k]项
              -(Integral * LastError)                       //E[k-1]项
              +(Derivative * PrevError);                    //E[k-2]项
              
  PrevError=LastError;                                      //存储误差，用于下次计算
  LastError=iError;
  return(Outpid);                                           //返回增量值
}

//MPUPID
int PID_Calc_MPU(int NextPoint,int SetPoint) 
{                      
	static int      LastError;                                //Error[-1]
	static int      PrevError;                                //Error[-2]
  int iError,Outpid;                                        //当前误差
	
  iError=SetPoint-NextPoint;                                //增量计算
  Outpid=(Proportion_mpu * iError)                              //E[k]项
              -(Integral_mpu * LastError)                       //E[k-1]项
              +(Derivative_mpu * PrevError);                    //E[k-2]项
              
  PrevError=LastError;                                      //存储误差，用于下次计算
  LastError=iError;
  return(Outpid);                                           //返回增量值
}
