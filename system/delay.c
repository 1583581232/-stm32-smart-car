#include "delay.h"	// 导入对应的自定义 .h 头文件

/* 	
	移植滴答定时器 需要 复制 9 - 47 代码   
	滴答定时器为系统时钟
	在使用时 需要对 滴答定时器进行初始化    
		使用   
			SysTick_Init(); // 初始化
			
	在程序中就可以使用
		delay_us (u32 nus);   // 微妙延时函数
		delay_ms (u16 nms);   // 毫妙延时函数
*/
static u8  fac_us=0;	// 延时参考全局变量
static u16 fac_ms=0;	// 延时参考全局变量


void SysTick_Init (void)
{		// 滴答定时器初始化
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
    fac_us = SystemCoreClock/8000000;  
	// fac_us = 72/8;
	fac_ms=(u16)fac_us*1000;				   
}
 
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 
//
