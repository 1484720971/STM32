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
 * @param addr 存储地址 (0x00 ~ 0xFF)
 * @param data 要写入的数据 (0x00 ~ 0xFF)
 */
void M24C02_SendByte(uint8_t addr, uint8_t data)
{
    I2C_Start();              // 产生起始条件, 占用总线
    I2C_SendAddr(WRITE_ADDR); // 发送0xA0, 选择写操作
    I2C_SendByte(addr);       // 发送存储地址 (0x00-0xFF)
    I2C_SendByte(data);       // 发送要写入的数据
    I2C_SendStop();           // 产生停止条件, 释放总线
    Delay_ms(5);              // 等待EEPROM完成内部写入
}

/**
 * @brief 向M24C02发送多个字节数据 (页写入)
 *
 * @param addr  起始存储地址 (建议页对齐)
 * @param pData 指向要写入数据的指针
 * @param len   要写入的字节数 (建议 ≤ 16)
 */
void M24C02_SendBytes(uint8_t start_addr, uint8_t* pData, uint8_t len)
{
    I2C_Start();
    I2C_SendAddr(WRITE_ADDR);
    I2C_SendByte(start_addr);
    for (uint8_t i = 0; i < len; i++)
    {
        I2C_SendByte(*(pData + i));
    }
    I2C_SendStop();
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
    /**
     * Random Read 需要两次START:
     * 1. 假写操作 (设置内部地址指针)
     * 2. 重新开始读操作
     */
     // 1. 假写: 设置存储地址
    I2C_Start();
    I2C_SendAddr(WRITE_ADDR);   // 写地址
    I2C_SendByte(addr); // 发送要读取的地址

    // 2. 重启: 切换为读操作
    I2C_Start();
    I2C_SendAddr(READ_ADDR);    // 读地址

    I2C_SendNACK();
    I2C_SetStop();

    // 3. 读取数据, 发送NACK (最后一位)
    uint8_t data = 0;
    I2C_ReceiveByte(&data);

    return data;
}

/**
 * @brief 从M24C02中读取多个字节数据
 *
 * @param start_addr 读取数据的起始地址
 * @param len 读取数据的个数
 * @param buffer 读取的数据
 */
void M24C02_ReadBytes(uint8_t start_addr, uint8_t len, uint8_t* buffer)
{
    // 1. 假写: 设置起始地址
    I2C_Start();
    I2C_SendAddr(WRITE_ADDR);
    I2C_SendByte(start_addr);

    // 2. 重启: 切换为读操作
    I2C_Start();
    I2C_SendAddr(READ_ADDR);

    I2C_SendACK();

    for (uint8_t i = 0; i < len; i++)
    {
        if (i == len - 1)
        {
            // 最后一位: 发送NACK
            I2C_SendNACK();
            I2C_SetStop();
        }

        I2C_ReceiveByte(buffer + i);
    }
}