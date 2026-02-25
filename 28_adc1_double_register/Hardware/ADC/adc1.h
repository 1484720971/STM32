#ifndef __ADC1_H__
#define __ADC1_H__

#include "stm32f10x.h"
#include "delay.h"
#include "stdio.h"

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

/**
 * @brief ADC1双通道初始化
 *
 */
void ADC1_DoubleChannelDMAInit(void);

/**
 * @brief ADC1双通道开启测量
 * 
 */
void ADC1_DoubleChannelStart(uint16_t *pData, uint16_t len);

#endif /* __ADC1_H__ */