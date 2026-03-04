#include "tim1.h"

/**
 * @brief 输出5个PWM方波
 *
 */
void TIM1_5PWMInit(void)
{
    // 1. 时钟使能
    // 1.1 使能GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // 1.2 使能TIM8时钟
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    // 2. GPIO初始化
    // 2.1 配置PA8为复用推挽输出
    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->CRH |= GPIO_CRH_CNF8_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0;

    // 3. TIM1初始化
    // 3.1 控制器初始化
    // 这部分保持复位值。默认使用72MHz时钟频率来计数

    // 3.2 时基单元初始化
    // 3.2.1 配置PSC预分频器
    // 2Hz 一个周期500ms 7200分频数一个数的时间是100us 数5000次就是500ms
    TIM1->PSC = 7200 - 1;
    // 3.2.2 配置自动重装载寄存器
    TIM1->ARR = 5000 - 1;
    // 3.2.3 配置重复计数器，溢出5次
    TIM1->RCR = 5 - 1;
    // 3.2.4 配置计数方向
    TIM1->CR1 &= ~TIM_CR1_DIR;

    // 3.3 输出部分初始化
    // 3.3.1 配置CCR1通道为输出
    TIM1->CCMR1 &= ~TIM_CCMR1_CC2S;
    // 3.3.2 配置输出比较1模式为PWM模式1
    TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    // 3.3.3 配置CCR1捕获1输出极性 - 高电平有效
    TIM1->CCER &= ~TIM_CCER_CC1P;
    // 3.3.4 使能CCR1通道
    TIM1->CCER |= TIM_CCER_CC1E;
    // 3.3.5 配置CCR1初始值 - 初始值配置为50%占空比
    TIM1->CCR1 = 2500;

    // 4. 使能主输出 - 高级定时器特有
    TIM1->BDTR |= TIM_BDTR_MOE;

    // 5. 手动产生一个更新事件
    TIM1->EGR |= TIM_EGR_UG;

    uint16_t timeout = UINT16_MAX;
    while ((TIM1->SR & TIM_SR_UIF == 0) && timeout--)
    {
        TIM1->SR &= ~TIM_SR_BIF;
        break;
    }

    // 6. 配置为单脉冲模式
    TIM1->CR1 |= TIM_CR1_OPM;

    // 7. 使能计数器
    TIM1->CR1 |= TIM_CR1_CEN;
}