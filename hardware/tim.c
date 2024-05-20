#include"tim.h"
#include "stm32f10x.h"


 int tim2_init(void){
		 
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		/* TIM2 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	 
		/* Enable the TIM2 global Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		
		NVIC_Init(&NVIC_InitStructure);
	    
		
	 
		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 7200-1;
		TIM_TimeBaseStructure.TIM_Prescaler = 2000-1; /*200ms*/
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		
		/* TIM IT enable */
		TIM_ITConfig(TIM2, TIM_IT_Update , ENABLE);
		TIM_Cmd(TIM2,ENABLE);


		return 0;
	 
}
  

int tim3_init(void){
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		/* TIM3 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
		/* Enable the TIM3 global Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		
		NVIC_Init(&NVIC_InitStructure);
	    
		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 7200-1;
		TIM_TimeBaseStructure.TIM_Prescaler = 8000-1; /*800ms*/
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
		/* TIM IT enable */
		TIM_ITConfig(TIM3, TIM_IT_Update , ENABLE);
		TIM_Cmd(TIM3,ENABLE);


		return 0;
	 
}
