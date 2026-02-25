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
    ADC1->CR1 &= ~ADC_CR1_SCAN;
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
    // ADC1->CR2 |= ADC_CR2_ADON;

    /* ----------------- 触发方式二：使用软件外部触发来启动规则通道转换 ----------------- */
    // 需要将 3.11 注释掉
    // // 3.12 使能规则通道的外部触发转换模式
    ADC1->CR2 |= ADC_CR2_EXTTRIG;
    // // 3.13 选择启动规则通道转换的外部事件（选择外部触发源），配置为软件触发SWSTART
    ADC1->CR2 |= ADC_CR2_EXTSEL;
    // 3.14 开始转换规则通道（手动产生触发信号）
    ADC1->CR2 |= ADC_CR2_SWSTART;
    // 3.15 开启转换结束的中断使能
    ADC1->CR1 |= ADC_CR1_EOCIE;

    // 4. NVIC配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(ADC1_2_IRQn, 3);
    NVIC_EnableIRQ(ADC1_2_IRQn);
}

uint8_t isConvert = 0;
/**
 * @brief ADC1的中断服务函数
 *
 */
void ADC1_2_IRQHandler(void)
{
    // 清空标志位
    ADC1->SR &= ~ADC_SR_EOC;

    // 转换完成
    isConvert = 1;
}

/**
 * @brief ADC1单通道读电压
 *
 * @return double 电压值
 */
double ADC1_SingleReadV(void)
{
    // 12位的ADC范围[0，4095]
    return ADC1->DR * 3.3 / 4095; // (ADC1->DR) * 3.3 / (2 ^ n - 1)   n = 12
}

/**
 * @brief ADC1双通道初始化
 *
 */
void ADC1_DoubleChannelDMAInit(void)
{
    // 1. 使能时钟
    // 1.1 使能GPIOC时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // 1.2 使能ADC1时钟
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // 1.3 使能DMA时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. GPIO初始化
    // 2.1 配置PC0为模拟输入
    GPIOC->CRL &= ~GPIO_CRL_MODE0;
    GPIOC->CRL &= ~GPIO_CRL_CNF0;
    // 2.2 配置PC2为模拟输入
    GPIOC->CRL &= ~GPIO_CRL_MODE2;
    GPIOC->CRL &= ~GPIO_CRL_CNF2;

    // 3. ADC1初始化
    // 3.1 配置ADC1时钟，因为ADC的输入时钟不得超过14MHz，所以要在RCC寄存器中对ADC的时钟进行分频，6分频后为12MHz
    RCC->CR |= RCC_CFGR_ADCPRE_1;
    RCC->CR &= ~RCC_CFGR_ADCPRE_0;
    // 3.2 配置扫描模式，因为当前只两个通道，所以要开启扫描模式（按照歌单从前往后扫）
    ADC1->CR1 |= ADC_CR1_SCAN;
    // 3.3 配置数据对齐，右对齐。ADC 精度12位 数据寄存器16位 默认是右对齐（不用配）
    ADC1->CR2 &= ~ADC_CR2_ALIGN;
    // 3.4 配置连续转换，不停的扫描。结合之前的连续扫描意思是歌单循环
    ADC1->CR2 |= ADC_CR2_CONT;
    // 3.5 配置ADC规则序列（歌单配置），PC0口为ADC1的10通道。PC2口为ADC1的12通道，
    ADC1->SQR3 |= 10;
    ADC1->SQR3 |= 12 << 5;
    // 3.6 配置规则通道序列长度，当前有两个通道，所以配置为2
    ADC1->SQR1 |= ADC_SQR1_L_0;
    // 3.7 配置通道10和通道12的采样时间，7.5周期，（周期根据12M主频算）
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10_2;
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10_1;
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_0;

    ADC1->SMPR1 &= ~ADC_SMPR1_SMP12_2;
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP12_1;
    ADC1->SMPR1 |= ADC_SMPR1_SMP12_0;

    /* --------------------- 4. DMA初始化（要写在ADC开始转换之前） --------------------- */
    // 4.1 设置外设寄存器的地址
    DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
    // 4.2 配置通道优先级
    DMA1_Channel1->CCR &= ~DMA_CCR1_PL;
    // 4.3 配置存储器到存储器模式，默认关闭存储器到存储器模式
    DMA1_Channel1->CCR &= ~DMA_CCR1_MEM2MEM;
    // 4.4 配置数据的传输方向，从外设读
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;
    // 4.5 配置循环模式，执行循环操作
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;
    // 4.6 配置外设的增量模式
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;
    // 4.7 配置存储器的增量模式
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;
    // 4.8 配置外设的宽度，16位
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;
    // 4.9 配置存储器的宽度，16位
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;
    // 4.10 在ADC1里使能DMA模式
    ADC1->CR2 |= ADC_CR2_DMA;

    /* -------------------------------- DMA初始化结束 --------------------------------- */

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
    // ADC1->CR2 |= ADC_CR2_ADON;

    /* ----------------- 触发方式二：使用软件外部触发来启动规则通道转换 ----------------- */
    // 需要将 3.11 注释掉
    // 3.12 使能规则通道的外部触发转换模式
    ADC1->CR2 |= ADC_CR2_EXTTRIG;
    // 3.13 选择启动规则通道转换的外部事件（选择外部触发源），配置为软件触发SWSTART
    ADC1->CR2 |= ADC_CR2_EXTSEL;
    // 3.14 开始转换规则通道（手动产生触发信号），ADC的转换是在DMA通道使能之才做的，在ADC1_DoubleChannelStart函数中执行
}

/**
 * @brief ADC1双通道开启测量
 *
 */
void ADC1_DoubleChannelStart(uint16_t *pData, uint16_t len)
{
    // 配置存储器寄存器，发送到数据的地址
    DMA1_Channel1->CMAR = (uint32_t)pData;
    // 配置传输数量寄存器
    DMA1_Channel1->CNDTR = len;

    // 使能DMA
    DMA1_Channel1->CCR |= DMA_CCR1_EN;

    // 3.14 开始转换规则通道（手动产生触发信号），ADC的转换是在DMA通道使能之才做的。
    ADC1->CR2 |= ADC_CR2_SWSTART;
}