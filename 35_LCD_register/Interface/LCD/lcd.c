#include "lcd.h"
#include "lcd_font.h"
#include "stdio.h"
#include "usart.h"

// 占空比
uint8_t duty = 10;
// 占空比修改标志位
uint8_t dutyFlag = 0;

/**
 * @brief LCD初始化
 *
 */
void LCD_Init(void)
{
    // 1. FSMC初始化
    FSMC_Init();
    // 2. LCD复位
    LCD_Reset();
    // 3. LCD开启背光
    LCD_Backlight_On();
    // 4. LCD内置的初始化配置
    LCD_RegConfig();
    // 5. TIM3初始化
    TIM3_PWMLcd_Init();
    // 6. KEY3初始化
    KEY3_Init();
    // 7. KEY1初始化
    KEY1_Init();
    // 8. KEY5初始化
    KEY5_Init();
}

/**
 * @brief LCD复位
 *
 */
void LCD_Reset(void)
{
    // LCD低电平复位
    GPIOG->ODR &= ~GPIO_ODR_ODR15;
    Delay_ms(100);
    GPIOG->ODR |= GPIO_ODR_ODR15;
    Delay_ms(100);
}

/**
 * @brief LCD开启背光
 *
 */
void LCD_Backlight_On(void)
{
    GPIOB->ODR |= GPIO_ODR_ODR0;
}

/**
 * @brief LCD关闭背光
 *
 */
void LCD_Backlight_Off(void)
{
    GPIOB->ODR &= ~GPIO_ODR_ODR0;
}

/**
 * @brief LCD内置的初始化配置
 *
 */
void LCD_RegConfig(void)
{
    /* 1. 设置灰阶电压以调整TFT面板的伽马特性， 正校准。一般出厂就设置好了 */
    LCD_WriteCmd(0xE0);
    LCD_WriteData(0x00);
    LCD_WriteData(0x07);
    LCD_WriteData(0x10);
    LCD_WriteData(0x09);
    LCD_WriteData(0x17);
    LCD_WriteData(0x0B);
    LCD_WriteData(0x41);
    LCD_WriteData(0x89);
    LCD_WriteData(0x4B);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x18);
    LCD_WriteData(0x1B);
    LCD_WriteData(0x0F);

    /* 2. 设置灰阶电压以调整TFT面板的伽马特性，负校准 */
    LCD_WriteCmd(0XE1);
    LCD_WriteData(0x00);
    LCD_WriteData(0x17);
    LCD_WriteData(0x1A);
    LCD_WriteData(0x04);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x06);
    LCD_WriteData(0x2F);
    LCD_WriteData(0x45);
    LCD_WriteData(0x43);
    LCD_WriteData(0x02);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x09);
    LCD_WriteData(0x32);
    LCD_WriteData(0x36);
    LCD_WriteData(0x0F);

    /* 3.  Adjust Control 3 (F7h)  */
    /*LCD_WriteCmd(0XF7);
   LCD_WriteData(0xA9);
   LCD_WriteData(0x51);
   LCD_WriteData(0x2C);
   LCD_WriteData(0x82);*/
    /* DSI write DCS command, use loose packet RGB 666 */

    /* 4. 电源控制1*/
    LCD_WriteCmd(0xC0);
    LCD_WriteData(0x11); /* 正伽马电压 */
    LCD_WriteData(0x09); /* 负伽马电压 */

    /* 5. 电源控制2 */
    LCD_WriteCmd(0xC1);
    LCD_WriteData(0x02);
    LCD_WriteData(0x03);

    /* 6. VCOM控制 */
    LCD_WriteCmd(0XC5);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x80);

    /* 7. Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    LCD_WriteCmd(0xB1);
    LCD_WriteData(0xB0);
    LCD_WriteData(0x11);

    /* 8.  Display Inversion Control (B4h) （正负电压反转，减少电磁干扰）*/
    LCD_WriteCmd(0xB4);
    LCD_WriteData(0x02);

    /* 9.  Display Function Control (B6h)  */
    LCD_WriteCmd(0xB6);
    LCD_WriteData(0x0A);
    LCD_WriteData(0xA2);

    /* 10. Entry Mode Set (B7h)  */
    LCD_WriteCmd(0xB7);
    LCD_WriteData(0xc6);

    /* 11. HS Lanes Control (BEh) */
    LCD_WriteCmd(0xBE);
    LCD_WriteData(0x00);
    LCD_WriteData(0x04);

    /* 12.  Interface Pixel Format (3Ah) */
    LCD_WriteCmd(0x3A);
    LCD_WriteData(0x55); /* 0x55 : 16 bits/pixel  */

    /* 13. Sleep Out (11h) 关闭休眠模式 */
    LCD_WriteCmd(0x11);

    /* 14. 设置屏幕方向和RGB */
    LCD_WriteCmd(0x36);
    LCD_WriteData(0x08);

    Delay_ms(120);

    /* 14. display on */
    LCD_WriteCmd(0x29);
}

