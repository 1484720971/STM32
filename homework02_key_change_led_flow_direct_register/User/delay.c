#include <delay.h>

void Delay_us(uint16_t us)
{
    // 1. 已经知道1us 72  计数器每计数一次 1/72 us   1us -> 72次
    SysTick->LOAD = 72 * us;
    // 2. 把时钟源设置为72M
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    // 3. 中断不开
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    // 4. 开启计数器使能，表示开始计数
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    // 数到0之后会将countflag置1
    // 5. 循环判断countflag是否为1，如果为1就说明计数到0
    while (1)
    {
        if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG)
        {
            break;
        }
    }

    // 6. 关闭计数器使能
    // SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Delay_ms(uint16_t ms)
{
    while (ms--)
    {
        Delay_us(1000);
    }
}

void Delay_s(uint16_t s)
{
    while (s--)
    {
        Delay_ms(1000);
    }
}