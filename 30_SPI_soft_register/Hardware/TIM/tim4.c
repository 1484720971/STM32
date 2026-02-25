#include "tim4.h"

/**
 * @brief 测量PWM方波周期
 *
 */
void TIM4_CH1MeasurePWMCycleInit(void)
{
    // 1. 使能时钟
    // 1.1 使能TIM4时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    // 1.2 使能GPIOB时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. GPIO初始化
    // 2.1 配置PB6浮空输入（复位后本来就是浮空输入，可以不用配置）
    // 因为要用PB6的TIM4_CH1通道来捕获PWM信号，所以应该配置为浮空输入
    GPIOB->CRL &= ~GPIO_CRL_MODE6;
    GPIOB->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOB->CRL |= GPIO_CRL_CNF6_0;

    // 3. TIM4初始化
    // 3.1 控制器初始化
    // 这部分保持复位值。默认使用72MHz时钟频率来计数

    // 3.2 时基单元初始化
    // 3.2.1 配置预分频器
    TIM4->PSC = 72 - 1;
    // 3.2.2 配置自动重装载寄存器
    TIM4->ARR = 65535 - 1;
    // 3.2.3 配置计数器方向
    TIM4->CR1 &= ~TIM_CR1_DIR;

    // 3.3 输入部分初始化
    // 3.3.1 配置TI1选择，TIM4_CH1引脚连到TI1输入
    TIM4->CR2 &= ~TIM_CR2_TI1S;
    // 3.3.2 配置CC1通道为输入，IC1映射在TI1上
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
    // 3.3.3 配置输入捕获1滤波器,为滤波器
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
    // 3.3.4 配置输入/捕获1预分频器，为无预分频器
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    // 3.3.5 配置输入/捕获1输出极性,OC1低电平有效
    TIM4->CCER &= ~TIM_CCER_CC1P;
    // 3.3.6 配置输入/捕获1输出使能
    TIM4->CCER |= TIM_CCER_CC1E;
    // 3.3.7 配置允许捕获/比较1中断
    TIM4->DIER |= TIM_DIER_CC1IE;

    // 4. NVIC配置
    // 4.1 配置设置抢占优先级还是响应优先级
    NVIC_SetPriorityGrouping(3);    // 全部是抢占优先级
    // 4.2 配置中断优先级
    NVIC_SetPriority(TIM4_IRQn, 3); // 这里的优先级是随便配的
    // 4.3 使能NVIC中断
    NVIC_EnableIRQ(TIM4_IRQn);

    // 5. 计数器使能
    TIM4->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 读取TIM4的CCR1
 *
 * @return uint16_t
 */
uint16_t TIM4_ReadCCR1(void)
{
    return TIM4->CCR1;
}

/**
 * @brief 获取PWM周期
 *
 */
double TIM4_GetPWMCycleMS(void)
{
    return (TIM4->CCR1 + 1.0) / 1000;
}

/**
 * @brief 计数频率
 *
 */
double TIM4_GetPWMFreq(void)
{
    return 1000000.0 / (TIM4->CCR1 + 1);
}

void TIM4_IRQHandler(void)
{
    // 1. 清除标志位（防止一直进中断）
    TIM4->SR &= ~TIM_SR_CC1IF;
    // 2. CNT清零
    TIM4->CNT = 0;
}

/**
 * @brief 测量PWM方波占空比和周期
 *
 */
void TIM4_CH1CH2MeasurePWMDutyAndCycleInit(void)
{
    // 1. 使能时钟
// 1.1 使能TIM4时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    // 1.2 使能GPIOB时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. GPIO初始化
    // 2.1 配置PB6浮空输入（复位后本来就是浮空输入，可以不用配置）
    // 因为要用PB6的TIM4_CH1通道来捕获PWM信号，所以应该配置为浮空输入
    GPIOB->CRL &= ~GPIO_CRL_MODE6;
    GPIOB->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOB->CRL |= GPIO_CRL_CNF6_0;

    // 3. TIM4初始化
    // 3.1 控制器初始化 控制器用通道1的上升沿 做 定时器的复位
    // 3.1.1 配置从模式控制寄存器 - 配置为滤波后的定时器输入1（TI1FP1）
    TIM4->SMCR |= TIM_SMCR_TS_2;
    TIM4->SMCR &= ~TIM_SMCR_TS_1;
    TIM4->SMCR |= TIM_SMCR_TS_0;
    // 3.1.2 配置从模式选择 - 配置为复位模式，选中的触发输入（TRGI）的上升沿重新初始化计数器，并且产生一个更新寄存器的信号
    TIM4->SMCR |= TIM_SMCR_SMS_2;
    TIM4->SMCR &= ~TIM_SMCR_SMS_1;
    TIM4->SMCR &= ~TIM_SMCR_SMS_0;

    // 3.2 时基单元初始化
    // 3.2.1 配置预分频器
    TIM4->PSC = 72 - 1;
    // 3.2.2 配置自动重装载寄存器
    TIM4->ARR = 65535 - 1;
    // 3.2.3 配置计数器方向
    TIM4->CR1 &= ~TIM_CR1_DIR;

    // 3.3 输入部分初始化（通道1）
    // 3.3.1 配置TI1选择，TIM4_CH1引脚连到TI1输入
    TIM4->CR2 &= ~TIM_CR2_TI1S;
    // 3.3.2 配置CC1通道为输入，IC1映射在TI1上
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
    // 3.3.3 配置输入捕获1滤波器,为无滤波器
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
    // 3.3.4 配置输入/捕获1预分频器，为无预分频器
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    // 3.3.5 配置输入/捕获1输出极性，不反相：捕获发生在IC1的上升沿
    TIM4->CCER &= ~TIM_CCER_CC1P;
    // 3.3.6 配置输入/捕获1输出使能
    TIM4->CCER |= TIM_CCER_CC1E;

    // 3.4 输入部分初始化（通道2）
    // 3.4.1 配置CC2通道为输入，IC2映射在TI1上
    TIM4->CCMR1 |= TIM_CCMR1_CC2S_1;
    TIM4->CCMR1 &= ~TIM_CCMR1_CC2S_0;
    // 3.4.2 配置输入捕获2滤波器，为无滤波器
    TIM4->CCMR1 &= ~TIM_CCMR1_IC2F;
    // 3.4.3 配置输入/捕获2预分频器，为无预分频器
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    // 3.4.5 配置输入/捕获2输出极性，反相：捕获发生在IC2的下降沿
    TIM4->CCER |=  TIM_CCER_CC2P;
    // 3.4.6 配置输入/捕获2输出使能
    TIM4->CCER |= TIM_CCER_CC2E;

    // 4. 计数器使能
    TIM4->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 获取占空比
 * 
 * @return double 
 */
double TIM4_GetDuty(void)
{
    return ((TIM4->CCR2 + 1.0) / (TIM4->CCR1 + 1.0)) * 100.0;
}