#include "usart1_dma.h"

/**
 * @brief  串口1DAM发送功能初始化
 *
 */
void DMA_USART1_TxInit(void)
{
    // 1. 使能时钟
    // 1.1 使能DMA时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. DMA初始化
    // 2.1 设置外设寄存器的地址
    DMA1_Channel4->CPAR = (uint32_t)&USART1->DR;
    // 2.2 配置通道优先级
    DMA1_Channel4->CCR &= ~DMA_CCR4_PL;
    // 2.3 配置存储器到存储器模式，默认关闭存储器到存储器模式
    DMA1_Channel4->CCR &= ~DMA_CCR4_MEM2MEM;
    // 2.4 配置数据的传输方向，从存储器（SRAM）读
    DMA1_Channel4->CCR |= DMA_CCR4_DIR;
    // 2.5 配置循环模式，不执行循环操作
    DMA1_Channel4->CCR &= ~DMA_CCR4_CIRC;
    // 2.6 配置外设的增量模式
    DMA1_Channel4->CCR &= ~DMA_CCR4_PINC;
    // 2.7 配置存储器的增量模式
    DMA1_Channel4->CCR |= DMA_CCR4_MINC;
    // 2.8 配置外设的宽度，8位
    DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE;
    // 2.9 配置存储器的宽度，8位
    DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;
}

/**
 * @brief 串口1DMA接收功能初始化
 *
 */
void DMA_USART1_RxInit(void)
{
    // 1. 使能时钟
    // 1.1 使能DMA时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. DMA初始化
    // 2.1 设置外设寄存器的地址
    DMA1_Channel5->CPAR = (uint32_t)&USART1->DR;
    // 2.2 配置通道优先级
    DMA1_Channel5->CCR &= ~DMA_CCR5_PL;
    // 2.3 配置存储器到存储器模式，默认关闭存储器到存储器模式
    DMA1_Channel5->CCR &= ~DMA_CCR5_MEM2MEM;
    // 2.4 配置数据的传输方向，从外设（串口）读
    DMA1_Channel5->CCR &= ~DMA_CCR5_DIR;
    // 2.5 配置循环模式，不执行循环操作
    DMA1_Channel5->CCR |= DMA_CCR5_CIRC;
    // 2.6 配置外设的增量模式
    DMA1_Channel5->CCR &= ~DMA_CCR5_PINC;
    // 2.7 配置存储器的增量模式
    DMA1_Channel5->CCR |= DMA_CCR5_MINC;
    // 2.8 配置外设的宽度，8位
    DMA1_Channel5->CCR &= ~DMA_CCR5_PSIZE;
    // 2.9 配置存储器的宽度，8位
    DMA1_Channel5->CCR &= ~DMA_CCR5_MSIZE;
    // 2.10 配置DMA传输完成中断使能
    // DMA1_Channel5->CCR |= DMA_CCR5_TCIE;

    // 3. 配置NVIC
    // 3.1 配置是抢占优先级还是响应优先级
    NVIC_SetPriorityGrouping(3);
    // 3.2 配置具体优先级
    NVIC_SetPriority(DMA1_Channel5_IRQn, 2);
    // 3.3 NVIC使能
    NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

/**
 * @brief 配置DMA要发送的数据的地址和长度
 *
 * @param pData 数据的地址
 * @param len 数据的长度
 */
void DMA_USART1_TransmitData(uint8_t *pData, uint16_t len)
{
    // 配置存储器寄存器，发送到数据的地址
    DMA1_Channel4->CMAR = (uint32_t)pData;
    // 配置传输数量寄存器
    DMA1_Channel4->CNDTR = len;

    // 使能DMA
    DMA1_Channel4->CCR |= DMA_CCR1_EN;
}

/**
 * @brief 配置DMA串口接收数据
 *
 * @param pData
 * @param len
 */
void DMA_USART1_ReceiveData(uint8_t *pData, uint16_t len)
{
    // 配置存储器寄存器，发送到数据的地址
    DMA1_Channel5->CMAR = (uint32_t)pData;
    // 配置传输数量寄存器
    DMA1_Channel5->CNDTR = len;

    // 使能DMA
    DMA1_Channel5->CCR |= DMA_CCR1_EN;
}

uint8_t TrCompleteFlag = 0;
// DMA1通道5的中断服务函数
void DMA1_Channel5_IRQHandler(void)
{
    // 清除中断标志位
    DMA1->IFCR |= DMA_IFCR_CTCIF5;

    TrCompleteFlag = 1;
}