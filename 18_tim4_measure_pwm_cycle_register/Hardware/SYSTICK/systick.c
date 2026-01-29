#include "systick.h"
#include "stdio.h"

/**
 * @brief Systick系统定时器初始化
 *
 */
void Systick_Init(void)
{
    // 1. SysTick配置 - 方法一 - 自己手动配置寄存器
    // // 1.1 配置时钟源,将Systick配置为72M的时钟频率
    // SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    // // 1.2 使能SysTick中断
    // SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    // // 1.3 配置重装载值  100ms为周期,触发一次Systick中断
    // // 根据手册 如果是多个中断连着用 reload的值应该是n-1
    // SysTick->LOAD = 72000 - 1;  // 1ms 一次中断
    // // 1.4 使能Systick
    // SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    // 1. SysTick配置 - 方法二 - 调用内核提供的函数
    SysTick_Config(72000);  // 调用这个函数的话NVIC也没必要配置，函数中已经配置过了

    // 2. NVIC配置
    // 2.1 优先级分组
    // NVIC_SetPriorityGrouping(3);
    // // 2.2 具体优先级设定
    // NVIC_SetPriority(SysTick_IRQn, 4);
    // // 2.3 NVIC使能  NVIC_EnableIRQ这个函数只能用来使能片上外设，只能传入正数，所以这里不用使能SysTick的NVIC
    // NVIC_EnableIRQ(SysTick_IRQn);
}

// 每隔1秒 让LED1亮1秒
// 内核中断是硬件清除标志位的
uint16_t count = 0;
void SysTick_Handler(void)
{
    count++;
    if (count == 1000)
    {
        printf("SysTick Interrupt\t");
        LED_Toggle(LED2);
        count = 0;
    }
}