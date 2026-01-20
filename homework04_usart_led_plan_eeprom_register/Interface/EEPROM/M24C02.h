#ifndef __M24C02_H__
#define __M24C02_H__

#include "soft_I2C.h"

#define WRITE_ADDR 0xA0
#define READ_ADDR 0xA1

/**
 * @brief M24C02初始化
 *
 */
void M24C02_Init(void);

/**
 * @brief 向M24C02发送一个字节数据
 * @param addr 写入的地址
 * @param data 写入的数据
 */
void M24C02_SendByte(uint8_t addr, uint8_t data);

/**
 * @brief 向M24C02发送多个字节数据
 *
 * @param addr 写入的地址
 * @param data 写入的数据
 */
void M24C02_SendBytes(uint8_t addr, uint8_t *pData, uint8_t len);

/**
 * @brief 从M24C02中读取一个字节数据
 *
 * @param addr 读取的地址
 * @param byte 读取的数据
 */
uint8_t M24C02_ReadByte(uint8_t addr);

/**
 * @brief 从M24C02中读取多个字节数据
 *
 * @param start_addr 读取数据的起始地址
 * @param len 读取数据的个数
 * @param buffer 读取的数据
 */
void M24C02_ReadBytes(uint8_t start_addr, uint8_t len, uint8_t *buffer);

#endif /* __M24C02_H__ */