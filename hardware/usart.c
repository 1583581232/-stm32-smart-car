#include "usart.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "motor.h"
#include "sys.h"

char USART1_RX_BUF[128]={0};     	        // 接收缓冲,最大USART_REC_LEN个字节.
u16 USART1_RX_STA=0;       					// 接收状态标记	
u16 USART1_RX_num=0;						// 接收计数偏移

char USART3_RX_BUF[128]={0};     	        // 接收缓冲,最大USART_REC_LEN个字节.
u16 USART3_RX_STA=0;       					// 接收状态标记	
u16 USART3_RX_num=0;						// 接收计数偏移



int fputc(int ch, FILE *f){      
	while((USART1->SR&0X40)==0);            //循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
int USART1_SendBytes(uint8_t *pbuf,uint32_t len){

	while(len--)
	{
		USART_SendData(USART1,*pbuf++);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET); 
	}

}

int USART1_SendString(uint8_t *pstr){
	while(pstr && *pstr)
	{
		USART_SendData(USART1,*pstr++);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET); 
	}
}
int USART3_SendBytes(uint8_t *pbuf,uint32_t len){

	while(len--)
	{
		USART_SendData(USART3,*pbuf++);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET); 	
	}

}

int USART3_SendString(uint8_t *pstr){
	while(pstr && *pstr)
	{
		USART_SendData(USART3,*pstr++);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET); 	
	}
}

int USART1_Init(u32 bound){
 
	GPIO_InitTypeDef 	GPIO_InitStructure;			// 引脚配置结构体变量
	USART_InitTypeDef 	USART_InitStructure;		// 串口配置结构体变量
	NVIC_InitTypeDef 	NVIC_InitStructure;			// 中断配置结构体变量
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	 // 时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	  //打开时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//USART1 Tx (PA.09)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//USART1 Rx (PA.10)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = bound;						// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		// 数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			// 停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;			// 效验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		// 流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// 中断模式
	USART_Init(USART1, &USART_InitStructure); 						// 初始化
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					// 允许中断
	USART_Cmd(USART1, ENABLE);										// 使能串口
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				// 串口 1  中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		// 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				// 响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					// 中断使能
	NVIC_Init(&NVIC_InitStructure);									// 初始化中断向量表
}

int USART3_Init(u32 bound){
    GPIO_InitTypeDef 	GPIO_InitStructure;			// 引脚配置结构体变量
	USART_InitTypeDef 	USART_InitStructure;		// 串口配置结构体变量
	NVIC_InitTypeDef 	NVIC_InitStructure;			// 中断配置结构体变量
    
    //2、串口所用的GPIO时钟使能，串口时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    
    //3、设置I/O引脚功能为复用推挽输出、浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;     //USART3 Tx (PB10)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;           //USART3 Rx (PB11)
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //4、设置串口的波特率，设置数据格式：数据位、停止位、校验位
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    
    //6、利用串口使能函数USART_Cmd()使能相应串口
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
    
    //7.1、配置中断
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

