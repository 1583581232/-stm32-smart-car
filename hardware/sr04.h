#ifndef __SR04_H
#define __SR04_H

#include "stm32f10x.h"
#include "stdio.h"

int sr04_init(void);
uint32_t sr04_get_distance(void);

int Moshushou(void);
int Chaoshengbo(void);
int Ruku(void);

#endif
