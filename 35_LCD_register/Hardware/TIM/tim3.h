#ifndef __TIM3_H__
#define __TIM3_H__

#include "stm32f10x.h"

/**
 * @brief 定时器3初始化
 * 
 * 配置定时器3的通道4，50%占空比
 */
void TIM3_PWMInit(void);

/**
 * @brief TIM3 PWM 控制LCD 初始化
 * 
 */
void TIM3_PWMLcd_Init(void);

/**
 * @brief 设置占空比
 * 
 */
void TIM3_SetDuty(uint8_t duty);

#endif /* __TIM3_H__ */