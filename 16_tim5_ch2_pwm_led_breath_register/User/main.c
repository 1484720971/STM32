#include "tim5.h"
#include "delay.h"

int main(void)
{
    // TIM5初始化
    TIM5_PWMLedBreathInit();

    // 设置占空比
    while (1)
    {
        for (uint8_t i = 0; i <= 100; i++)
        {
            TIM5_SetDuty(i);
            Delay_ms(10);
        }
        
        for (uint8_t i = 0; i <= 100; i++)
        {
            TIM5_SetDuty(100 - i);
            Delay_ms(10);
        }
    }
}
