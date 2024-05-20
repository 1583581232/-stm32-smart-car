#include "delay.h"	// �����Ӧ���Զ��� .h ͷ�ļ�

/* 	
	��ֲ�δ�ʱ�� ��Ҫ ���� 9 - 47 ����   
	�δ�ʱ��Ϊϵͳʱ��
	��ʹ��ʱ ��Ҫ�� �δ�ʱ�����г�ʼ��    
		ʹ��   
			SysTick_Init(); // ��ʼ��
			
	�ڳ����оͿ���ʹ��
		delay_us (u32 nus);   // ΢����ʱ����
		delay_ms (u16 nms);   // ������ʱ����
*/
static u8  fac_us=0;	// ��ʱ�ο�ȫ�ֱ���
static u16 fac_ms=0;	// ��ʱ�ο�ȫ�ֱ���


void SysTick_Init (void)
{		// �δ�ʱ����ʼ��
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
    fac_us = SystemCoreClock/8000000;  
	// fac_us = 72/8;
	fac_ms=(u16)fac_us*1000;				   
}
 
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 
//
