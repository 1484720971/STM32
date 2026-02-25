#include "soft_spi.h"


/**
 * @brief SPI初始化
 *
 */
void SPI_Init(void)
{
    // 1. 使能时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // 2. GPIO初始化
    // 2.1 将PC13的ODR置1，如果不做这个，转成通用推挽后，会立刻是低电平
    GPIOC->ODR |= GPIO_ODR_ODR13;
    // 2.2 PC13 使能W25Q32 配置为通用推挽输出
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13;
    // 2.3 RA5 时钟 配置为通用推挽输出
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
    GPIOA->CRL |= GPIO_CRL_MODE5;
    // 2.4 PA6 主设备输入 配置为浮空输入
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    // 2.5 PA7 主设备输出 配置为推挽输出
    GPIOA->CRL &= ~GPIO_CRL_CNF7;
    GPIOA->CRL |= GPIO_CRL_MODE7;
}

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
    uint8_t receive_byte = 0;
    for (uint8_t i = 0; i < 8; i++)
    {   
        // 1. 上升沿前准备数据
        if (byte & 0x80)
        {
            // 数据是1
            MOSI_HIGH;
        }
        else
        {
            // 数据是0
            MOSI_LOW;
        }
        byte <<= 1;

        // 2. 上升沿
        SCK_HIGH;

        // 3. 上升沿时读取数据
        receive_byte <<= 1;
        if (MISO_READ)
        {
            // 读到了1
            receive_byte |= 0x01;
        }
        else
        {
            // 读到了0
            receive_byte &= ~0x01;
        }

        // 4. 下降沿
        SCK_LOW;
    }

    return receive_byte;
}