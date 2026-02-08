#ifndef __ADC1_H__
#define __ADC1_H__

#include "stm32f10x.h"

/**
 * @brief ADC1单通道初始化
 *
 */
void ADC1_SingleChannelInit(void);

/**
 * @brief ADC1单通道读电压
 * 
 * @return double 电压值
 */
double ADC1_SingleReadV(void);

#endif /* __ADC1_H__ */