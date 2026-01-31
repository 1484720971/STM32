#include "tim5.h"
#include "tim4.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"

/*
    这是一个STM32F103ZE微控制器的PWM周期测量项目，
    主要功能是使用定时器4的输入捕获功能来测量PWM信号的周期和频率。

    信号流：TIM5生成PWM → TIM4测量PWM → 串口输出结果
    1. PWM信号生成：TIM5产生100Hz的PWM方波信号
    2. PWM周期测量：TIM4通过输入捕获测量PWM信号的周期
    3. 实时显示：通过USART串口打印测量结果

    工作原理详解
    1. TIM5 - PWM信号生成器
    // 配置100Hz PWM信号
    TIM5->PSC = 7200 - 1;    // 预分频：72MHz/7200 = 10kHz
    TIM5->ARR = 100 - 1;     // 自动重装：10kHz/100 = 100Hz
    TIM5->CCR2 = duty;       // 占空比控制
    频率计算: 72MHz ÷ 7200 ÷ 100 = 100Hz
    一个方波的周期：1s / 100Hz = 10ms
    2. TIM4 - PWM周期测量器
    // 输入捕获配置
    TIM4->PSC = 72 - 1;      // 预分频：72MHz/72 = 1MHz (1μs精度)
    TIM4->ARR = 65535 - 1;   // 最大计数值
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;  // CH1输入捕获模式

    测量原理:
    - 设置为上升沿触发
    - 每次捕获到上升沿时，CCR1记录当前计数值
    - 中断中清零CNT计数器
    - 下次上升沿时，CCR1的值就是一个PWM周期的时长
*/

int main(void)
{
    USART1_Init();                  // 串口初始化
    TIM5_PWMLedBreathInit();        // TIM5产生100Hz PWM
    TIM5_SetDuty(10);               // 设置10%占空比

    TIM4_CH1MeasurePWMCycleInit();   // TIM4初始化输入捕获

    while (1)
    {
        printf("周期:%.2fms | 频率:%.2fHz\n", TIM4_GetPWMCycleMS(), TIM4_GetPWMFreq());
        Delay_s(1); // 每秒输出一次
    }
}
