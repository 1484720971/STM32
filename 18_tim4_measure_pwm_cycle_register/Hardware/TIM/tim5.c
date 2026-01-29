#include "tim5.h"

/**
 * @brief TIM5定时器生成PWM方波LED呼吸灯
 *
 * 让LED以100Hz的频率来闪烁
 *
 */
void TIM5_PWMLedBreathInit(void)
{
    // 1. 时钟使能
    // 1.1 使能GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // 1.2 使能TIM5时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    
    // 2. GPIO初始化
    // 2.1 配置PA1工作模式为复用推挽输出（现在想让LED的高低电平由占空比来控制，所以使用复用推挽输出）
    GPIOA->CRL |= GPIO_CRL_MODE1;
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    // 3. TIM5初始化
    // 3.1 控制器初始化
    // 这部分保持复位值。默认使用72MHz时钟频率来计数

    // 3.2 时基单元初始化
    // 3.2.1 配置PSC预分频器
    TIM5->PSC = 7200 - 1;
    // 3.2.2 配置自动重装载寄存器
    TIM5->ARR = 100 - 1;
    // 3.2.3 配置计数方向
    TIM5->CR1 &= ~TIM_CR1_DIR;

    // 3.3 输出部分初始化
    // 3.3.1 配置CCR2通道为输出
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;
    // 3.3.2 配置输出比较2模式为PWM模式1 - CNT比CCR小，输出有效。反之输出无效
    TIM5->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M_0;
    // 3.3.3 配置CCR2捕获2输出极性 - 低电平有效 CCR里的值 表示低电平占空比 低电平->灯亮
    //                              低电平 占空比越高（CCR变大）灯越亮
    TIM5->CCER |= TIM_CCER_CC2P;
    // 3.3.4 使能CCR2通道
    TIM5->CCER |= TIM_CCER_CC2E;
    // 3.3.5 配置CCR2初始值
    // 初始值配置为0 表示全是高电平 灯不亮
    TIM5->CCR2 = 0;

    // 4. 计数器使能
    TIM5->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 设置占空比
 *
 */
void TIM5_SetDuty(uint8_t duty)
{
    TIM5->CCR2 = duty;
}

