#include <stdio.h>
#include <usart.h>

/**
 * IO重定向 设置PA9 PA10为默认复用功能
 * PA9 -> USART_TX
 * 因为PA9要设置为USART的发送引脚，所以要设置为输出模式。因为是让片上外设USART来输出高低电平，所以应该设置为复用推挽输出
 * PA10 -> USART_RX
 * 因为PA10要设置为USART的接收引脚，所以要设置为输入模式。因为，串口接收数据是被动接收外部电平，浮空输入对串口数据的识别最准确，禁止配置为上拉
 * / 下拉输入，会导致串口接收乱码。
 */

/**
 * USART1初始化
 */
void USART1_Init(void)
{
    // 1. PA9 和 PA10 开启复用功能
    // (1) 使能GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // (2) PA9 设置为复用推挽输出
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
    // (3) PA10 设置为浮空输入（默认就是浮空输入）
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;
    GPIOA->CRH &= ~GPIO_CRH_MODE10;

    // 2. USART1 初始化
    // (1) 使能USART1时钟
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // ---------------以下代码按照 "STM32F10x参考手册" 第25章通用同步异步收发器的USART配置步骤来配置 ---------------
    // (2) 通过在USART_CR1寄存器上置位UE位来激活USART（USART模块使能）
    USART1->CR1 |= USART_CR1_UE;
    // (3) 编程USART_CR1的M位来定义字长
    USART1->CR1 &= ~USART_CR1_M;
    // (4) 在USART_CR2中编程停止位的位数
    USART1->CR2 &= ~USART_CR2_STOP;
    // (5) 利用USART_BRR寄存器选择要求的波特率
    USART1->BRR = 0x271;
    // (6) 设置USART_CR1中的TE位，发送一个空闲帧作为第一次数据发送
    USART1->CR1 |= USART_CR1_TE;
    // (7) 设置USART_CR1的RE位。激活接收器，使它开始寻找起始位
    USART1->CR1 |= USART_CR1_RE;
}

/**
 * @brief USART1初始化带中断
 *
 */
void USART1_InitWithIt(void)
{
    // 1. PA9 和 PA10 开启复用功能
    // (1) 使能GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // (2) PA9 设置为复用推挽输出
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
    // (3) PA10 设置为浮空输入（默认就是浮空输入）
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;
    GPIOA->CRH &= ~GPIO_CRH_MODE10;

    // 2. USART1 初始化
    // (1) 使能USART1时钟
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // ---------------以下代码按照 "STM32F10x参考手册" 第25章通用同步异步收发器的USART配置步骤来配置 ---------------
    // (2) 通过在USART_CR1寄存器上置位UE位来激活USART（USART模块使能）
    USART1->CR1 |= USART_CR1_UE;
    // (3) 编程USART_CR1的M位来定义字长
    USART1->CR1 &= ~USART_CR1_M;
    // (4) 在USART_CR2中编程停止位的位数
    USART1->CR2 &= ~USART_CR2_STOP;
    // (5) 利用USART_BRR寄存器选择要求的波特率
    USART1->BRR = 0x271;
    // (6) 设置USART_CR1中的TE位，发送一个空闲帧作为第一次数据发送
    USART1->CR1 |= USART_CR1_TE;
    // (7) 设置USART_CR1的RE位。激活接收器，使它开始寻找起始位
    USART1->CR1 |= USART_CR1_RE;
    // (8) 使能接收缓冲区非空中断使能 -> 每接收完一个字节进一次中断
    USART1->CR1 |= USART_CR1_RXNEIE;
    // (9) IDLE中断使能 -> 全部传输完成进一次中断
    USART1->CR1 |= USART_CR1_IDLEIE;

    // 配置NVIC
    // (1) 设置抢占优先级还是响应优先级
    NVIC_SetPriorityGrouping(3); // 全是抢占优先级
    // (2) 设置中断优先级
    NVIC_SetPriority(USART1_IRQn, 8); // 设置USARAT中断的优先级为8
    // (3) 使能NVIC中断
    NVIC_EnableIRQ(USART1_IRQn);
}

/**
 * USART1发送一个字节
 */
void USART1_TransmitByte(uint8_t byte)
{
    while ((USART1->SR & USART_SR_TXE) == 0)
    {
    }
    USART1->DR = byte;
}

