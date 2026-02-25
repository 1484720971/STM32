#include "led.h"

/**
 * @brief 初始化
 * 
 */
void LED_Init(void)
{
    // 1. 使能GPIOA的时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. 设置PA0、PA1、PA8的工作模式推挽输出
    // PA0
    GPIOA->CRL &= ~GPIO_CRL_CNF0;
    GPIOA->CRL |= GPIO_CRL_MODE0;
    // PA1
    GPIOA->CRL &= ~GPIO_CRL_CNF1;
    GPIOA->CRL |= GPIO_CRL_MODE1;
    // PA8
    GPIOA->CRH &= ~GPIO_CRH_CNF8;
    GPIOA->CRH |= GPIO_CRH_MODE8;

    // 3. 设置为默认关灯
    LED_Off(LED1 | LED2 | LED3);
}

/**
 * @brief 开灯
 * 
 */
void LED_On(uint16_t led)
{
    GPIOA->ODR &= ~led;
}

/**
 * @brief 关灯
 * 
 */
void LED_Off(uint16_t led)
{
    GPIOA->ODR |= led;
}

/**
 * @brief 灯翻转
 * 
 * @param led 
 */
void LED_Toggle(uint16_t led)
{
    GPIOA->ODR ^= led;
}