#include"motor.h"
#include "sys.h"
#include "stm32f10x.h"

int motor_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_11|GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_6|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	
	return 0;

}
int go(void){
		PDout(4) = 1;
		PCout(11) = 1;
		PDout(0) = 0;
		PDout(2) = 1;
		PDout(6) = 1;
		PGout(9) = 0;

}

int back(void){
		PDout(4) = 1;
		PCout(11) = 0;
		PDout(0) = 1;
		PDout(2) = 1;
		PDout(6) = 0;
		PGout(9) = 1;


}

int right(void){
		PDout(4) = 1;
		PCout(11) = 0;
		PDout(0) = 1;
		PDout(2) = 1;
		PDout(6) = 1;
		PGout(9) = 0;
}


int left(void){
		PDout(4) = 1;
		PCout(11) = 1;
		PDout(0) = 0;
		PDout(2) = 1;
		PDout(6) = 0;
		PGout(9) = 1;

}

int minileft(void){
		PDout(4) = 1;
		PCout(11) = 0;
		PDout(0) = 0;
		PDout(2) = 1;
		PDout(6) = 1;
		PGout(9) = 0;

}

int miniright(void){
		PDout(4) = 1;
		PCout(11) = 1;
		PDout(0) = 0;
		PDout(2) = 1;
		PDout(6) = 0;
		PGout(9) = 0;

}

int stop(void){
		PDout(4) = 1;
		PCout(11) = 1;
		PDout(0) = 1;
		PDout(2) = 1;
		PDout(6) = 1;
		PGout(9) = 1;
}
