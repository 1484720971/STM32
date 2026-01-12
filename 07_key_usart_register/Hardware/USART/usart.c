#include <usart.h>
#include <stdio.h>

/**
 * IO重定向 设置PA9 PA10为默认复用功能
 * PA9 -> USART_TX      因为PA9要设置为USART的发送引脚，所以要设置为输出模式。因为是让片上外设USART来输出高低电平，所以应该设置为复用推挽输出
 * PA10 -> USART_RX     因为PA10要设置为USART的接收引脚，所以要设置为输入模式。因为，串口接收数据是被动接收外部电平，浮空输入对串口数据的识别最准确，禁止配置为上拉 / 下拉输入，会导致串口接收乱码。
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
uint8_t USART1_TransmitBytes(uint8_t *bytes, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        USART1_TransmitByte(bytes[i]);
    }
}

/**
 * USART接收字符串
 */
void USART1_ReceiveBytes(uint8_t* buffer, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        buffer[i] = USART1_ReceiveByte();
    }
}

int fputc(int ch, FILE *file)
{
    USART1_TransmitByte(ch);

    return ch;
}