#include "pwm.h"
#include "led.h"

extern volatile u8 Time1_hq_status;


/*
 *PWM�����ʼ��
 *arr���Զ���װ��
 *psc��ʱ��Ԥ��Ƶ��
*/
void TIM1_PWM_Init(u16 arr,u16 psc)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);				//ʹ��tim1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);			//ʹ��GPIO����ʱ��
	
	
	/*���ø�����Ϊ����������ܣ����TIM CH1��pwm���岨��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;						//TIM_CH1
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;						//TIM_CH1

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	/*��ʱ���ṹ���ʼ��*/
	TIM_TimeBaseStructure.TIM_Period = arr;							//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc;						//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//����ʱ�ӷָTDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);					//��ʼ��TIM1
	
	/*����ṹ���ʼ��*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;				//�������ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;								//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//������Ը�
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);							//��ʼ�� ����TIMx
	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_Trigger,ENABLE);				/*��������ж�*/
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���

	TIM_CtrlPWMOutputs(TIM1,ENABLE);								//MOEס���ʹ��
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);				//ch1Ԥװ��ʹ��
	TIM_ARRPreloadConfig(TIM1,ENABLE);								//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM1,ENABLE);											//ʹ��TIM1
	
	
}

/**************************************************************************************
 * @Author  linyibin
 * @Date	  2018-02-27
 * @Func		��ʱ�����жϷ����Ӻ���
 * @Para		
   >
 * @Desc		
 * 			>����һ������������ʱ��������ʱ������������check�������
 * 			>check������������ʱ�������ݸ�����Ķ�׼
****************************************************************************************/
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)	/*���TIM1�ж��Ƿ����*/
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);			/*���TIM1 �����жϱ�־*/
		Time1_hq_status = 1;														/*�жϷ���*/
	}
	//printf("TIME1_IRQHandler\n");
}