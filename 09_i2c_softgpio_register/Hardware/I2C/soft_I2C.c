#include "soft_I2C.h"

/**
 * @brief I2C初始化
 * 
 */
void I2C_Init(void)
{
    // 1. 初始化GPIO
    // (1) 使能GPIO时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // (2) 下面要设置为开漏模式，所以必须在设置模式之前，来设置默认输出电平。确保初始化时，不会发生额外的总线占用
    GPIOB->ODR |= GPIO_ODR_ODR10;
    GPIOB->ODR |= GPIO_ODR_ODR11;

    // (3) 配置工作模式 - I2C必须用开漏模式，防止短路
    // PB10 - 通用开漏输出 - SCL
    GPIOB->CRH |= GPIO_CRH_MODE10;
    GPIOB->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOB->CRH |= GPIO_CRH_CNF10_0;
    // PB11 - 通用开漏输出 - SDA
    GPIOB->CRH |= GPIO_CRH_MODE11;
    GPIOB->CRH &= ~GPIO_CRH_CNF11_1;
    GPIOB->CRH |= GPIO_CRH_CNF11_0;
}


/**
 * @brief I2C起始信号
 * 通讯速度为100kbit/s  时钟周期为10us
 * 
 * 约定我们需要确保每一个函数结束时的状态
 * 应该确保 时钟线是低电平（控制权在主设备手里） SDA是高电平
 */
void I2C_Start(void)
{
    SCL_HIGH;
    SDA_HIGH;
    Delay_us(2);
    SDA_LOW;
    Delay_us(3);
    SCL_LOW;
    Delay_us(5);
    SDA_HIGH;
}

/**
 * @brief I2C停止信号
 * 
 */
void I2C_Stop(void)
{
    SDA_LOW;
    SCL_HIGH;
    Delay_us(2);
    SDA_HIGH;
    Delay_us(3);
}