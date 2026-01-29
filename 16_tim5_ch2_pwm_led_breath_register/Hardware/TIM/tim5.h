#ifndef __TIM5_H__
#define __TIM5_H__

#include "stm32f10x.h"

/**
 * @brief TIM5定时器生成PWM方波LED呼吸灯
 *
 */
void TIM5_PWMLedBreathInit(void);

/**
 * @brief 设置占空比
 *
 */
void TIM5_SetDuty(uint8_t duty);

#endif /* __TIM5_H__ */