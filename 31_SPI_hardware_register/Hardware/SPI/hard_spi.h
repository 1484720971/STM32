#ifndef __HARD_SPI_H__
#define __HARD_SPI_H__

#include "stm32f10x.h"

#define SS_LOW GPIOC->ODR &= ~GPIO_ODR_ODR13
#define SS_HIGH GPIOC->ODR |= GPIO_ODR_ODR13

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
 * 进入这个函数时 SCK应该是低电平 因为MODE0的话 低电平表示空闲
 */
uint8_t SPI_SwapByte(uint8_t byte);

#endif /* __HARD_SPI_H__ */