#include "hard_spi.h"

/**
 * @brief SPI起始信号
 *
 */
void SPI_Start(void)
{
    SS_LOW;
}

/**
 * @brief SPI结束信号
 *
 */
void SPI_Stop(void)
{
    SS_HIGH;
}

/**
 * @brief SPI交换单字节数据
 *
 * 进入这个函数时 SCK应该是低电平 因为MODE0的话 低电平表示空闲
 */
uint8_t SPI_SwapByte(uint8_t byte)
{
    uint8_t receice_byte = 0;
    HAL_SPI_TransmitReceive(&hspi1, (const uint8_t *)&byte, &receice_byte, 1, UINT32_MAX);
    return receice_byte;
}