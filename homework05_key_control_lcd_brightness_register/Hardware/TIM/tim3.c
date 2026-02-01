#include "tim3.h"

/**
 * @brief TIM3初始化
 *
 */
void TIM3_PWMLcd_Init(void)
{
    // 1. 使能时钟
    // 1.1 使能GPIOB时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // 1.2 使能TIM3时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // 2. 初始化PB0
    // 2.1 配置PB0为复用推挽输出
    GPIOB->CRL |= GPIO_CRL_MODE0;
    GPIOB->CRL |= GPIO_CRL_CNF0_1;
    GPIOB->CRL &= ~GPIO_CRL_CNF0_0;

    // 3. 初始化TIM3
    // 3.1 控制器初始化
    // 这部分保持复位值。默认使用72MHz时钟频率来计数

    // 3.2 时基单元初始化
    // 3.2.1 配置PSC预分频器
    TIM3->PSC = 7200 - 1;
    // 3.2.2 配置自动重装载寄存器
    TIM3->ARR = 10 - 1;
    // 3.2.3 配置计数方向
    TIM3->CR1 &= TIM_CR1_DIR;

    // 3.3 输出部分初始化
    // 3.3.1 配置CCR3通道为输出
    TIM3->CCMR2 &= ~TIM_CCMR2_CC3S;
    // 3.3.2 配置输出比较3模式为PWM模式1
    TIM3->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
    TIM3->CCMR2 &= ~TIM_CCMR2_OC3M_0;
    // 3.3.3 配置CCR3捕获3输出极性 - 低电平有效
    TIM5->CCER |= TIM_CCER_CC3P;
    // 3.3.4 使能CCR3通道
    TIM3->CCER |= TIM_CCER_CC3E;
    // 3.3.5 配置CCR3初始值
    TIM3->CCR3 = 0;

    // 4. 计数器使能
    TIM3->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 设置占空比
 * 
 * @param duty 
 */
void TIM3_SetDuty(uint8_t duty)
{
    TIM3->CCR3 = duty;
}