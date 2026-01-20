#include "hard_I2C.h"

/**
 * @brief I2C初始化
 *
 */
void I2C_Init(void)
{
    // 1. 配置GPIO     PB10 -> SCL   PB11 -> SDA
    // 1.1 使能GPIOB时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2.1 配置PB10、PB11为复用开漏输出
    GPIOB->CRH |= (GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
    GPIOB->CRH |= (GPIO_CRH_CNF11 | GPIO_CRH_MODE11);

    // 2. 配置I2C
    // 2.1 使能I2C时钟
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    // 2.2 在I2C_CR2寄存器中设定该模块的输入时钟以产生正确的时序  36MHz
    I2C2->CR2 |= (I2C_CR2_FREQ_5 | I2C_CR2_FREQ_2);
    // 2.3 配置时钟控制寄存器
    I2C2->CCR |= 180;
    // 2.4 配置上升时间寄存器
    I2C2->TRISE |= 37;  // 1us / (1 / 36) + 1 = 37   弱上拉强下拉需要躲避SDA和SCK的上升时间
    // 2.5 编程I2C_CR1寄存器启动外设
    I2C2->CR1 |= I2C_CR1_PE;
}

/**
 * @brief I2C起始信号
 *
 */
void I2C_Start(void)
{
    // 2.6 置I2C_CR1寄存器中的START位为1，产生起始条件
    I2C2->CR1 |= I2C_CR1_START;
}

/**
 * @brief I2C停止信号
 *
 */
void I2C_Stop(void)
{

}

/**
 * @brief I2C发送一个字节
 *
 * @param byte
 */
void I2C_SendByte(uint8_t byte)
{

}

/**
 * @brief I2C获取主设备有没有响应
 *
 */
ACK_TypeDef I2C_GetAck(void)
{

}

/**
 * @brief I2C接收一个字节
 *
 * @return uint8_t 接收到的数据
 */
uint8_t I2C_ReceiveByte(void)
{

}

/**
 * @brief 发送ACK
 *
 */
void I2C_SendACK(void)
{

}

/**
 * @brief 发送NACK
 *
 */
void I2C_SendNACK(void)
{

}