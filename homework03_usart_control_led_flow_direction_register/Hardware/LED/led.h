#ifndef __LED_H__
#define __LED_H__

#include <stm32f10x.h>
#include <delay.h>

#define LED1 GPIO_ODR_ODR0
#define LED2 GPIO_ODR_ODR1
#define LED3 GPIO_ODR_ODR8

/**
 * @brief 初始化
 *
 */
void LED_Init(void);

/**
 * @brief 开灯
 *
 */
void LED_On(uint16_t led);

/**
 * @brief 关灯
 *
 */
void LED_Off(uint16_t led);

/**
 * @brief 灯翻转
 *
 * @param led
 */
void LED_Toggle(uint16_t led);

/**
 * @brief 点亮所有LED灯
 * 
 * @param dir 控制LED灯的方向
 */
void LED_On_All(uint8_t dir);

#endif