/**
 * USART1接收一个字节
 */
uint8_t USART1_ReceiveByte(void)
{
    while ((USART1->SR & USART_SR_RXNE) == 0)
    {
    }
    return USART1->DR;
}

/**
 * USART发送字符串
 */
void USART1_TransmitBytes(uint8_t *bytes, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        USART1_TransmitByte(bytes[i]);
    }
}

/**
 * USART接收字符串
 */
void USART1_ReceiveBytes(uint8_t *buffer, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        buffer[i] = USART1_ReceiveByte();
    }
}

/**
 * @brief  使用IDLE中断机制接收字节数据
 *         该函数利用USART1的IDLE中断特性来接收一帧完整的数据
 * @param  buffer 指向接收数据缓冲区的指针
 * @param  len 用于存储实际接收到的字节数的变量的指针
 * @retval None
 */
void USART1_ReceiveBytesToIdle(uint8_t *buffer, uint8_t *len)
{
    uint8_t count = 0; // 记录已接收的字节数
    while (1)
    {
        // 等待接收数据寄存器RXNE置位
        while ((USART1->SR & USART_SR_RXNE) == 0)
        {
            // 检查是否检测到总线空闲状态(IDLE)
            if (USART1->SR & USART_SR_IDLE) // 要清除IDLE位，先读SR寄存器
            {
                USART1->DR;   // 再读DR寄存器，就可将IDLE位清除
                *len = count; // 保存接收到的字节数
                return;       // 结束函数
            }
        }

        // 从数据寄存器读取接收到的字节，并递增计数器
        buffer[count++] = USART1->DR & USART_DR_DR;
    }
}

int fputc(int ch, FILE *file)
{
    USART1_TransmitByte(ch);

    return ch;
}

// 定义接收缓冲区，用于存放串口接收到的原始字节数据
uint8_t it_rx_buffer[100] = {0};

// 定义接收计数器，记录当前缓冲区中已存储的字节数（即当前数据包长度）
uint8_t it_rx_num = 0;

// 定义接收完成标志位，0表示未完成，1表示检测到总线空闲（一帧数据接收完毕）
uint8_t it_rx_idle_flag = 0;

/**
 * @brief USART1 中断服务函数
 * @note  该函数由硬件在满足中断条件时自动调用（需先开启 RXNE 和 IDLE 中断使能）
 */
void USART1_IRQHandler()
{
    /* 1. 判定接收寄存器非空标志位 (RXNE) */
    // 当串口接收到一个字节的数据并从移位寄存器转移到数据寄存器(DR)时，该位置1
    if (USART1->SR & USART_SR_RXNE)
    {
        // 从DR寄存器读取数据并存入缓冲区，同时将计数器自增
        // 读取DR的操作会自动清除 RXNE 标志位，无需手动清除
        // 使用 & USART_DR_DR 掩码是为了只提取有效的 8/9 位数据内容
        it_rx_buffer[it_rx_num++] = USART1->DR & USART_DR_DR;
    }

    /* 2. 判定空闲总线检测标志位 (IDLE) */
    // 当串口检测到总线上在一个字节的时间内没有新的数据传输时（即一帧数据结束），该位置1
    if (USART1->SR & USART_SR_IDLE)
    {
        /**
         * 清除 IDLE 标志位的硬件序列要求：
         * 第一步：读取 USART_SR 寄存器（在上面的 if 判断条件中已经完成读取动作）
         * 第二步：读取 USART_DR 寄存器（如下行代码所示）
         * 只有按顺序执行这两步，硬件才会将 IDLE 位清零，否则会持续触发中断导致程序死机
         */
        USART1->DR;

        // 置位软件标志位，通知主程序（main loop）可以开始处理这包完整的数据了
        it_rx_idle_flag = 1;
    }
}

/**
 * @brief 使能USART1的DMA发送功能
 *
 */
void USART1_EnableDMATransmit()
{
    USART1->CR3 |= USART_CR3_DMAT;
}

/**
 * @brief 使能USART1的DMA接收
 *
 */
void USART1_EnableDMAReceive()
{
    USART1->CR3 |= USART_CR3_DMAR;
}