#ifndef __DELAY_H		
/* �ж�  __LED_H  ��û�ж��� 
	��� 
		�ж��� 	��ִ��ifndef-endif�м�Ĵ���
		δ����	ִ��ifndef-endif�м�Ĵ���
	ע������
		ͬһ�������У� �����жϵĺ궨�岻���ظ�
		��֤һ�������궨�������һ��
			#ifndef __DELAY_H	
			#define __DELAY_H
*/
#define __DELAY_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


//  ��������
void SysTick_Init (void);		// ��ʼ�� �δ�ʱ�� ��ʱ����
void delay_us (u32 nus);	// ��ʱ����  ��ʱ��λ ΢��
void delay_ms (u16 nms);	// ��ʱ����  ��ʱ��λ ����
#endif
