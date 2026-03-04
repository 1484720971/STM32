#ifndef __LCD_H__
#define __LCD_H__

#include "fsmc_bank1_3_sram.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "tim3.h"

/* 宏定义地址表示发送命令/地址 */
#define LCD_ADDRESS_BASE 0x6C000000
#define LCD_ADDRESS_DATA (uint16_t *)LCD_ADDRESS_BASE
#define LCD_ADDRESS_CMD (uint16_t *)(LCD_ADDRESS_BASE + (1 << 11))

/* 宏定义页面宽高 */
#define LCD_WIDTH 320
#define LCD_HIGH 480

/* 宏定义颜色 */
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 // 棕色
#define BRRED 0XFC07 // 棕红色
#define GRAY 0X8430  // 灰色


/**
 * @brief LCD初始化
 * 
 */
void LCD_Init(void);

/**
 * @brief LCD复位
 * 
 */
void LCD_Reset(void);

/**
 * @brief LCD开启背光
 * 
 */
void LCD_Backlight_On(void);

/**
 * @brief LCD关闭背光
 * 
 */
void LCD_Backlight_Off(void);

/**
 * @brief LCD内置的初始化配置
 * 
 */
void LCD_RegConfig(void);

/**
 * @brief LCD写命令
 * 
 * @param cmd 命令
 */
void LCD_WriteCmd(uint16_t cmd);

/**
 * @brief LCD写数据
 * 
 * @param cmd 数据
 */
void LCD_WriteData(uint16_t data);

/**
 * @brief LCD读数据
 * 
 * @return uint16_t 数据
 */
uint16_t LCD_ReadData(void);

/**
 * @brief 按键控制LCD屏幕亮度
 * 
 * @return uint16_t 
 */
void LCD_Control_Backlight_level(void);

#endif /* __LCD_H__ */