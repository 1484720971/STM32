#include "tim5.h"
#include "tim4.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"

int main(void)
{
    // USART初始化
    USART1_Init();

    // TIM5初始化 - 生成PWM方波
    TIM5_PWMLedBreathInit();

    // TIM4初始化 - 测量PWM方波占空比和周期
    TIM4_CH1CH2MeasurePWMDutyAndCycleInit();

    // 设置占空比
    TIM5_SetDuty(20);

    while (1)
    {
        printf("周期：%.2fms | 频率：%.2fHz | 占空比：%.2f%%", TIM4_GetPWMCycleMS(), TIM4_GetPWMFreq(), TIM4_GetDuty());
        Delay_s(1);
    }
}
