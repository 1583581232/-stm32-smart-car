#include"timer.h"
#include "stm32f10x.h"


int timer_init(void)
{
	//��ʱ�������������ýṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//��ʱ���ж����ȼ����ýṹ��
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//ʹ�ܶ�ʱ��2��Ӳ��ʱ��
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	  //��ʱ��
	
	//��ʼ����ʱ��2�Ļ�����ʱ����
	TIM_TimeBaseInitStruct.TIM_CounterMode	=	TIM_CounterMode_Up;	//���ϼ���ģʽ -- ��С������
	TIM_TimeBaseInitStruct.TIM_Period	=  10-1;		// ����ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler =	720-1; 	//Ԥ��Ƶֵ 72MHz/7200 = 10000Hz; 
															//10000Hz�ȼ��� ��ʱ����10000��������1000000΢��																												 //5000/10000 = 0.5s = 500ms
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStruct);

	//��ն�ʱ�������¼��ı�־λ
	TIM_ClearITPendingBit( TIM2, TIM_IT_Update);
	
	//������ʱ������
	TIM_Cmd( TIM2, ENABLE);
	
	//���ö�ʱ��2�����ж�( ��ʱ����ʱ����)
	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE);

	//���ö�ʱ��2���ȼ�
	NVIC_InitStruct.NVIC_IRQChannel		=	TIM2_IRQn;				//���ö�ʱ��6�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd	=	ENABLE;						//�ж�ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	=	1;								//��ռʽ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	=	0;								//��Ӧʽ���ȼ�
	NVIC_Init( &NVIC_InitStruct);
}

