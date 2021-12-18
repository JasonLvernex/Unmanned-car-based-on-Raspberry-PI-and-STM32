#include "pid2.h"

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�λ��ʽPI������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
�����ǵ�λ�ÿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm=Kp*e(k)+Ki*��e(k)
**************************************************************************/
int Position_PI (int Encoder,int Target)
{ 	
   float Kp=0.02,Ki=0.0002;	
	 static int Bias,Pwm;
	 static long Integral_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Integral_bias+=Bias;	               //���ƫ��Ļ���
	 if(Integral_bias>1500000)  Integral_bias=1500000;   //�����޷�
	 if(Integral_bias<-1500000)  Integral_bias=-1500000; //�����޷� 
	 Pwm=Kp*Bias+Ki*Integral_bias;       //λ��ʽPI������
	 return Pwm;                         //�������

}

/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI (int Encoder,int Target)
{ 	
   float Kp=200,Ki=200;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
