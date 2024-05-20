#include "sr04.h"
#include "stm32f10x.h"
#include "motor - 2.0.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"

extern PWM_left;
extern PWM_right;
uint32_t distance;

extern char USART1_RX_BUF[128];     	        // 接收缓冲,最大USART_REC_LEN个字节.
extern u16 USART1_RX_STA;       					// 接收状态标记	
extern u16 USART1_RX_num;						// 接收计数偏移

extern char USART3_RX_BUF[128];     	        // 接收缓冲,最大USART_REC_LEN个字节.
extern u16 USART3_RX_STA;       					// 接收状态标记	
extern u16 USART3_RX_num;						// 接收计数偏移

int sr04_init(void){
	 GPIO_InitTypeDef 	GPIO_InitStructure;
	
	/* config GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);	 // 时钟		

	/* Configure PA1 in input pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);					

	/* Configure PA1 in input pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);		
	
	return 0;
}

//获取距离，单位为mm
uint32_t sr04_get_distance(void){
	uint32_t t=0;
	
	//发送触发信号
	PDout(8)=1;
	delay_us(20);
	PDout(8)=0;
	
	//等待输出回响信号
	while(PBin(14)==0);
		
	//测量回响信号时间
	
	while(PBin(14))
	{
		delay_us(8);
		
		t++;
	
	}	
	t = t/2;	
	return (3*t);
}


int Moshushou(void){
	while((strstr(USART1_RX_BUF, "moshushou") != NULL)||(strstr(USART3_RX_BUF, "moshushou") != NULL)){
		
			//获取距离
		distance = sr04_get_distance();

		
		if(1400 < distance && distance < 4000 ){
			GO();
			PCout(13) = 1;
		}
		else if(1000 < distance && distance < 1400){
			GO();
			PWM_left=50;
			PWM_right=50;
		    PCout(13) = 1;
			PBout(0) = 0;
		}
		else if(600 < distance && distance < 1000){
			BACK();
			PWM_left=60;
			PWM_right=60;
			PCout(13) = 1;
			PBout(0) = 0;
			PBout(8) = 0;
		}
		else if(0 < distance && distance < 600){
			BACK();
			PCout(13) = 1;
			PBout(0) = 0;
			PBout(8) = 0;
			
			PEout(14) = 0;
			delay_ms(200);
			PEout(14) = 1;
		}
	}
}

int Chaoshengbo(void){
			
		distance = sr04_get_distance();
				  
		if(1400 < distance && distance < 4000 ){
			PCout(13) = 1;
		}
		else if(1000 < distance && distance < 1400){
			
		    PCout(13) = 1;
			PBout(0) = 0;
		}
		else if(600 < distance && distance < 1000){
	
			PCout(13) = 1;
			PBout(0) = 0;
			PBout(8) = 0;
		}
		else if(0 < distance && distance < 600){
			
			PCout(13) = 1;
			PBout(0) = 0;
			PBout(8) = 0;
			PEout(14) = 0;
			delay_ms(200);
			
			PCout(13) = 0;
			PBout(0) = 1;
			PBout(8) = 1;
			PEout(14) = 1;
			delay_ms(200);
		}
}

int Ruku(void){
	while(1){
		
		if(USART3_RX_STA == 1)
		{
		printf("data:%s\n",USART3_RX_BUF);
		
		if(strstr(USART3_RX_BUF, "go") != NULL)
		{   
			Chaoshengbo();
			GO(); 			
		}
		else if(strstr(USART3_RX_BUF, "left") != NULL)
		{
			Chaoshengbo();
			BIGLEFT();
		}
		else if(strstr(USART3_RX_BUF, "right") != NULL)
		{
			Chaoshengbo();
			BIGRIGHT();
		}
		else if(strstr(USART3_RX_BUF, "back") != NULL)
		{
			BACK();
			Minianquan();
		}
		else if(strstr(USART3_RX_BUF, "stop") != NULL)
		{
			STOP();
			Minianquan();
		}
		else if(strstr(USART3_RX_BUF, "break") != NULL)
		{
			break;
		}
	}
	}
}
