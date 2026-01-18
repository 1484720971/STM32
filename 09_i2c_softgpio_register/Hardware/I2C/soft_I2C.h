#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

#include "delay.h"
#include "stm32f10x.h"

// SCL、SDA输出高低电平
#define SCL_HIGH GPIOB->ODR |= GPIO_ODR_ODR10
#define SCL_LOW GPIOB->ODR &= ~GPIO_ODR_ODR10
#define SDA_HIGH GPIOB->ODR |= GPIO_ODR_ODR11
#define SDA_LOW GPIOB->ODR &= ~GPIO_ODR_ODR11

// SDA读数据
#define READ_SDA GPIOB->IDR & GPIO_IDR_IDR11

typedef enum
{
    ACK,
    NACK
} ACK_TypeDef;

/**
 * @brief I2C初始化
 *
 */
void I2C_Init(void);

/**
 * @brief I2C起始信号
 *
 */
void I2C_Start(void);

/**
 * @brief I2C停止信号
 *
 */
void I2C_Stop(void);

/**
 * @brief I2C发送一个字节
 *
 * @param byte
 */
void I2C_SendByte(uint8_t byte);

/**
 * @brief I2C获取主设备有没有响应
 *
 */
ACK_TypeDef I2C_GetAck(void);

/**
 * @brief I2C接收一个字节
 * 
 * @return uint8_t 接收到的数据
 */
uint8_t I2C_ReceiveByte(void);

/**
 * @brief 发送ACK
 * 
 */
void I2C_SendACK(void);

/**
 * @brief 发送NACK
 * 
 */
void I2C_SendNACK(void);

#endif