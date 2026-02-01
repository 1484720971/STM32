#ifndef __APP_KEY_LCD_BRIGHTNESS_CTRL_H__
#define __APP_KEY_LCD_BRIGHTNESS_CTRL_H__

#include "key.h"
#include "tim3.h"

// 占空比
extern uint8_t duty;
// 占空比修改标志位
extern uint8_t dutyFlag;

/**
 * @brief 初始化按键控制LCD屏幕亮度
 *
 */
void key_control_lcd_brightness_10level_Init();

/**
 * @brief 按键控制LCD屏幕亮度
 * 
 */
void key_control_lcd_brightness_10level(void);

#endif /* __APP_KEY_LCD_BRIGHTNESS_CTRL_H__ */