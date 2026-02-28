#include "hard_spi.h"

/**
 * @brief SPI初始化
 *
 */
void SPI_Init(void)
{
    // 1. 使能时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // 2. GPIO初始化
    // 2.0 ODE置1 如果不做这个 转成通用推挽后，会立刻输出低电平
    GPIOC->ODR |= GPIO_ODR_ODR13;
    // 2.1 配置PC13为推挽输出
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13;
    // 2.2 配置PA5为复用推挽输出
    GPIOA->CRL |= GPIO_CRL_CNF5_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF5_0;
    GPIOA->CRL |= GPIO_CRL_MODE5;
    // 2.3 配置PA7为复用推挽输出
    GPIOA->CRL |= GPIO_CRL_CNF7_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF7_0;
    GPIOA->CRL |= GPIO_CRL_MODE7;
    // 2.4 配置PA6为浮空输入
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;
    GPIOA->CRL &= ~GPIO_CRL_MODE6;

    // 3. SPI初始化
    // 3.1 配置波特率,主频/2 = 36Mhz
    SPI1->CR1 &= ~SPI_CR1_BR;
    // 3.2 配置极性,空闲状态时,SCK保持低电平
    SPI1->CR1 &= ~SPI_CR1_CPOL;
    // 3.3 配置相位,数据采样从第一个时钟边沿开始
    SPI1->CR1 &= ~SPI_CR1_CPHA;
    // 3.4 配置数据帧格式,使用8位数据帧格式进行发送/接收
    SPI1->CR1 &= ~SPI_CR1_DFF;
    // 3.5 配置帧格式,先发送MSB(高位先行)
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    // 3.6 配置软件从模式管理,启用软件从设备管理
    // 如果需要NSS引脚工作在输入模式，硬件模式下，在整个数据帧传输期间应把NSS脚连接到高电平；在软件模式下，需设置SPI_CR1寄存器的SSM位和SSI位。如果NSS引脚工作在输出模式，则只需设置SSOE位
    // NSS工作在输入模式（CR2 SSOE 默认复位是0 0的意思就是NSS为输入模式）
    SPI1->CR1 |= SPI_CR1_SSM;
    // 3.7 配置内部从设备选择
    SPI1->CR1 |= SPI_CR1_SSI;
    // 3.8 配置主模式选择,配置为主模式
    SPI1->CR1 |= SPI_CR1_MSTR;
    // 3.9 使能SPI
    SPI1->CR1 |= SPI_CR1_SPE;
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
    uint32_t timeout = UINT32_MAX;
    // 1. 等待发送完成
    while ((SPI1->SR & SPI_SR_TXE) == 0 && timeout)
    {
        timeout--;
    }

    // 2. 发送数据
    SPI1->DR = byte;

    // 3. 等待接收完成
    timeout = UINT32_MAX;
    while ((SPI1->SR & SPI_SR_RXNE) == 0 && timeout)
    {
        timeout--;
    }

    // 4. 返回接收到的数据
    return SPI1->DR;
}