#ifndef __LCD_H__
#define __LCD_H__

#include "delay.h"
#include "fsmc_bank1_3_sram.h"
#include "key.h"
#include "led.h"
#include "tim3.h"
#include "math.h"

/* 宏定义地址表示发送命令/地址 */
#define LCD_ADDRESS_BASE 0x6C000000
#define LCD_ADDRESS_CMD (uint16_t *)LCD_ADDRESS_BASE
#define LCD_ADDRESS_DATA (uint16_t *)(LCD_ADDRESS_BASE + (1 << 11))

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
#define GRAY 0X8430  // 灰色

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
 * @brief 读取LCD的ID
 *
 * @return uint32_t LCD的ID
 */
uint32_t LCD_ReadID(void);

/**
 * @brief 清屏操作
 *
 * @param color 指定的颜色
 */
void LCD_ClearAll(uint16_t color);

/**
 * @brief 设置范围
 *
 * @param x 横坐标
 * @param y 纵坐标
 * @param w 宽度
 * @param h 高度
 */
void LCD_SetArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

/**
 * @brief 按键控制LCD屏幕亮度
 *
 * @return uint16_t
 */
void LCD_Control_Backlight_level(void);

/**
 * @brief 在指定的位置显示指定的字符
 *
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param height 高度   宽度 = 高度 / 2
 * @param c 指定的字符
 * @param fColor 字符的颜色
 * @param bColor 背景的颜色
 */
void LCD_WriteASCIIChar(uint16_t x, uint16_t y, uint16_t height, uint16_t c, uint16_t fColor, uint16_t bColor);

/**
 * @brief 在指定的位置显示指定的字符串
 *
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param height 高度   宽度 = 高度 / 2
 * @param str 指定的字符串指针
 * @param fColor 字符的颜色
 * @param bColor 背景的颜色
 */
void LCD_WriteASCIIString(uint16_t x, uint16_t y, uint16_t height, const char *str, uint16_t fColor, uint16_t bColor);

/**
 * @brief 在指定的位置显示数组中的中文
 *
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param height 高度 32 数组中的中文字符都是32
 * @param index 下标
 * @param fColor 字符的颜色
 * @param bColor 背景的颜色
 */
void LCD_WriteChineseChar(uint16_t x, uint16_t y, uint16_t height, uint8_t index, uint16_t fColor, uint16_t bColor);

/**
 * @brief 在指定的位置显示图片
 *
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param width 图片的宽
 * @param height 图片的高
 */
void LCD_DisplayImg(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief 绘制点
 *
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param width 宽度
 * @param color 颜色
 */
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t width, uint16_t color);

/**
 * @brief 绘制线
 *
 * @param x1 起始横坐标
 * @param y1 起始纵坐标
 * @param x2 结束横坐标
 * @param y2 结束纵坐标
 * @param width 线宽
 * @param color 颜色
 */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, uint16_t color);

/**
 * @brief 绘制线
 *
 * @param x1 起始横坐标
 * @param y1 起始纵坐标
 * @param x2 结束横坐标
 * @param y2 结束纵坐标
 * @param width 线宽
 * @param color 颜色
 */
void LCD_DrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, uint16_t color);

/**
 * @brief 绘制圆
 * 
 * @param xCenter 圆心横坐标
 * @param yCenter 圆心纵坐标
 * @param r 半径
 * @param width 线宽
 * @param color 颜色
 */
void LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width,uint16_t color);

/**
 * @brief 绘制圆
 * 
 * @param xCenter 圆心横坐标
 * @param yCenter 圆心纵坐标
 * @param r 半径
 * @param width 线宽
 * @param color 颜色
 */
void LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width,uint16_t color);

/**
 * @brief 绘制圆升级版
 * 
 * @param xCenter 圆心横坐标
 * @param yCenter 圆心纵坐标
 * @param r 半径
 * @param width 线宽
 * @param color 颜色
 */
void LCD_DrawCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width,uint16_t color);

/**
 * @brief 绘制实心圆
 * 
 * @param xCenter 圆心横坐标
 * @param yCenter 圆心纵坐标
 * @param r 半径
 * @param width 线宽
 * @param color 颜色
 */
void LCD_DrawFillCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width,uint16_t color);

/**
 * @brief 绘制实心圆升级版
 * 
 * @param xCenter 圆心横坐标
 * @param yCenter 圆心纵坐标
 * @param r 半径
 * @param width 线宽
 * @param color 颜色
 */
void LCD_DrawFillCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width,uint16_t color);

#endif /* __LCD_H__ */
