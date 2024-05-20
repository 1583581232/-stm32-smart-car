#include"motor - 2.0.h"
#include "sys.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "usart.h"


extern char USART1_RX_BUF[128];     	        // 接收缓冲,最大USART_REC_LEN个字节.
extern u16 USART1_RX_STA;       					// 接收状态标记	
extern u16 USART1_RX_num;						// 接收计数偏移

extern char USART3_RX_BUF[128];     	        // 接收缓冲,最大USART_REC_LEN个字节.
extern u16 USART3_RX_STA;       					// 接收状态标记	
extern u16 USART3_RX_num;						// 接收计数偏移

int Pump_init(void)
{
	//	1	结构体变量定义
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//	2 	对应组时钟使能
	/* GPIOC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		// 使能对应组的电源
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);		// 使能对应组的电源
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);		// 使能对应组的电源
	
	//	3	功能配置	
	/* Configure  in output pushpull mode */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//	电平切换的速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			// 输出推挽

	//	4	初始化
		//	左
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;					// 对应组的引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);						// 通过结构体 初始化 C 组
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4;					// 对应组的引脚
	GPIO_Init(GPIOD, &GPIO_InitStructure);						// 通过结构体 初始化 C 组
	
		// 右
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_6;					// 对应组的引脚
	GPIO_Init(GPIOD, &GPIO_InitStructure);						// 通过结构体 初始化 D 组
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					// 对应组的引脚
	GPIO_Init(GPIOG, &GPIO_InitStructure);						// 通过结构体 初始化 G 组
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);					// 让输出低电平
	GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_4);		// 让输出低电平
	
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);					// 让输出低电平
	GPIO_ResetBits(GPIOD,GPIO_Pin_2 | GPIO_Pin_6);		// 让输出低电平
}

int GO (void)
{
	//	左
	//GPIO_SetBits(GPIOD,GPIO_Pin_4);			// 让PD4输出高电平		1
	GPIO_SetBits(GPIOC,GPIO_Pin_11);		// 让PC11输出高电平		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);		// 让PD0输出低电平		0
	
	//	右
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// 让输出高电平		1
	GPIO_SetBits(GPIOD,GPIO_Pin_6);			// 让输出高电平		1
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);		// 让输出低电平		0
}

int BACK(void)
{
	//	左
	//GPIO_SetBits(GPIOD,GPIO_Pin_4);			// 让PD4输出高电平		1
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);		// 让PC11输出高电平		0
	GPIO_SetBits(GPIOD,GPIO_Pin_0);			// 让PD0输出低电平		1
	
	//	右
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// 让输出高电平		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);		// 让输出高电平		0
	GPIO_SetBits(GPIOG,GPIO_Pin_9);			// 让输出低电平		1
}
int LEFT(void)	// 左
{
	//	左
	//GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// 让输出高电平		0
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);		// 让输出高电平		0
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);		// 让输出低电平		0
	
	//	右
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// 让输出高电平		1
	GPIO_SetBits(GPIOD,GPIO_Pin_6);			// 让输出高电平		1
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);		// 让输出低电平		0
}
int BIGLEFT(void)	// 左
{
	//	左
	//GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// 让输出高电平		0
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);		// 让输出高电平		0
	GPIO_SetBits(GPIOD,GPIO_Pin_0);		 // 让输出低电平		0
	
	//	右
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// 让输出高电平		1
	GPIO_SetBits(GPIOD,GPIO_Pin_6);			// 让输出高电平		1
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);		// 让输出低电平		0
}
int RIGHT(void)	// 右
{
	//	左
	//GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// 让输出高电平		0
	GPIO_SetBits(GPIOC,GPIO_Pin_11);		// 让输出高电平		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);		// 让输出低电平		0
	
	//	右
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// 让输出高电平		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);			// 让输出高电平		0
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);		// 让输出低电平		0
}
int BIGRIGHT(void)	// 右
{
	//	左
	//GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// 让输出高电平		0
	GPIO_SetBits(GPIOC,GPIO_Pin_11);		// 让输出高电平		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);		// 让输出低电平		0
	
	//	右
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// 让输出高电平		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);			// 让输出高电平		0
	GPIO_SetBits(GPIOG,GPIO_Pin_9);		// 让输出低电平		0
}
int STOP(void)
{
	//	左
	//GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// 让PD4输出低电平		0
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);		// 让PC11输出低电平		0
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);		// 让PD0输出低电平		0
	
	//	右
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// 让输出高电平		0
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);		// 让输出高电平		0
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);		// 让输出低电平		0
}

int SanxianControl(void){
		while((strstr(USART1_RX_BUF, "sxxl") != NULL)||(strstr(USART3_RX_BUF, "sxxl") != NULL)){
		
		if((PGin(4) == 0 && PGin(6) == 0 && PGin(8) == 1)&&(PGin(2) == 1 && PCin(7) ==1))
		{
			GO();
			Anquan();
		}
		else if(((PGin(4) == 1 && PGin(6) == 0) && (PGin(8) == 0 || 
			PGin(8) ==1)) && (PGin(2) == 1 && PCin(7) == 1))
		{
			RIGHT();
			Noanquan();
		}
		else if(((PGin(4) == 0 && PGin(6) == 1 )&&( PGin(8) == 0 || 
			PGin(8) ==1)) && (PGin(2) == 1 && PCin(7) == 1))
		{
			LEFT();
			Noanquan();
		}
		else if(((PGin(4) == 1 && PGin(6) == 1 && PGin(8) == 1) ||
		(PGin(4) == 0 && PGin(6) == 0 && PGin(8) == 0)) &&
		((PGin(2) == 0 || PGin(2) == 1)||(PCin(7) == 0 || PCin(7) == 1)))
		{
			BIGLEFT();
			Minianquan();
		}	
		else{
			STOP();
			Minianquan();
		}

		}
		return 0;
}


int HongwaiControl(void){
	while((strstr(USART1_RX_BUF, "hwbz") != NULL)||(strstr(USART3_RX_BUF, "hwbz") != NULL)){
		if(PGin(2) == 1 && PCin(7) == 1){
		GO();
		Anquan();
		}
		else if(PGin(2)==0&&PCin(7)== 1){
		BACK();
		BIGRIGHT();
		delay_ms(700);
		Noanquan();
		}
		else if(PGin(2)==1&&PCin(7) == 0){
		BACK();
		BIGLEFT();
		delay_ms(700);
		Noanquan();
		}
		else{
		STOP();
		BACK();
		delay_ms(1000);
		Noanquan();
		}
	}
}