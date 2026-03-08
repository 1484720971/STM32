#include "lcd.h"
#include "lcd_font.h"
#include "stdio.h"
#include "usart.h"

// ???
uint8_t duty = 10;
// ????????
uint8_t dutyFlag = 0;

/**
 * @brief LCD???
 *
 */
void LCD_Init(void)
{
    // 1. FSMC???
    FSMC_Init();
    // 2. LCD??
    LCD_Reset();
    // 3. LCD????
    LCD_Backlight_On();
    // 4. LCD????????
    LCD_RegConfig();
    // 5. TIM3???
    TIM3_PWMLcd_Init();
    // 6. KEY3???
    KEY3_Init();
    // 7. KEY1???
    KEY1_Init();
    // 8. KEY5???
    KEY5_Init();
}

/**
 * @brief LCD??
 *
 */
void LCD_Reset(void)
{
    // LCD?????
    GPIOG->ODR &= ~GPIO_ODR_ODR15;
    Delay_ms(100);
    GPIOG->ODR |= GPIO_ODR_ODR15;
    Delay_ms(100);
}

/**
 * @brief LCD????
 *
 */
void LCD_Backlight_On(void)
{
    GPIOB->ODR |= GPIO_ODR_ODR0;
}

/**
 * @brief LCD????
 *
 */
void LCD_Backlight_Off(void)
{
    GPIOB->ODR &= ~GPIO_ODR_ODR0;
}

/**
 * @brief LCD????????
 *
 */
void LCD_RegConfig(void)
{
    /* 1. ?????????TFT???????? ????????????? */
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

    /* 2. ?????????TFT??????????? */
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

    /* 4. ????1*/
    LCD_WriteCmd(0xC0);
    LCD_WriteData(0x11); /* ????? */
    LCD_WriteData(0x09); /* ????? */

    /* 5. ????2 */
    LCD_WriteCmd(0xC1);
    LCD_WriteData(0x02);
    LCD_WriteData(0x03);

    /* 6. VCOM?? */
    LCD_WriteCmd(0XC5);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x80);

    /* 7. Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    LCD_WriteCmd(0xB1);
    LCD_WriteData(0xB0);
    LCD_WriteData(0x11);

    /* 8.  Display Inversion Control (B4h) ???????????????*/
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

    /* 13. Sleep Out (11h) ?????? */
    LCD_WriteCmd(0x11);

    /* 14. ???????RGB */
    LCD_WriteCmd(0x36);
    LCD_WriteData(0x08);

    Delay_ms(120);

    /* 14. display on */
    LCD_WriteCmd(0x29);
}

/**
 * @brief LCD???
 *
 * @param cmd ??
 */
void LCD_WriteCmd(uint16_t cmd)
{
    *LCD_ADDRESS_CMD = cmd;
}

/**
 * @brief LCD???
 *
 * @param cmd ??
 */
void LCD_WriteData(uint16_t data)
{
    *LCD_ADDRESS_DATA = data;
}

/**
 * @brief LCD???
 *
 * @return uint16_t ??
 */
uint16_t LCD_ReadData(void)
{
    return *LCD_ADDRESS_DATA;
}

/**
 * @brief ??LCD?ID
 *
 * @return uint32_t LCD?ID
 */
uint32_t LCD_ReadID(void)
{
    uint32_t id = 0;

    // 1. ????ID???
    LCD_WriteCmd(0x04);
    // 2. ????????????
    LCD_ReadData();
    // 3. ??????????????
    for (uint8_t i = 0; i < 3; i++)
    {
        id <<= 8;
        id |= LCD_ReadData() & 0xFF;
    }
    return id;
}

/**
 * @brief ????
 *
 * @param color ?????
 */
void LCD_ClearAll(uint16_t color)
{
    // 1. ????
    LCD_SetArea(0, 0, LCD_WIDTH, LCD_HIGH);
    // 2. ????????
    LCD_WriteCmd(0x2C);
    // 3. ????????????????
    for (uint32_t i = 0; i < LCD_WIDTH * LCD_HIGH; i++)
    {
        LCD_WriteData(color);
    }
}

/**
 * @brief ????
 *
 * @param x ???
 * @param y ???
 * @param w ??
 * @param h ??
 */
