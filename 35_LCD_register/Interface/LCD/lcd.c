#include "lcd.h"

#include "stdio.h"

// 占空比
uint8_t duty = 0;
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
            printf("Hello");

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