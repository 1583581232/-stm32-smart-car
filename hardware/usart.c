#include "usart.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "motor.h"
#include "sys.h"

char USART1_RX_BUF[128]={0};     	        // ���ջ���,���USART_REC_LEN���ֽ�.
u16 USART1_RX_STA=0;       					// ����״̬���	
u16 USART1_RX_num=0;						// ���ռ���ƫ��

char USART3_RX_BUF[128]={0};     	        // ���ջ���,���USART_REC_LEN���ֽ�.
u16 USART3_RX_STA=0;       					// ����״̬���	
u16 USART3_RX_num=0;						// ���ռ���ƫ��



int fputc(int ch, FILE *f){      
	while((USART1->SR&0X40)==0);            //ѭ������,ֱ���������   
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
 
	GPIO_InitTypeDef 	GPIO_InitStructure;			// �������ýṹ�����
	USART_InitTypeDef 	USART_InitStructure;		// �������ýṹ�����
	NVIC_InitTypeDef 	NVIC_InitStructure;			// �ж����ýṹ�����
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	 // ʱ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	  //��ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//USART1 Tx (PA.09)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//USART1 Rx (PA.10)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = bound;						// ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		// ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			// ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;			// Ч��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		// ����
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// �ж�ģʽ
	USART_Init(USART1, &USART_InitStructure); 						// ��ʼ��
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					// �����ж�
	USART_Cmd(USART1, ENABLE);										// ʹ�ܴ���
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				// ���� 1  �ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		// ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				// ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					// �ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);									// ��ʼ���ж�������
}

int USART3_Init(u32 bound){
    GPIO_InitTypeDef 	GPIO_InitStructure;			// �������ýṹ�����
	USART_InitTypeDef 	USART_InitStructure;		// �������ýṹ�����
	NVIC_InitTypeDef 	NVIC_InitStructure;			// �ж����ýṹ�����
    
    //2���������õ�GPIOʱ��ʹ�ܣ�����ʱ��ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    
    //3������I/O���Ź���Ϊ���������������������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;     //USART3 Tx (PB10)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;           //USART3 Rx (PB11)
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING; //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //4�����ô��ڵĲ����ʣ��������ݸ�ʽ������λ��ֹͣλ��У��λ
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    
    //6�����ô���ʹ�ܺ���USART_Cmd()ʹ����Ӧ����
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
    
    //7.1�������ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

