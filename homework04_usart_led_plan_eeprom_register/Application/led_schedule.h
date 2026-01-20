#ifndef __LED_SCHEDULE_H__
#define __LED_SCHEDULE_H__

#include "usart.h"
#include "led.h"
#include "M24C02.h"
#include <stdio.h>

/**
 * @brief 点灯计划初始化
 * 
 */
void led_schedule_init(void);

/**
 * @brief 点灯计划 - 接收&存储数据
 * 
 */
uint8_t led_schedule_recv_plan(uint8_t *plan_buf, uint8_t recv_len);

/**
 * @brief 点灯计划 - 根据EEPROM中的指令点亮LED
 * 
 */
void led_schedule_run_cycle(void);

#endif /* __LED_SCHEDULE_H__ */