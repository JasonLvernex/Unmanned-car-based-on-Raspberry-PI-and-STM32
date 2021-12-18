#include "pidcal.h"
#include "all_data.h"

struct PidCAL pid;
//位置环
int PID_output (void)
{
	float PIDOUT;
	pid.Ek=pid.Sv-pid.Pv;
	PIDOUT=pid.Kp*pid.Ek+pid.Ki*(pid.Ek+pid.Ek_1+pid.Ek_2)+pid.Kd*(pid.Ek-pid.Ek_1)+pid.OUT0;
	pid.Ek_2=pid.Ek_1;
	pid.Ek_1=pid.Ek;
	pid.PIDOUT=PIDOUT;
	pid.PIDOUT=(pid.PIDOUT<pid.lowlimit) ? pid.lowlimit:(pid.PIDOUT>pid.uplimit) ? pid.uplimit : pid.PIDOUT;
	return pid.PIDOUT;
}
//速度环
int PID1_output (void)
{
	float PIDOUT;
	pid1.Ek=pid1.Sv-pid1.Pv;
	PIDOUT=pid1.Kp*(pid1.Ek-pid1.Ek_1)+pid1.Ki*pid1.Ek+pid1.Kd*(pid1.Ek_2*pid1.Ek_1+pid1.Ek_2)+pid1.OUT0;
	pid1.Ek_2=pid1.Ek_1;
	pid1.Ek_1=pid1.Ek;
	pid1.PIDOUT+=PIDOUT;
	pid1.PIDOUT=(pid1.PIDOUT<pid1.lowlimit) ? pid1.lowlimit:(pid1.PIDOUT>pid1.uplimit) ? pid1.uplimit : pid1.PIDOUT;
	return pid1.PIDOUT;
}
//绝对值
float myabs(float a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
//输出
float Set_Pwm(float moto1)
{
	float OUT;
	if(moto1>0)			AIN2=0,			AIN1=1;
	else 	          AIN2=1,			AIN1=0;
	OUT=myabs(moto1);
	return OUT;
}

int combine(void)
{
if(pid.Ek>-100&&pid.Ek <100) OUT=0;
else OUT=PID1_output();
pwm=Set_Pwm(PID_output())+OUT;
	
}

