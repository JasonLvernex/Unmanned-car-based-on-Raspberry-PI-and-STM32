/*
********************************************************************************************************
文件名：bsp_timer8.c
功  能：adc的配置
备  注：
淘  宝：https://shop60670850.taobao.com
作  者：lilianhe
日  期: 2017-2-6
                 普通车轮及麦克纳姆轮安装方向，TIM2-TIM5使用编码器功能

      TIM5  A轮  \\   ------   //  B轮 TIM2
						     \\   ------  //
							        ------
									    ------
									    ------
							    //  ------  \\
			 TIM4 D轮  //   ------   \\  C轮 TIM3
 
*********************************************************************************************************
*/
// timer8 控制ABCD四个轮子的PWM
//TIM8_CH1----A轮 //左前
//TIM8_CH2----B轮 //右前
//TIM8_CH3----C轮 //右后
//TIM8_CH4----D轮 //左后

#include "bsp_timer.h"
//PWM的频率 = 72MHz/ARR/PCS 例如  20K = 72M/3600/1 =  20KHz
void TIM8_PWM_Init(u16 arr,u16 psc)
{  
        GPIO_InitTypeDef GPIO_InitStructure;
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        TIM_OCInitTypeDef  TIM_OCInitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);//
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //使能GPIO外设时钟使能      

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM8_CH1-4
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
       
        TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值         80K
        TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值  不分频
        TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
				TIM_TimeBaseStructure.TIM_RepetitionCounter=0; //高级定时器1是用定时器功能配置这个才可以是正常的计数频率一开始的72mhz 值得注意的地方      
				TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 50; //设置待装入捕获比较寄存器的脉冲值
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
        TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
        TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1预装载使能       
       
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 50; //设置待装入捕获比较寄存器的脉冲值
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
        TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
        TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH2预装载使能       
       
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 50; //设置待装入捕获比较寄存器的脉冲值
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
        TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
        TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH3预装载使能       
       
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
        TIM_OCInitStructure.TIM_Pulse = 50; //设置待装入捕获比较寄存器的脉冲值
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
        TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
        TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH4预装载使能       
       
        TIM_ARRPreloadConfig(TIM8, ENABLE);              //使能TIMx在ARR上的预装载寄存器

        TIM_Cmd(TIM8, ENABLE);  //使能TIM8
        TIM_CtrlPWMOutputs(TIM8,ENABLE);        //MOE 主输出使能       

}
//***************************A电机占空比调节***************************//
//占空比 = TIMx_CCRx / TIMx_ARR
//数值 0-3600  占空比=设置值/3600
//电机驱动L298N建议设置65%以上占空比
//***************************占空比调节***************************//
/*void MOTOR_PWM_OutA(int zkb)
{
	TIM8->CCR1 = zkb;
}	*/

//***************************B电机占空比调节***************************//
//占空比 = TIMx_CCRx / TIMx_ARR
//数值 0-3600
//***************************占空比调节***************************//
/*void MOTOR_PWM_OutB(int zkb)
{
	TIM8->CCR2 = zkb;
}	*/

//***************************C电机占空比调节***************************//
//占空比 = TIMx_CCRx / TIMx_ARR
//数值 0-3600
//***************************占空比调节***************************//
void MOTOR_PWM_OutC(int zkb)
{
	TIM8->CCR2=zkb;//TIM8->CCR3=zkb;
}

//***************************D电机占空比调节***************************//
//占空比 = TIMx_CCRx / TIMx_ARR
//数值 0-3600
//***************************占空比调节***************************//
void MOTOR_PWM_OutD(int zkb)
{
	TIM8->CCR1 = zkb;//TIM8->CCR4 = zkb;

}

//四个轮子占空比调节
void MOTOR_PWM_Out(int zkbA,int zkbB,int zkbC,int zkbD)
{
	//A轮：左前轮              //B轮：右前轮
	//MOTOR_PWM_OutA(zkbA);     MOTOR_PWM_OutB(zkbB);  
	
	//C轮：左后轮              //D轮：右后轮
	MOTOR_PWM_OutC(zkbC);     MOTOR_PWM_OutD(zkbD); 
}



//***************************定时器1初始化 系统每1ms处理一次中断，更新数据，更新pwm等***************************//
void Timer6_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 99;//自动重新装载寄存器周期的值澹ㄥ计数值澹)
	TIM_TimeBaseStructure.TIM_Prescaler = 719;//时钟分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//对外部时钟进行采样的时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0; //高级定时器1是用定时器功能配置这个才可以是正常的计数频率一开始的72mhz 值得注意的地方
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);//参数初始化

	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM6, ENABLE);//启动定时器 
}



//***************************定时器2初始化 ，使用编码器功能***************************//
//左电机编码器计数
//PB3----接 编码器A相 或者电机驱动的BM1A标识
//PA15----接 编码器B相 或者电机驱动的BM1B标识
void Encoder_Init_TIM2(void)
{
GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   

	//GPIO功能时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //禁用JTAG
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

	//配置IO口为复用功能-定时器通道
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//TIM时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//Timer configuration in Encoder mode 
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	//Reset counter
	TIM2->CNT = 0;

	TIM_Cmd(TIM2, ENABLE);   
}

//***************************定时器3初始化 ，使用编码器功能***************************//
//左电机编码器计数
//PB4----接 编码器A相 或者电机驱动的BM2B标识
//PB5----接 编码器B相 或者电机驱动的BM2A标识
void Encoder_Init_TIM3(void)
{
		GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   

	//GPIO功能时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	//配置IO口为复用功能-定时器通道
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//TIM时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE); //这个就是重映射功能函数

	//Timer configuration in Encoder mode 
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	//Reset counter
	TIM_SetCounter(TIM3,0);

	TIM_Cmd(TIM3, ENABLE);  
	
	
}

//***************************定时器4初始化 ，使用编码器功能***************************//
//复用TIM4
//PB6----接 编码器A相 或者电机驱动的BM1A标识
//PB7----接 编码器B相 或者电机驱动的BM1B标识
void Encoder_Init_TIM4(void)
{
		GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   

	//GPIO功能时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	//配置IO口为复用功能-定时器通道
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//TIM时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//Timer configuration in Encoder mode 
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	//Reset counter
	TIM_SetCounter(TIM4,0);

	TIM_Cmd(TIM4, ENABLE); 
}

//***************************定时器5初始化 ，使用编码器功能***************************//
//复用TIM5
//PA0----接 编码器A相 或者电机驱动的BM2A标识
//PA1----接 编码器B相 或者电机驱动的BM2B标识
void Encoder_Init_TIM5(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	//GPIO功能时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//配置IO口为复用功能-定时器通道
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//TIM时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	//Timer configuration in Encoder mode 
	TIM_DeInit(TIM5);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);

	//Reset counter
	TIM_SetCounter(TIM5,0);
	TIM_Cmd(TIM5, ENABLE);    
}





/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
	   //case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=30000;break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT; TIM3 -> CNT=30000;break;	
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=30000;break;	
		// case 5:  Encoder_TIM= (short)TIM5 -> CNT;  TIM5 -> CNT=30000;break;
		 default:  Encoder_TIM=0;
	 }
		return Encoder_TIM;
}