void LCD_SetArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    // 1. ???
    // 1.1 ???????
    LCD_WriteCmd(0x2A);
    // 1.2 ??????????
    LCD_WriteData(x >> 8);
    // 1.2 ??????????
    LCD_WriteData(x & 0xFF);
    // 1.3 ??????????
    LCD_WriteData((x + w - 1) >> 8);
    // 1.4 ??????????
    LCD_WriteData((x + w - 1) & 0xFF);

    // 2. ???
    // 2.1 ???????
    LCD_WriteCmd(0x2B);
    // 2.2 ??????????
    LCD_WriteData(y >> 8);
    // 2.2 ??????????
    LCD_WriteData(y & 0xFF);
    // 2.3 ??????????
    LCD_WriteData((y + h - 1) >> 8);
    // 2.4 ??????????
    LCD_WriteData((y + h - 1) & 0xFF);
}

/**
 * @brief ?????????????
 *
 * @param x ?????
 * @param y ?????
 * @param height ??   ?? = ?? / 2
 * @param c ?????
 * @param fColor ?????
 * @param bColor ?????
 */
void LCD_WriteASCIIChar(uint16_t x, uint16_t y, uint16_t height, uint16_t c, uint16_t fColor, uint16_t bColor)
{
    // 1. ????
    LCD_SetArea(x, y, height / 2, height);

    // 2. ???????????
    LCD_WriteCmd(0x2C);

    // 3. ???????
    uint8_t index = c - ' ';

    // 4. ?????????????
    if (height == 12)
    {
        // 5. ??????????????
        for (uint8_t i = 0; i < height; i++)
        {
            // 6. ??????????
            uint8_t tempByte = ascii_1206[index][i];
            // 7. ??????????
            for (uint8_t j = 0; j < 6; j++)
            {
                if (tempByte & 0x01)
                {
                    // ????????????
                    LCD_WriteData(fColor);
                }
                else
                {
                    // ????????????
                    LCD_WriteData(bColor);
                }
                tempByte >>= 1;
            }
        }
    }
    else if (height == 16)
    {
        // 5. ??????????????
        for (uint8_t i = 0; i < height; i++)
        {
            // 6. ??????????
            uint8_t tempByte = ascii_1608[index][i];
            // 7. ??????????
            for (uint8_t j = 0; j < 8; j++)
            {
                if (tempByte & 0x01)
                {
                    // ????????????
                    LCD_WriteData(fColor);
                }
                else
                {
                    // ????????????
                    LCD_WriteData(bColor);
                }
                tempByte >>= 1;
            }
        }
    }
    else if (height == 24)
    {
        // 5. ??????????????
        for (uint8_t i = 0; i < height * 2; i++)
        {
            // 6. ??????????
            uint8_t tempByte = ascii_2412[index][i];
            // 7. ???????????8???4?
            uint8_t jCount = (i % 2) ? 4 : 8;
            // 8. ??????????
            for (uint8_t j = 0; j < jCount; j++)
            {
                if (tempByte & 0x01)
                {
                    // ????????????
                    LCD_WriteData(fColor);
                }
                else
                {
                    // ????????????
                    LCD_WriteData(bColor);
                }
                tempByte >>= 1;
            }
        }
    }
    else if (height == 32)
    {
        // 5. ??????????????
        for (uint8_t i = 0; i < height * 2; i++)
        {
            // 6. ??????????
            uint8_t tempByte = ascii_3216[index][i];
            // 7. ??????????
            for (uint8_t j = 0; j < 8; j++)
            {
                if (tempByte & 0x01)
                {
                    // ????????????
                    LCD_WriteData(fColor);
                }
                else
                {
                    // ????????????
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
 * @brief ???????????????????
 *
 * @param x ?????
 * @param y ?????
 * @param height ??   ?? = ?? / 2
 * @param str ?????????const?????
 * @param fColor ?????
 * @param bColor ?????
 */
void LCD_WriteASCIIString(uint16_t x, uint16_t y, uint16_t height, const char *str, uint16_t fColor, uint16_t bColor)
{
    // ??????????NULL??
    if (str == NULL)
    {
        return;
    }

    uint16_t first_x = x; // ??????x?????????

    // ???????????
    for (uint8_t i = 0; str[i] != '\0'; i++)
    {
        // ?????/??
        if (str[i] == '\n')
        {
            x = first_x;
            y += height;
            continue; // ????????
        }

        // ??????????
        if ((x + height / 2) > LCD_WIDTH)
        {
            x = first_x;
            y += height;
        }

        // ????
        LCD_WriteASCIIChar(x, y, height, str[i], fColor, bColor);
        // ??x??
        x += (height / 2);
    }
}

/**
 * @brief ??????????????
 *
 * @param x ?????
 * @param y ?????
 * @param height ?? 32 ??????????32
 * @param fColor ?????
 * @param bColor ?????
 */
void LCD_WriteChineseChar(uint16_t x, uint16_t y, uint16_t height, uint8_t index, uint16_t fColor, uint16_t bColor)
{
    // 1. ????
    LCD_SetArea(x, y, height, height);

    // 2. ???????????
    LCD_WriteCmd(0x2C);

    // 3. ???????????
    for (uint8_t i = 0; i < height * 4; i++)
    {
        // 4. ??????????
        uint8_t tempByte = chinese[index][i];

        // 5. ???????????
        for (uint8_t j = 0; j < 8; j++)
        {
            if (tempByte & 0x01)
            {
                // ????????????
                LCD_WriteData(fColor);
            }
            else
            {
                // ????????????
                LCD_WriteData(bColor);
            }
            tempByte >>= 1;
        }
    }
}

/**
 * @brief ??????????
 *
 * @param x ?????
 * @param y ?????
 * @param width ????
 * @param height ????
 */
void LCD_DisplayImg(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    // 1. ????
    LCD_SetArea(x, y, width, height);

    // 2. ???????????
    LCD_WriteCmd(0x2C);

    // 3. ??????
    for (uint32_t i = 0; i < width * height * 2; i += 2)
    {
        // 4. ????16??color
        uint16_t pointColor = gImage_test[i] + (gImage_test[i + 1] << 8);

        // 5. ??????????
        LCD_WriteData(pointColor);
    }
}

/**
 * @brief ???
 *
 * @param x ?????
 * @param y ?????
 * @param width ??
 * @param color ??
 */
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
    // 1. ????
    LCD_SetArea(x, y, width, width);

    // 2. ???????????
    LCD_WriteCmd(0x2C);

    // 3. ???
    for (uint16_t i = 0; i < width * width; i++)
    {
        LCD_WriteData(color);
    }
}

/**
 * @brief ????????????????????????????
 */
static void LCD_DrawHLineFast(int32_t x1, int32_t x2, int32_t y, uint16_t color)
{
    if (y < 0 || y >= LCD_HIGH)
    {
        return;
    }

    if (x1 > x2)
    {
        int32_t temp = x1;
        x1 = x2;
        x2 = temp;
    }

    if (x2 < 0 || x1 >= LCD_WIDTH)
    {
        return;
    }

    if (x1 < 0)
    {
        x1 = 0;
    }
    if (x2 >= LCD_WIDTH)
    {
        x2 = LCD_WIDTH - 1;
    }

    uint16_t lineWidth = (uint16_t)(x2 - x1 + 1);
    LCD_SetArea((uint16_t)x1, (uint16_t)y, lineWidth, 1);
    LCD_WriteCmd(0x2C);
    for (uint16_t i = 0; i < lineWidth; i++)
    {
        LCD_WriteData(color);
    }
}

/**
 * @brief ???
 *
 * @param x1 ?????
 * @param y1 ?????
 * @param x2 ?????
 * @param y2 ?????
 * @param width ??
 * @param color ??
 */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, uint16_t color)
{
    /*
        ?????
            1. ???????????
            2. ???????????x?y??
            3. ???????????????????
        ?????
            ?????y = kx + b
            k???   k =  (y2 - y1) / (x2 - x1)
               - ?0
               - 0???
               - ??????
            b???????x,y????
               - b = y1 - k * x1
    */

    // ?????????????
    if (x1 == x2)
    {
        for (uint16_t y = y1; y < y2; y++)
        {
            LCD_DrawPoint(x1, y, width, color);
        }
        return;
    }

    // ????
    double k = 1.0 * ((y2 - y1) / (x2 - x1));
    // ????
    double b = y1 - k * x1;

    for (uint16_t x = x1; x < x2; x++)
    {
        // ??y
        uint16_t y = (uint16_t)(k * x + b);
        LCD_DrawPoint(x, y, width, color);
    }
}

/**
 * @brief ????LCD????
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

        // ????????
        dutyFlag = 0;
    }
}

/**
 * @brief ???
 *
 * @param x1 ?????
 * @param y1 ?????
 * @param x2 ?????
 * @param y2 ?????
 * @param width ??
 * @param color ??
 */
void LCD_DrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, width, color);
    LCD_DrawLine(x1, y1, x1, y2, width, color);
    LCD_DrawLine(x2, y1, x2, y2, width, color);
    LCD_DrawLine(x1, y2, x2, y2, width, color);
}

/**
 * @brief ???
 *
 * @param xCenter ?????
 * @param yCenter ?????
 * @param r ??
 * @param color ??
 */
void LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width, uint16_t color)
{
    /*
        ?????
            1. ????????????
            2. ??????????????????
        ????????
            x = xCenter + r * cos?
            y = yCenter + r * sin?
        ??????
            360° = 2? ---???---> ? = ?  ????
            360°/? = 2?/?
            ? = (? * ?) / ?
    */

    // ????theta?????
    for (uint16_t theta = 0; theta < 360; theta++)
    {
        uint16_t x = xCenter + r * cos(theta * 3.14 / 180);
        uint16_t y = yCenter + r * sin(theta * 3.14 / 180);
        LCD_DrawPoint(x, y, width, color);
    }
}

