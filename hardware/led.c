#include"led.h"
#include"sys.h"
#include "stm32f10x.h"


int led_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	
	return 0;
}
int Anquan(void){
		PBout(0) = 1;
		PBout(8) = 1;
		PEout(14) = 1;
		PCout(13) = 0;	
}

int Noanquan(void){
		PBout(0) = 0;
		PBout(8) = 0;
		PEout(14) = 0;
		PCout(13) = 1;
}

int Minianquan(void){
		PBout(0) = 0;
		PBout(8) = 0;
		PEout(14) = 1;
		PCout(13) = 1;
}
