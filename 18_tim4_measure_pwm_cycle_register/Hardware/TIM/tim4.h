#ifndef __TIM4_H__
#define __TIM4_H__

#include "stm32f10x.h"

/**
 * @brief 测量PWM方波周期
 * 
 */
void TIM4_CH1MeasurePWMCycleInit(void);

/**
 * @brief 读取TIM4的CCR1
 * 
 * @return uint16_t 
 */
uint16_t TIM4_ReadCCR1(void);

/**
 * @brief 获取PWM周期
 * 
 */
double TIM4_GetPWMCycleMS(void);

/**
 * @brief 计数频率
 * 
 */
double TIM4_GetPWMFreq(void);

#endif /* __TIM4_H__ */