/**
 * @brief LCD写命令
 *
 * @param cmd 命令
 */
void LCD_WriteCmd(uint16_t cmd)
{
    *LCD_ADDRESS_CMD = cmd;
}

/**
 * @brief LCD写数据
 *
 * @param cmd 数据
 */
void LCD_WriteData(uint16_t data)
{
    *LCD_ADDRESS_DATA = data;
}

/**
 * @brief LCD读数据
 *
 * @return uint16_t 数据
 */
uint16_t LCD_ReadData(void)
{
    return *LCD_ADDRESS_DATA;
}

/**
 * @brief 读取LCD的ID
 *
 * @return uint32_t LCD的ID
 */
uint32_t LCD_ReadID(void)
{
    uint32_t id = 0;

    // 1. 发送读取ID的命令
    LCD_WriteCmd(0x04);
    // 2. 过度返回的第一个无效字节
    LCD_ReadData();
    // 3. 分别接收返回的三个单字节数据
    for (uint8_t i = 0; i < 3; i++)
    {
        id <<= 8;
        id |= LCD_ReadData() & 0xFF;
    }
    return id;
}

/**
 * @brief 清屏操作
 *
 * @param color 指定的颜色
 */
void LCD_ClearAll(uint16_t color)
{
    // 1. 设置范围
    LCD_SetArea(0, 0, LCD_WIDTH, LCD_HIGH);
    // 2. 发送设置颜色命令
    LCD_WriteCmd(0x2C);
    // 3. 设置颜色（每次只设置一个像素点）
    for (uint32_t i = 0; i < LCD_WIDTH * LCD_HIGH; i++)
    {
        LCD_WriteData(color);
    }
}

/**
 * @brief 设置范围
 *
 * @param x 横坐标
 * @param y 纵坐标
 * @param w 宽度
 * @param h 高度
 */
