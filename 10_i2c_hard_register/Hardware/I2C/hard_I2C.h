#ifndef __HARD_I2C_H__
#define __HARD_I2C_H__

#include "stm32f10x.h"

typedef enum
{
    ACK,
    NACK
} ACK_TypeDef;

typedef enum
{
    OK,
    FAIL
} I2C_STATE;

/**
 * @brief I2C初始化
 *
 */
void I2C_Init(void);

/**
 * @brief I2C起始信号
 *
 * @return I2C_STATE    OK：起始信号设置成功    FAIL：起始信号设置失败
 */
I2C_STATE I2C_Start(void);

/**
 * @brief I2C发送地址
 *
 * @param addr 地址
 */
I2C_STATE I2C_SendAddr(uint8_t addr);

/**
 * @brief I2C发送一个字节
 *
 * @param byte
 */
I2C_STATE I2C_SendByte(uint8_t byte);

/**
 * @brief I2C停止信号
 *
 */
I2C_STATE I2C_SendStop(void);

/**
 * @brief 设置停止位
 * 
 */
void I2C_SetStop(void);

/**
 * @brief I2C接收一个字节
 *
 * @return uint8_t 接收到的数据
 */
I2C_STATE I2C_ReceiveByte(uint8_t* data);

/**
 * @brief 开启ACK使能
 *
 */
void I2C_SendACK(void);

/**
 * @brief 关闭ACK使能
 *
 */
void I2C_SendNACK(void);

#endif