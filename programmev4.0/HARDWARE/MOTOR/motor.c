/*
********************************************************************************************************
�ļ�����motor.c
��  �ܣ�ֱ�����ٵ������
��  ע��	
��  ����https://shop60670850.taobao.com
��  �ߣ�lilianhe
��  ��: 2017-2-6

             �����ķ�ְ�װ����

      TIM5  A��  \\   ------   //  B�� TIM2
						     \\   ------  //
							        ------
									    ------
									    ------
							    //  ------  \\
			 TIM4 D��  //   ------   \\  C�� TIM3
				
			//�������̼����������ķ�ֵ���
			//Ӳ������˵����
			timer8 ����ABCD�ĸ����ӵ�PWM
			TIM8_CH1--PC6--A�� //��ǰ ---- �������1-ENA
			TIM8_CH2--PC7--B�� //��ǰ ---- �������1-ENB
			TIM8_CH3--PC8--C�� //�Һ� ---- �������2-ENA
		  TIM8_CH4--PC9--D�� //��� ---- �������2-ENB
			
			
			A�֣�PC1 PC0 ����ǰ���˶�   PC1----IN2��PC0----IN1
			B�֣�PC3 PC2 ����ǰ���˶�   PC3----IN4��PC2----IN3			
			C�֣�PC10��PC11����ǰ���˶� PC10 --- IN1��PC11---IN2
			D�֣�PC12��PD2����ǰ���˶�  PC12 --- IN3��PD2 ---IN4
				
				
			�粻��˵����������ʹ��L298N������������ʹ��TB6612��Ҫ�Լ��޸�  �ײ�
		    
*********************************************************************************************************
*/

#include "motor.h"
//***************************���õ������IO��***************************//

void MOTOR_GPIO_Init(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);                                	  /*����GPIO������ʱ��*/																   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 ;	/*ѡ��Ҫ���Ƶ�GPIO����*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*��������ģʽΪͨ���������*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*������������Ϊ50MHz */ 
		GPIO_Init(GPIOC,&GPIO_InitStructure); 	/*���ÿ⺯������ʼ��GPIO*/	
	
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);                                	  /*����GPIO������ʱ��*/																   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	/*ѡ��Ҫ���Ƶ�GPIO����*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*��������ģʽΪͨ���������*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*������������Ϊ50MHz */ 
		GPIO_Init(GPIOD, &GPIO_InitStructure);                                    /*���ÿ⺯������ʼ��GPIO*/		
}
//��ǰA���
void MOTOR_A(char state)
{
	if(state == GO)//��ǰ���ǰ��
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_1);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_0);	
	}
	if(state == BACK)//��ǰ�������
	{
	

		GPIO_SetBits(GPIOC,GPIO_Pin_0);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_1);
		
		
	}
	if(state == STOP)//ͣת
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	}
		
		
}

//��ǰB���
void MOTOR_B(char state)
{
	if(state == GO)//��ǰ���ǰ��
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_2);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	}
	if(state == BACK)//��ǰ�������
	{
		
		GPIO_SetBits(GPIOC,GPIO_Pin_3);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	
	}
	if(state == STOP)//ͣת
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	}
}

//���C���
void MOTOR_C(char state)
{
	if(state == GO)//�����ǰ��
	{
			GPIO_SetBits(GPIOC,GPIO_Pin_10);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_11);

	}
	if(state == BACK)//���������
	{

	
		GPIO_SetBits(GPIOC,GPIO_Pin_11);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_10);
	
	}
	if(state == STOP)//ͣת
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_10);
	}
		
}

//�Һ�D���
void MOTOR_D(char state)
{
	if(state == GO)//�Һ���ǰ��
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_0);//ԭ����D2���������һ��
	  GPIO_ResetBits(GPIOC,GPIO_Pin_12);
		
	}
	if(state == BACK)//�Һ�������
	{

		GPIO_SetBits(GPIOC,GPIO_Pin_12);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	}
	if(state == STOP)//ͣת
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_12);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	}
		
}


//***************************ǰ��***************************//
//ֻҪ����INx()��״̬�Ϳ��Ըı���ת������
void Car_Go(void)
{
/*	//��ǰ��� ǰ    //��ǰ��� ǰ
	MOTOR_A(GO);       MOTOR_B(GO);*/

	//����� ǰ   //�Һ��� ǰ
	MOTOR_D(GO);       MOTOR_C(GO);
	
	
}

void Car_Back(void)
{
/*		//��ǰ��� ��    //��ǰ��� ��
MOTOR_A(BACK);       MOTOR_B(BACK);*/

	//����� ��   //�Һ��� ��
MOTOR_D(BACK);       MOTOR_C(BACK);
}

//***************************����תȦ***************************//
void Car_Turn_Left(void)
{
	
/*	//��ǰ��� ��    //��ǰ��� ǰ
MOTOR_A(BACK);       MOTOR_B(GO);*/

	//����� ��   //�Һ��� ǰ
MOTOR_D(BACK);       MOTOR_C(GO);
	
}
//***************************����תȦ***************************//
void Car_Turn_Right(void)
{
	
/*	//��ǰ��� ǰ    //��ǰ��� ��
MOTOR_A(GO);       MOTOR_B(BACK);*/

	//����� ǰ   //�Һ��� ��
MOTOR_D(GO);       MOTOR_C(BACK);
	
}


//***************************����ת***************************//
void Car_Right(void)
{
/*	//��ǰ��� ǰ    //��ǰ��� ͣ
MOTOR_A(GO);       MOTOR_B(STOP);*/

	//����� ǰ   //�Һ��� ͣ
  MOTOR_D(GO);       MOTOR_C(STOP);
	
}
//***************************����ת***************************//
void Car_Left(void)
{
	
/*	//��ǰ��� ��    //��ǰ��� ǰ
MOTOR_A(STOP);       MOTOR_B(GO);*/

	//����� ��   //�Һ��� ǰ
MOTOR_D(STOP);       MOTOR_C(GO);
	
}



//***************************ͣ��***************************//
void Car_Stop(void)
{
/*	//��ǰ��� ͣ    //��ǰ��� ͣ
MOTOR_A(STOP);       MOTOR_B(STOP);*/

	//����� ͣ   //�Һ��� ͣ
  MOTOR_D(STOP);       MOTOR_C(STOP);
}