void LCD_SetArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    // 1. 设置列
    // 1.1 发送设置列命令
    LCD_WriteCmd(0x2A);
    // 1.2 发送列起始（高字节）
    LCD_WriteData(x >> 8);
    // 1.2 发送列起始（低字节）
    LCD_WriteData(x & 0xFF);
    // 1.3 发送列结束（高字节）
    LCD_WriteData((x + w - 1) >> 8);
    // 1.4 发送列结束（低字节）
    LCD_WriteData((x + w - 1) & 0xFF);

    // 2. 设置行
    // 2.1 发送设置行命令
    LCD_WriteCmd(0x2B);
    // 2.2 发送行起始（高字节）
    LCD_WriteData(y >> 8);
    // 2.2 发送行起始（低字节）
    LCD_WriteData(y & 0xFF);
    // 2.3 发送行结束（高字节）
    LCD_WriteData((y + h - 1) >> 8);
    // 2.4 发送行结束（低字节）
    LCD_WriteData((y + h - 1) & 0xFF);
}

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
void LCD_WriteASCIIChar(uint16_t x, uint16_t y, uint16_t height, uint16_t c, uint16_t fColor, uint16_t bColor)
{
    // 1. 设置范围
    LCD_SetArea(x, y, height / 2, height);

    // 2. 发送数据的命令
    LCD_WriteCmd(0x2C);

    // 3. 根据height（行数）判断应该使用哪个字模
    uint8_t index = c - ' ';
    if (height == 12 || height == 16)
    {
        // 遍历获取当前字符的每一个字节
        for (uint8_t i = 0; i < height; i++)
        {
            // 获取到对应的字节
            uint8_t tempByte = height == 12 ? ascii_1206[index][i] : ascii_1608[index][i];

            // 遍历当前字节的每一位
            for (uint8_t j = 0; j < 8; j++)
            {
                // 取到屏幕中最高位的像素点（最左边）
                if (tempByte & 0x01)
                {
                    // 设置当前像素点的前景颜色
                    LCD_WriteData(fColor);
                }
                else
                {
                    // 设置当前像素点的背景颜色
                    LCD_WriteData(bColor);
                }
                tempByte >>= 1;
            }
        }
    }
    else if (height == 24)
    {
        // 遍历获取当前字符的每一个字节
        for (uint8_t i = 0; i < height * 2; i++)
        {
            // 获取到对应的字节
            uint8_t tempByte = ascii_2412[index][i];
            // 判断当前字节应该使用全8位还是只使用低4位（每一行的第二个字节）
            uint8_t jCount = (i % 2) ? 4 : 8;

            // 遍历当前字节的每一位
            for (uint8_t j = 0; j < jCount; j++)
            {
                // 取到屏幕中最高位的像素点（最左边）
                if (tempByte & 0x01)
                {
                    // 设置当前像素点的前景颜色
                    LCD_WriteData(fColor);
                }
                else
                {
                    // 设置当前像素点的背景颜色
                    LCD_WriteData(bColor);
                }
                tempByte >>= 1;
            }
        }
    }
    else if (height == 32)
    {
        // 遍历获取当前字符的每一个字节
        for (uint8_t i = 0; i < height * 2; i++)
        {
            // 获取到对应的字节
            uint8_t tempByte = ascii_3216[index][i];

            // 遍历当前字节的每一位
            for (uint8_t j = 0; j < 8; j++)
            {
                // 取到屏幕中最高位的像素点（最左边）
                if (tempByte & 0x01)
                {
                    // 设置当前像素点的前景颜色
                    LCD_WriteData(fColor);
                }
                else
                {
                    // 设置当前像素点的背景颜色
                    LCD_WriteData(bColor);
                }
                tempByte >>= 1;
            }
        }
    }
    else
    {
    }
}

/**
 * @brief 按键控制LCD屏幕亮度
 *
 * @return uint16_t
 */
void LCD_Control_Backlight_level(void)
{
    if (dutyFlag == 1)
    {
        if (duty > 10)
        {
            duty = (duty == 11) ? 10 : 0;
        }

        TIM3_SetDuty(duty);

        // 清零占空比标志位
        dutyFlag = 0;
    }
}

// 中断服务函数
void EXTI9_5_IRQHandler(void)
{
    // 判断是否是中断9
    if (EXTI->PR & EXTI_PR_PR9)
    {
        // 清除中断标志位
        EXTI->PR |= EXTI_PR_PR9;

        // 延时消抖
        Delay_ms(5);

        // 判断这个按键是不是真的按下了
        if (GPIOF->IDR & GPIO_IDR_IDR9)
        {
            // LED翻转
            LED_Toggle(LED2);
        }
    }

    // 判断是否是中断7
    if (EXTI->PR & EXTI_PR_PR7)
    {
        // 清除中断标志位
        EXTI->PR |= EXTI_PR_PR7;

        // 延时消抖
        Delay_ms(5);

        // 判断这个按键是不是真的按下了
        if (GPIOF->IDR & GPIO_IDR_IDR7)
        {
            // 增加占空比
            duty++;
            // 占空比修改，标志位变1
            dutyFlag = 1;
        }
    }
}

void EXTI15_10_IRQHandler(void)
{
    // 判断是否是中断11
    if (EXTI->PR & EXTI_PR_PR11)
    {
        // 清除中断标志位
        EXTI->PR |= EXTI_PR_PR11;

        // 延时消抖
        Delay_ms(5);

        // 判断这个按键是不是真的按下了
        if ((GPIOF->IDR & GPIO_IDR_IDR11) == 0)
        {
            // 减少占空比
            duty--;
            // 占空比修改，标志位变1
            dutyFlag = 1;
        }
    }
}