/**
 * @brief ??????
 *
 * @param xCenter ?????
 * @param yCenter ?????
 * @param r ??
 * @param width ??
 * @param color ??
 */
void LCD_DrawCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width, uint16_t color)
{
    for (uint16_t theta = 0; theta <= 90; theta++)
    {
        uint16_t detal_x = r * cos(theta * 3.14 / 180);
        uint16_t detal_y = r * sin(theta * 3.14 / 180);

        // ????
        uint16_t x = xCenter + detal_x;
        uint16_t y = yCenter + detal_y;
        LCD_DrawPoint(x, y, width, color);

        // ????
        x = xCenter - detal_x;
        y = yCenter + detal_y;
        LCD_DrawPoint(x, y, width, color);

        // ????
        x = xCenter - detal_x;
        y = yCenter - detal_y;
        LCD_DrawPoint(x, y, width, color);

        // ????
        x = xCenter + detal_x;
        y = yCenter - detal_y;
        LCD_DrawPoint(x, y, width, color);
    }
}

/**
 * @brief ?????
 *
 * @param xCenter ?????
 * @param yCenter ?????
 * @param r ??
 * @param width ??
 * @param color ??
 */
void LCD_DrawFillCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width, uint16_t color)
{
    for (uint8_t i = 0; i < r; i++)
    {
        LCD_DrawCircle(xCenter, yCenter, i, width, color);
    }
}

