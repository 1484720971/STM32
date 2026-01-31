#include "tim6.h"

/**
 * @brief 基本定时器6初始化
 *
 */
void TIM6_Init(void)
{
    // 1. 使能TIM6时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    // 2. TIM6配置
    // 2.1 配置预分频器 
    // 主频为72000000Hz / 7200 = 10000Hz -> 10KHz -> 1/10ms记一个数 = 100us记一个数
    TIM6->PSC = 7200 - 1;   // 真实是7200分频
    // 2.2 配置自动重装载寄存器
    // 记一个数的时间为100us，要数要1s的话，就需要数10000个数
    TIM6->ARR = 10000 - 1;

    // 2.3 手动产生一次更新
    TIM6->EGR |= TIM_EGR_UG;
    // 2.4 等待更新真的发送完
    while (1)
    {
        if (TIM6->SR & TIM_SR_UIF)
        {
            printf("软件更新发生了\n");
            TIM6->SR &= ~TIM_SR_UIF;
            break;
        }
    }

    // 2.5 使能更新中断
    TIM6->DIER |= TIM_DIER_UIE;

    // 2.6 使能计数器
    TIM6->CR1 |= TIM_CR1_CEN;

    // 3. NVIC配置
    // 3.1 设置抢占优先级还是响应优先级
    NVIC_SetPriorityGrouping(3);    // 全是抢占优先级
    // 3.2 设置中断优先级
    NVIC_SetPriority(TIM6_IRQn, 8); // 设置USARAT中断的优先级为8(没人跟我们抢，这里优先级随便设置)
    // 3.3 使能NVIC中断
    NVIC_EnableIRQ(TIM6_IRQn);
}

void TIM6_IRQHandler(void)  // 1s触发一次
{
    printf("TIM6 Interrupt\t");

    // 片上外设必须清除中断标志位,内核可以不用清除中断标志位
    TIM6->SR &= ~TIM_SR_UIF;

    LED_Toggle(LED3);
}