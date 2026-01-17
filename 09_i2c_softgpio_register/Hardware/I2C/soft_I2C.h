#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

#include "stm32f10x.h"
#include "delay.h"

// SCL、SDA输出高低电平
#define SCL_HIGH GPIOB->ODR |= GPIO_ODR_ODR10
#define SCL_LOW GPIOB->ODR &= ~GPIO_ODR_ODR10
#define SDA_HIGH GPIOB->ODR |= GPIO_ODR_ODR11
#define SDA_LOW GPIOB->ODR &= ~GPIO_ODR_ODR11

// SDA读数据
#define READ_SDA GPIOB->IDR & GPIO_IDR_IDR11

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

#endif