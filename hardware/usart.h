#ifndef __SR04_H
#define __SR04_H

#include "stm32f10x.h"
#include "stdio.h"

int fputc(int ch, FILE *f);
int USART1_SendBytes(uint8_t *pbuf,uint32_t len);
int USART1_SendString(uint8_t *pstr);
int USART3_SendBytes(uint8_t *pbuf,uint32_t len);
int USART3_SendString(uint8_t *pstr);

int USART1_Init(u32 bound);
int USART3_Init(u32 bound);


#endif
