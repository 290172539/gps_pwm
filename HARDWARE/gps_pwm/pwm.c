#include "pwm.h"
#include "led.h"

extern volatile u8 Time1_hq_status;


/*
 *PWM输出初始化
 *arr：自动重装载
 *psc：时钟预分频数
*/
void TIM1_PWM_Init(u16 arr,u16 psc)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);				//使能tim1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);			//使能GPIO外设时钟
	
	
	/*设置该引脚为复用输出功能，输出TIM CH1的pwm脉冲波形*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;						//TIM_CH1
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;						//TIM_CH1

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	/*定时器结构体初始化*/
	TIM_TimeBaseStructure.TIM_Period = arr;							//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;						//设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//设置时钟分割：TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);					//初始化TIM1
	
	/*脉冲结构体初始化*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;				//脉宽调试模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性高
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);							//初始化 外设TIMx
	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_Trigger,ENABLE);				/*允许更新中断*/
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
	NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器

	TIM_CtrlPWMOutputs(TIM1,ENABLE);								//MOE住输出使能
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);				//ch1预装载使能
	TIM_ARRPreloadConfig(TIM1,ENABLE);								//使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM1,ENABLE);											//使能TIM1
	
	
}

/**************************************************************************************
 * @Author  linyibin
 * @Date	  2018-02-27
 * @Func		定时器的中断服务子函数
 * @Para		
   >
 * @Desc		
 * 			>定义一个变量，当定时器发生的时候我在主函数check这个变量
 * 			>check到变量进行延时，做数据跟脉冲的对准
****************************************************************************************/
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)	/*检查TIM1中断是否更新*/
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);			/*清除TIM1 更新中断标志*/
		Time1_hq_status = 1;														/*中断发生*/
	}
	//printf("TIME1_IRQHandler\n");
}