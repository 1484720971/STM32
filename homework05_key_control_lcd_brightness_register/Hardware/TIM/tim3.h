#ifndef __TIM3_H__
#define __TIM3_H__

#include "stm32f10x.h"

/**
 * @brief TIM3初始化
 * 
 */
void TIM3_PWMLcd_Init(void);

/**
 * @brief 设置占空比
 * 
 * @param duty 
 */
void TIM3_SetDuty(uint8_t duty);

#endif /* __TIM3_H__ */