#include "tim3.h"

/**
 * @brief 定时器3初始化
 *
 * 配置定时器3的通道4
 */
void TIM3_PWMInit(void)
{
    // 1. 使能时钟
    // 1.1 使能GPIOB时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // 1.2 使能TIM3时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // 2. GPIOB初始化
    // 2.1 配置PB1的工作模式为服用推挽输出
    GPIOB->CRL |= GPIO_CRL_MODE1;
    GPIOB->CRL |= GPIO_CRL_CNF1_1;
    GPIOB->CRL &= ~GPIO_CRL_CNF1_0;

    // 3. TIM3初始化
    // 3.1 控制器初始化
    // 这部分保持复位值。默认使用72MHz时钟频率来计数

    // 3.2 时基单元初始化
    // 3.2.1 配置PSC预分频器
    TIM3->PSC = 7200 - 1;
    // 3.2.2 配置自动重装载值
    TIM3->ARR = 100 - 1;
    // 3.2.3 配置计数方向
    TIM3->CR1 &= ~TIM_CR1_DIR;

    // 3.3 输出部分初始化
    // 3.3.1 配置CCR4通道为输出
    TIM3->CCMR2 &= TIM_CCMR2_CC4S;
    // 3.3.2 配置输出比较4模式为PWM模式1 - CNT比CCR小，输出有效。反之输出无效
    TIM3->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1);
    TIM3->CCMR2 &= ~TIM_CCMR2_OC4M_0;
    // 3.3.3 配置CCR4输出极性 - 低电平有效
    TIM3->CCER &= ~TIM_CCER_CC4E;
    // 3.3.4 使能CCR4通道
    TIM3->CCER |= TIM_CCER_CC4E;
    // 3.3.5 配置CCR4初始值
    TIM3->CCR4 = 0;

    // 4. 计数器使能
    TIM3->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 设置占空比
 * 
 */
void TIM3_SetDuty(uint8_t duty)
{
    TIM3->CCR4 = duty;
}