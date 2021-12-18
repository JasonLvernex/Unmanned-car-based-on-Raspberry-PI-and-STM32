#include "pid2.h"

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：位置式PI控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
在我们的位置控制闭环系统里面，只使用PI控制
pwm=Kp*e(k)+Ki*∑e(k)
**************************************************************************/
int Position_PI (int Encoder,int Target)
{ 	
   float Kp=0.02,Ki=0.0002;	
	 static int Bias,Pwm;
	 static long Integral_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Integral_bias+=Bias;	               //求出偏差的积分
	 if(Integral_bias>1500000)  Integral_bias=1500000;   //积分限幅
	 if(Integral_bias<-1500000)  Integral_bias=-1500000; //积分限幅 
	 Pwm=Kp*Bias+Ki*Integral_bias;       //位置式PI控制器
	 return Pwm;                         //增量输出

}

/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI (int Encoder,int Target)
{ 	
   float Kp=200,Ki=200;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
