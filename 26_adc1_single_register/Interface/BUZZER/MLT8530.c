#include "MLT8530.h"

/**
 * @brief 蜂鸣器初始化
 * 
 */
void MLT8530_Init(void)
{
    // TIM3 初始化
    TIM3_PWMInit();

    // 设置50%占空比
    TIM3_SetDuty(10);
}