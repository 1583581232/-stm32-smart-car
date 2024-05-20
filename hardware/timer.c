#include"timer.h"
#include "stm32f10x.h"


int timer_init(void)
{
	//定时器基本功能配置结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//定时器中断优先级配置结构体
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//使能定时器2的硬件时钟
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	  //打开时钟
	
	//初始化定时器2的基本定时功能
	TIM_TimeBaseInitStruct.TIM_CounterMode	=	TIM_CounterMode_Up;	//向上计数模式 -- 从小数到大
	TIM_TimeBaseInitStruct.TIM_Period	=  10-1;		// 计数值
	TIM_TimeBaseInitStruct.TIM_Prescaler =	720-1; 	//预分频值 72MHz/7200 = 10000Hz; 
															//10000Hz等价与 定时器数10000个数过了1000000微妙																												 //5000/10000 = 0.5s = 500ms
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStruct);

	//清空定时器更新事件的标志位
	TIM_ClearITPendingBit( TIM2, TIM_IT_Update);
	
	//启动定时器工作
	TIM_Cmd( TIM2, ENABLE);
	
	//设置定时器2更新中断( 定时器定时功能)
	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE);

	//设置定时器2优先级
	NVIC_InitStruct.NVIC_IRQChannel		=	TIM2_IRQn;				//设置定时器6中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd	=	ENABLE;						//中断使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	=	1;								//抢占式优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	=	0;								//响应式优先级
	NVIC_Init( &NVIC_InitStruct);
}