/**
 * @brief ????????
 *
 * @param xCenter ?????
 * @param yCenter ?????
 * @param r ??
 * @param width ??
 * @param color ??
 */
void LCD_DrawFillCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width, uint16_t color)
{
    if (width == 0)
    {
        width = 1;
    }

    uint16_t radius = r + (width - 1) / 2;
    if (radius == 0)
    {
        LCD_DrawPoint(xCenter, yCenter, 1, color);
        return;
    }

    int32_t x = radius;
    int32_t y = 0;
    int32_t d = 1 - (int32_t)radius;

    while (x >= y)
    {
        int32_t cx = (int32_t)xCenter;
        int32_t cy = (int32_t)yCenter;

        // ??????????y=0 ??????
        LCD_DrawHLineFast(cx - x, cx + x, cy + y, color);
        if (y != 0)
        {
            LCD_DrawHLineFast(cx - x, cx + x, cy - y, color);
        }

        // ?????x==y ??????????????????
        if (x != y)
        {
            LCD_DrawHLineFast(cx - y, cx + y, cy + x, color);
            if (x != 0)
            {
                LCD_DrawHLineFast(cx - y, cx + y, cy - x, color);
            }
        }

        y++;
        if (d < 0)
        {
            d += (2 * y + 1);
        }
        else
        {
            x--;
            d += (2 * (y - x) + 1);
        }
    }
}

// ??????
void EXTI9_5_IRQHandler(void)
{
    // ???????9
    if (EXTI->PR & EXTI_PR_PR9)
    {
        // ???????
        EXTI->PR |= EXTI_PR_PR9;

        // ????
        Delay_ms(5);

        // ??????????????
        if (GPIOF->IDR & GPIO_IDR_IDR9)
        {
            // LED??
            LED_Toggle(LED2);
        }
    }

    // ???????7
    if (EXTI->PR & EXTI_PR_PR7)
    {
        // ???????
        EXTI->PR |= EXTI_PR_PR7;

        // ????
        Delay_ms(5);

        // ??????????????
        if (GPIOF->IDR & GPIO_IDR_IDR7)
        {
            // ?????
            duty++;
            // ??????????1
            dutyFlag = 1;
        }
    }
}

void EXTI15_10_IRQHandler(void)
{
    // ???????11
    if (EXTI->PR & EXTI_PR_PR11)
    {
        // ???????
        EXTI->PR |= EXTI_PR_PR11;

        // ????
        Delay_ms(5);

        // ??????????????
        if ((GPIOF->IDR & GPIO_IDR_IDR11) == 0)
        {
            // ?????
            duty--;
            // ??????????1
            dutyFlag = 1;
        }
    }
}
