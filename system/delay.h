#ifndef __DELAY_H		
/* 判断  __LED_H  有没有定义 
	如果 
		有定义 	不执行ifndef-endif中间的代码
		未定义	执行ifndef-endif中间的代码
	注意事项
		同一个工程中， 这里判断的宏定义不能重复
		保证一下两个宏定义的内容一样
			#ifndef __DELAY_H	
			#define __DELAY_H
*/
#define __DELAY_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


//  函数声明
void SysTick_Init (void);		// 初始化 滴答定时器 延时函数
void delay_us (u32 nus);	// 延时函数  延时单位 微妙
void delay_ms (u16 nms);	// 延时函数  延时单位 毫妙
#endif
