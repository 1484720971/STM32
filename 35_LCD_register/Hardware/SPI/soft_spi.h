#ifndef __SOFT_SPI_H__
#define __SOFT_SPI_H__

#include "stm32f10x.h"

// 片选
#define SS_LOW      GPIOC->ODR &= ~GPIO_ODR_ODR13
#define SS_HIGH     GPIOC->ODR |= GPIO_ODR_ODR13
// 时钟
#define SCK_LOW     GPIOA->ODR &= ~GPIO_ODR_ODR5
#define SCK_HIGH    GPIOA->ODR |= GPIO_ODR_ODR5
// 主设备输出
#define MOSI_LOW    GPIOA->ODR &= ~GPIO_ODR_ODR7
#define MOSI_HIGH   GPIOA->ODR |= GPIO_ODR_ODR7
// 主设备输入
#define MISO_READ   GPIOA->IDR & GPIO_IDR_IDR6

/**
 * @brief SPI初始化
 *
 */
void SPI_Init(void);

/**
 * @brief SPI起始信号
 *
 */
void SPI_Start(void);

/**
 * @brief SPI结束信号
 *
 */
void SPI_Stop(void);

/**
 * @brief SPI交换单字节数据
 *
 */
uint8_t SPI_SwapByte(uint8_t byte);


#endif /* __SOFT_SPI_H__ */