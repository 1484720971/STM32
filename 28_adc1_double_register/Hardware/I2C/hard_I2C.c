#include "hard_I2C.h"

/**
 * @brief I2C初始化
 *
 */
void I2C_Init(void)
{
    // 1. 时钟配置
    // 1.1 使能GPIOB时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // 1.2 使能I2C时钟
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // 2. 配置GPIO     PB10 -> SCL   PB11 -> SDA
    // 2.1 配置PB10、PB11为复用开漏输出
    GPIOB->CRH |= (GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
    GPIOB->CRH |= (GPIO_CRH_CNF11 | GPIO_CRH_MODE11);

    // 3. 配置I2C
    // 3.1 在I2C_CR2寄存器中设定该模块的输入时钟以产生正确的时序  36MHz
    I2C2->CR2 |= (I2C_CR2_FREQ_5 | I2C_CR2_FREQ_2);
    // 3.2 配置时钟控制寄存器
    I2C2->CCR |= 180;
    // 3.3 配置上升时间寄存器
    I2C2->TRISE |= 37;  // 1us / (1 / 36) + 1 = 37   弱上拉强下拉需要躲避SDA和SCK的上升时间
    // 3.4 编程I2C_CR1寄存器启动外设
    I2C2->CR1 |= I2C_CR1_PE;
}

/**
 * @brief I2C起始信号
 *
 * 对应图中的EV5
 */
I2C_STATE I2C_Start(void)
{
    // 1. 产生起始信号：将CR1寄存器的START位置1
    I2C2->CR1 |= I2C_CR1_START;

    // 2. 等待起始信号发送完成：通过判断SR1寄存器的SB标志位
    // SB位在起始信号发送成功后置1
    uint16_t timeout = UINT16_MAX;
    while (((I2C2->SR1 & I2C_SR1_SB) == 0) && timeout)
    {
        timeout--;
    }

    // 3. 返回状态：超时返回FAIL（从设备无响应或总线忙），成功返回OK
    return timeout ? OK : FAIL;
}


/**
 * @brief I2C发送地址
 *
 * @param addr 地址
 *
 * 对应图中的EV6
 */
I2C_STATE I2C_SendAddr(uint8_t addr)
{
    // 1. 将从设备地址写入DR寄存器，硬件会自动发送地址
    I2C2->DR = addr;

    // 2. 等待ADDR标志位（地址发送完成，且从设备已响应）
    uint16_t timeout = UINT16_MAX;
    while (((I2C2->SR1 & I2C_SR1_ADDR) == 0) && timeout)
    {
        timeout--;
    }

    // 3. 清除ADDR标志位：读SR1后再读SR2（SR1在上面的循环中已读取）
    I2C2->SR2;

    // 4. 返回状态：超时返回FAIL，成功返回OK
    return timeout ? OK : FAIL;
}

/**
 * @brief I2C发送一个字节
 *
 * @param byte
 *
 * 对应图中的EV8_1 和 EV8
 */
I2C_STATE I2C_SendByte(uint8_t byte)
{
    // 等待数据寄存器为空（TXE=1表示DR中的数据已转移到移位寄存器，DR可以写入新数据）
    uint16_t timeout = UINT16_MAX;
    while ((I2C2->SR1 & I2C_SR1_TXE) == 0 && timeout)
    {
        timeout--;
    }

    // 写入数据到DR，硬件会自动将数据发送到总线
    I2C2->DR = byte;

    return timeout ? OK : FAIL;
}

/**
 * @brief I2C接收一个字节
 *
 * @return uint8_t 接收到的数据
 * 
 * 对应图中的EV7
 */
I2C_STATE I2C_ReceiveByte(uint8_t* data)
{
    uint16_t timeout = UINT16_MAX;
    while ((I2C2->SR1 & I2C_SR1_RXNE) == 0 && timeout)
    {
        timeout--;
    }

    if (timeout != 0)
    {
        *data = I2C2->DR & I2C_DR_DR;
    }

    return timeout ? OK : FAIL;
}

/**
 * @brief 开启ACK使能
 * 这个ACK指的是作为接收器时，给别人返回ACK
 * 不是作为发送器 别人给我们的ACK
 */
void I2C_SendACK(void)
{
    I2C2->CR1 |= I2C_CR1_ACK;
}

/**
 * @brief 关闭ACK使能
 * 关闭ACK之后 对方给我们发送数据 我们就给他NACK
 */
void I2C_SendNACK(void)
{
    I2C2->CR1 &= ~I2C_CR1_ACK;
}

/**
 * @brief I2C停止信号
 *
 * 对应图中的EV8_2 和 EV9
 */
I2C_STATE I2C_SendStop(void)
{
    // 1. 等待当前字节传输完成（BTF=1表示数据已从DR转移到移位寄存器，且已发送完成，包括ACK位）
    uint16_t timeout = UINT16_MAX;
    while ((I2C2->SR1 & I2C_SR1_BTF) == 0 && timeout)
    {
        timeout--;
    }

    // 2. 产生停止信号：将CR1寄存器的STOP位置1
    I2C2->CR1 |= I2C_CR1_STOP;

    // 3. 返回状态：超时返回FAIL，成功返回OK
    return timeout ? OK : FAIL;
}

/**
 * @brief 设置停止位
 *
 * 对应图中的EV9
 */
void I2C_SetStop(void)
{
    I2C2->CR1 |= I2C_CR1_STOP;
}