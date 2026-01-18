#include "soft_I2C.h"

/**
 * @brief I2C初始化
 *
 */
void I2C_Init(void)
{
    // 1. 初始化GPIO
    // (1) 使能GPIO时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // (2) 下面要设置为开漏模式，所以必须在设置模式之前，来设置默认输出电平。确保初始化时，不会发生额外的总线占用
    GPIOB->ODR |= GPIO_ODR_ODR10;
    GPIOB->ODR |= GPIO_ODR_ODR11;

    // (3) 配置工作模式 - I2C必须用开漏模式，防止短路
    // PB10 - 通用开漏输出 - SCL
    GPIOB->CRH |= GPIO_CRH_MODE10;
    GPIOB->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOB->CRH |= GPIO_CRH_CNF10_0;
    // PB11 - 通用开漏输出 - SDA
    GPIOB->CRH |= GPIO_CRH_MODE11;
    GPIOB->CRH &= ~GPIO_CRH_CNF11_1;
    GPIOB->CRH |= GPIO_CRH_CNF11_0;
}

/**
 * @brief I2C起始信号
 * 通讯速度为100kbit/s  时钟周期为10us
 *
 * 约定我们需要确保每一个函数结束时的状态
 * 应该确保 时钟线是低电平（控制权在主设备手里） SDA是高电平
 */
void I2C_Start(void)
{
    SCL_HIGH;
    SDA_HIGH;
    Delay_us(2);
    SDA_LOW;
    Delay_us(3);
    SCL_LOW;
    Delay_us(5);
    SDA_HIGH;
}

/**
 * @brief I2C停止信号
 *
 */
void I2C_Stop(void)
{
    SDA_LOW;
    SCL_HIGH;
    Delay_us(2);
    SDA_HIGH;
    Delay_us(3);
}

/**
 * @brief I2C发送一个字节
 * 该函数通过软件模拟I2C协议发送一个字节的数据。
 * 发送顺序为高位先行（MSB first）。
 * 每个位发送后，SDA线恢复为高电平，以准备接收ACK信号。
 *
 * 因为有了约定我可以确保本函数调用时
 * SDA是高 SCL是低
 *
 * @param byte 要发送的字节数据
 */
void I2C_SendByte(uint8_t byte)
{
    // 循环发送字节的8位，高位先行
    for (uint8_t i = 0; i < 8; i++)
    {
        // 检查当前最高位是否为1
        if (byte & 0x80)
        {
            // 当前位为1，设置SDA为高电平
            SDA_HIGH;
        }
        else
        {
            // 当前位为0，设置SDA为低电平
            SDA_LOW;
        }
        // 将字节左移一位，准备下一位
        byte <<= 1;

        // 拉高SCL，产生时钟上升沿，数据位稳定
        SCL_HIGH;
        Delay_us(5);

        // 拉低SCL，准备下一个数据位
        SCL_LOW;
        // 释放SDA线（设置为高电平），以便从机发送ACK
        SDA_HIGH;
        Delay_us(5);
    }
}

/**
 * @brief I2C获取主设备有没有响应
 *
 * 因为有了约定我可以确保本函数调用时
 * SDA是高 SCL是低
 *
 */
ACK_TypeDef I2C_GetAck(void)
{
    ACK_TypeDef temp;

    SCL_HIGH;
    Delay_us(3);
    if (READ_SDA)
    {
        temp = NACK;
    }
    else
    {
        temp = ACK;
    }
    Delay_us(2);

    SCL_LOW;

    Delay_us(5);

    return temp;
}

/**
 * @brief I2C接收一个字节
 *
 * @return uint8_t 接收到的数据
 */
uint8_t I2C_ReceiveByte(void)
{
    uint8_t temp = 0; // 初始化接收数据变量为0

    // 循环接收8位数据，高位先行
    for (uint8_t i = 0; i < 8; i++)
    {
        SCL_HIGH;   // 拉高SCL，准备读取SDA数据
        temp <<= 1; // 将temp左移一位，为新位腾出位置

        if (READ_SDA) // 读取SDA线状态
        {
            temp |= 0x01; // 如果SDA为高电平，设置最低位为1
        }
        else
        {
            temp &= ~0x01; // 如果SDA为低电平，设置最低位为0（实际上是多余的，因为左移后最低位已经是0）
        }
        // 保证高电平足够长
        Delay_us(5); // 延时5us，确保时钟高电平稳定

        SCL_LOW;     // 拉低SCL，准备下一个时钟周期
        Delay_us(5); // 延时5us，确保时钟低电平稳定
    }

    return temp; // 返回接收到的字节数据
}

/**
 * @brief 发送ACK
 *
 */
void I2C_SendACK(void)
{
    // 约定 结束时 SDA为高 SCL为低
    SDA_LOW;
    SCL_HIGH;

    Delay_us(5);

    SCL_LOW;
    SDA_HIGH;

    Delay_us(5);
}

/**
 * @brief 发送NACK
 *
 */
void I2C_SendNACK(void)
{
    // 约定 结束时 SDA为高 SCL为低
    SCL_HIGH;

    Delay_us(5);

    SCL_LOW;

    Delay_us(5);
}