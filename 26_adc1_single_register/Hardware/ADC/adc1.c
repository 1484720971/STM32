#include "adc1.h"

/**
 * @brief ADC1单通道初始化
 *
 */
void ADC1_SingleChannelInit(void)
{
    // 1. 使能时钟
    // 1.1 使能GPIOC时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // 1.2 使能ADC1时钟
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // 2. GPIO初始化
    // 2.1 配置PC0为模拟输入
    GPIOC->CRL &= ~GPIO_CRL_MODE0;
    GPIOC->CRL &= ~GPIO_CRL_CNF0;

    // 3. ADC1初始化
    // 3.1 配置ADC1时钟，因为ADC的输入时钟不得超过14MHz，所以要在RCC寄存器中对ADC的时钟进行分频，6分频后为12MHz
    RCC->CR |= RCC_CFGR_ADCPRE_1;
    RCC->CR &= ~RCC_CFGR_ADCPRE_0;
    // 3.2 配置扫描模式，因为当前只有一个通道，所以关闭扫描模式。多通道时使用扫描模式
    ADC1->CR1 &= ADC_CR1_SCAN;
    // 3.3 配置数据对齐，右对齐。ADC 精度12位 数据寄存器16位 默认是右对齐（不用配）
    ADC1->CR2 &= ~ADC_CR2_ALIGN;
    // 3.4 配置连续转换，不停的扫描。结合之前的不扫描意思是单曲循环
    ADC1->CR2 |= ADC_CR2_CONT;
    // 3.5 配置ADC规则序列（歌单配置），PC0口为ADC的10通道，所以这里应该写10
    ADC1->SQR3 |= 10;
    // 3.6 配置规则通道序列长度，当前只有一个通道，所以配1
    ADC1->SQR1 &= ~ADC_SQR1_L;
    // 3.7 配置采样时间，7.5周期，（周期根据12M主频算）
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10_2;
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10_1;
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_0;
    // 3.8 开启A/D转换器。当该位为'0'时（第一次写1），写入'1将把ADC从断电模式下唤醒。
    ADC1->CR2 |= ADC_CR2_ADON;
    // 3.9 开启A/D校准
    ADC1->CR2 |= ADC_CR2_CAL;
    // 3.10 等待校准完成
    uint16_t timeout = UINT16_MAX;
    while ((ADC1->CR2 & ADC_CR2_CAL) != 0 && timeout)
    {
        timeout--;
    }
    // 3.11 开启A/D转换器，启动以开始测量。当该位为'1时，写入1将启动转换。
    ADC1->CR2 |= ADC_CR2_ADON;
}

/**
 * @brief ADC1单通道读电压
 * 
 * @return double 电压值
 */
double ADC1_SingleReadV(void)
{
    // 12位的ADC范围[0，4095]
    return ADC1->DR * 3.3 / 4095;   // (ADC1->DR) * 3.3 / (2 ^ n - 1)   n = 12
}