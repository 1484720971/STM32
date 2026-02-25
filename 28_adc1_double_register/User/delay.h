#ifndef __DELAY_H_
#define __DELAY_H_

#include "stm32f10x.h"

// 使用定时器提更加精确的延时
void Delay_us(uint16_t us);

void Delay_ms(uint16_t ms);

void Delay_s(uint16_t s);

#endif