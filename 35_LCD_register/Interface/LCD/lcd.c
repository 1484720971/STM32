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

    // 2. 发送写入颜色数据的命令
    LCD_WriteCmd(0x2C);

    // 3. 显示字符的下标
    uint8_t index = c - ' ';

    // 4. 判断当前字符属于哪个字符表
    if (height == 12)
    {
        // 5. 遍历获取当前字符的每一个字节
        for (uint8_t i = 0; i < height; i++)
        {
            // 6. 获取当前的字符的字节
            uint8_t tempByte = ascii_1206[index][i];
            // 7. 遍历当前字节的每一位
            for (uint8_t j = 0; j < 6; j++)
            {
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
    else if (height == 16)
    {
        // 5. 遍历获取当前字符的每一个字节
        for (uint8_t i = 0; i < height; i++)
        {
            // 6. 获取当前的字符的字节
            uint8_t tempByte = ascii_1608[index][i];
            // 7. 遍历当前字节的每一位
            for (uint8_t j = 0; j < 8; j++)
            {
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
        // 5. 遍历获取当前字符的每一个字节
        for (uint8_t i = 0; i < height * 2; i++)
        {
            // 6. 获取当前的字符的字节
            uint8_t tempByte = ascii_2412[index][i];
            // 7. 判断当前字节应该使用全8位还是4位
            uint8_t jCount = (i % 2) ? 4 : 8;
            // 8. 遍历当前字节的每一位
            for (uint8_t j = 0; j < jCount; j++)
            {
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
        // 5. 遍历获取当前字符的每一个字节
        for (uint8_t i = 0; i < height * 2; i++)
        {
            // 6. 获取当前的字符的字节
            uint8_t tempByte = ascii_3216[index][i];
            // 7. 遍历当前字节的每一位
            for (uint8_t j = 0; j < 8; j++)
            {
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
 * @brief 在指定的位置显示指定的字符串（优化版）
 *
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param height 高度   宽度 = 高度 / 2
 * @param str 指定的字符串指针（const保护只读）
 * @param fColor 字符的颜色
 * @param bColor 背景的颜色
 */
void LCD_WriteASCIIString(uint16_t x, uint16_t y, uint16_t height, const char* str, uint16_t fColor, uint16_t bColor)
{
    // 空指针保护，避免传入NULL崩溃
    if (str == NULL)
    {
        return;
    }

    uint16_t first_x = x; // 保存原始起始x，换行后回到该位置

    // 变量字符串中的每个字符
    for (uint8_t i = 0; str[i] != '\0'; i++)
    {
        // 先处理换行/越界
        if (str[i] == '\n')
        {
            x = first_x;
            y += height;
            continue; // 跳过换行符的显示
        }

        // 检查是否超出屏幕宽度
        if ((x + height / 2) > LCD_WIDTH)
        {
            x = first_x;
            y += height;
        }

        // 显示字符
        LCD_WriteASCIIChar(x, y, height, str[i], fColor, bColor);
        // 偏移x坐标
        x += (height / 2);
    }
}

/**
 * @brief 在指定的位置显示数组中的中文
 *
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param height 高度 32 数组中的中文字符都是32
 * @param fColor 字符的颜色
 * @param bColor 背景的颜色
 */
void LCD_WriteChineseChar(uint16_t x, uint16_t y, uint16_t height, uint8_t index, uint16_t fColor, uint16_t bColor)
{
    // 1. 设置范围
    LCD_SetArea(x, y, height, height);

    // 2. 发送写入颜色数据的命令
    LCD_WriteCmd(0x2C);

    // 3. 遍历中文字符串的每一行
    for (uint8_t i = 0; i < height * 4; i++)
    {
        // 4. 获取当前的字符的字节
        uint8_t tempByte = chinese[index][i];

        // 5. 遍历当字符字节的每一位
        for (uint8_t j = 0; j < 8; j++)
        {
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

/**
 * @brief 在指定的位置显示图片
 *
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param width 图片的宽
 * @param height 图片的高
 */
void LCD_DisplayImg(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    // 1. 设置范围
    LCD_SetArea(x, y, width, height);

    // 2. 发送写入颜色数据的命令
    LCD_WriteCmd(0x2C);

    // 3. 遍历图片数组
    for (uint32_t i = 0; i < width * height * 2; i += 2)
    {
        // 4. 拼接一个16位的color
        uint16_t pointColor = gImage_test[i] + (gImage_test[i + 1] << 8);

        // 5. 设置当前像素点的颜色
        LCD_WriteData(pointColor);
    }
}

/**
 * @brief 绘制点
 *
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param width 宽度
 * @param color 颜色
 */
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
    // 1. 设置范围
    LCD_SetArea(x, y, width, width);

    // 2. 发送写入颜色数据的命令
    LCD_WriteCmd(0x2C);

    // 3. 绘制点
    for (uint16_t i = 0; i < width * width; i++)
    {
        LCD_WriteData(color);
    }
}

/**
 * @brief 蹇€熺粯鍒舵按骞崇嚎锛堣嚜鍔ㄨ鍓埌灞忓箷鑼冨洿锛?
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
 * @brief 绘制线
 *
 * @param x1 起始横坐标
 * @param y1 起始纵坐标
 * @param x2 结束横坐标
 * @param y2 结束纵坐标
 * @param width 线宽
 * @param color 颜色
 */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, uint16_t color)
{
    /*
        思路分析：
            1. 已知条件：两个点的坐标
            2. 需要求出：线上每个点的x，y坐标
            3. 用已经封装好的绘制点的功能函数依次绘制
        解析几何：
            直线方程：y = kx + b
            k：斜率   k =  (y2 - y1) / (x2 - x1)
               - 非0
               - 0：横线
               - 无穷大：竖线
            b：截距，直线和x,y轴的交点
               - b = y1 - k * x1
    */

    // 判断斜率是否是无穷大，竖线
    if (x1 == x2)
    {
        for (uint16_t y = y1; y < y2; y++)
        {
            LCD_DrawPoint(x1, y, width, color);
        }
        return;
    }

    // 计算斜率
    double k = 1.0 * ((y2 - y1) / (x2 - x1));
    // 计算截距
    double b = y1 - k * x1;

    for (uint16_t x = x1; x < x2; x++)
    {
        // 计算y
        uint16_t y = (uint16_t)(k * x + b);
        LCD_DrawPoint(x, y, width, color);
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
void LCD_DrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, width, color);
    LCD_DrawLine(x1, y1, x1, y2, width, color);
    LCD_DrawLine(x2, y1, x2, y2, width, color);
    LCD_DrawLine(x1, y2, x2, y2, width, color);
}

/**
 * @brief 绘制圆
 *
 * @param xCenter 圆心横坐标
 * @param yCenter 圆心纵坐标
 * @param r 半径
 * @param color 颜色
 */
void LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width, uint16_t color)
{
    /*
        思路分析：
            1. 已知条件：圆心坐标，半径
            2. 需要求出：园上每个点的坐标，依次绘制
        极坐标三角函数：
            x = xCenter + r * cosθ
            y = yCenter + r * sinθ
        弧度转角度：
            360° = 2π ---等比例---> θ = α  求弧度α
            360°/θ = 2π/α
            α = (θ * π) / θ
    */

    // 循环变量theta相当于θ角
    for (uint16_t theta = 0; theta < 360; theta++)
    {
        uint16_t x = xCenter + r * cos(theta * 3.14 / 180);
        uint16_t y = yCenter + r * sin(theta * 3.14 / 180);
        LCD_DrawPoint(x, y, width, color);
    }
}

/**
 * @brief 绘制圆升级版
 *
 * @param xCenter 圆心横坐标
 * @param yCenter 圆心纵坐标
 * @param r 半径
 * @param width 线宽
 * @param color 颜色
 */
void LCD_DrawCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width, uint16_t color)
{
    for (uint16_t theta = 0; theta <= 90; theta++)
    {
        uint16_t detal_x = r * cos(theta * 3.14 / 180);
        uint16_t detal_y = r * sin(theta * 3.14 / 180);

        // 第一象限
        uint16_t x = xCenter + detal_x;
        uint16_t y = yCenter + detal_y;
        LCD_DrawPoint(x, y, width, color);

        // 第二象限
        x = xCenter - detal_x;
        y = yCenter + detal_y;
        LCD_DrawPoint(x, y, width, color);

        // 第三象限
        x = xCenter - detal_x;
        y = yCenter - detal_y;
        LCD_DrawPoint(x, y, width, color);

        // 第四象限
        x = xCenter + detal_x;
        y = yCenter - detal_y;
        LCD_DrawPoint(x, y, width, color);
    }
}

/**
 * @brief 绘制实心圆
 *
 * @param xCenter 圆心横坐标
 * @param yCenter 圆心纵坐标
 * @param r 半径
 * @param width 线宽
 * @param color 颜色
 */
void LCD_DrawFillCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t width, uint16_t color)
{
    for (uint8_t i = 0; i < r; i++)
    {
        LCD_DrawCircle(xCenter, yCenter, i, width, color);
    }
}

/**
 * @brief 绘制实心圆升级版
 *
 * @param xCenter 圆心横坐标
 * @param yCenter 圆心纵坐标
 * @param r 半径
 * @param width 线宽
 * @param color 颜色
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

        // 主扫描线：始终绘制，y=0 时只绘制一次
        LCD_DrawHLineFast(cx - x, cx + x, cy + y, color);
        if (y != 0)
        {
            LCD_DrawHLineFast(cx - x, cx + x, cy - y, color);
        }

        // 次扫描线：x==y 时与主扫描线重合，跳过以减少总线开销
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