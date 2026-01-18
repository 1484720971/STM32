#include "M24C02.h"

/**
 * @brief M24C02初始化
 *
 */
void M24C02_Init(void)
{
    I2C_Init();
}

/**
 * @brief 向M24C02发送一个字节数据
 *
 */
void M24C02_SendByte(uint8_t addr, uint8_t data)
{
    // 起始信号
    I2C_Start();

    // 发送写地址
    I2C_SendByte(WRITE_ADDR);
    // 获取响应
    I2C_GetAck();

    // 发送要写入的地址
    I2C_SendByte(addr);
    // 获取响应
    I2C_GetAck();

    // 发送数据
    I2C_SendByte(data);
    // 获取响应
    I2C_GetAck();

    // I2C结束信号
    I2C_Stop();

    Delay_ms(5);
}

/**
 * @brief 向M24C02发送多个字节数据
 *
 * @param addr 写入的地址
 * @param data 写入的数据
 */
void M24C02_SendBytes(uint8_t addr, uint8_t *pData, uint8_t len)
{
    // 起始信号
    I2C_Start();

    // 发送写地址
    I2C_SendByte(WRITE_ADDR);
    // 获取响应
    I2C_GetAck();

    // 发送要写入的地址
    I2C_SendByte(addr);
    // 获取响应
    I2C_GetAck();

    // 循环发送字符串的单个字符
    for (uint8_t i = 0; i < len; i++)
    {
        I2C_SendByte(*(pData + i));
        // 获取响应
        I2C_GetAck();
    }

    // I2C结束信号
    I2C_Stop();

    Delay_ms(5);
}

/**
 * @brief 从M24C02中读取一个字节数据
 *
 * @param addr 读取的地址
 * @param byte 读取的数据
 */
uint8_t M24C02_ReadByte(uint8_t addr)
{
    // 起始信号
    I2C_Start();

    // 发送写地址（假写）
    I2C_SendByte(WRITE_ADDR);
    // 获取响应
    I2C_GetAck();

    // 发送要读取的地址
    I2C_SendByte(addr);
    // 获取响应
    I2C_GetAck();

    // 起始信号
    I2C_Start();

    // 发送读地址
    I2C_SendByte(READ_ADDR);
    // 获取响应
    I2C_GetAck();

    // 读取数据
    uint8_t data = I2C_ReceiveByte();
    // 发送NACK响应
    I2C_SendNACK();

    // I2C结束信号
    I2C_Stop();

    return data;
}

/**
 * @brief 从M24C02中读取多个字节数据
 *
 * @param start_addr 读取数据的起始地址
 * @param len 读取数据的个数
 * @param buffer 读取的数据
 */
void M24C02_ReadBytes(uint8_t start_addr, uint8_t len, uint8_t *buffer)
{
    // 起始信号
    I2C_Start();

    // 发送写地址
    I2C_SendByte(WRITE_ADDR);
    // 获取响应
    I2C_GetAck();

    // 发送要读取数据的起始地址
    I2C_SendByte(start_addr);
    // 获取响应
    I2C_GetAck();
    /* ---------------- 假写结束 ---------------- */

    // 起始信号
    I2C_Start();

    // 发送读地址
    I2C_SendByte(READ_ADDR);
    // 获取响应
    I2C_GetAck();

    // 读取数据
    for (uint8_t i = 0; i < len; i++)
    {
        *(buffer + i) = I2C_ReceiveByte();
        // 发送响应。接收完一个字节，发ACK。接收完最后一个字节后，发NACK
        if (i != len - 1)
        {
            I2C_SendACK();
        }
        else
        {
            I2C_SendNACK();
        }
    }

    // 结束信号
    I2C_Stop();
}