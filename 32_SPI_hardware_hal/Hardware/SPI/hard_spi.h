#ifndef __HARD_SPI_H__
#define __HARD_SPI_H__

#include "stm32f103xe.h"
#include "main.h"
#include "spi.h"

#define SS_LOW HAL_GPIO_WritePin(GPIOC, FLASH_SS_Pin, GPIO_PIN_RESET)
#define SS_HIGH HAL_GPIO_WritePin(GPIOC, FLASH_SS_Pin, GPIO_PIN_SET)

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