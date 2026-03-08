#include "lcd.h"
#include "stdio.h"
#include "usart.h"

int main(void)
{
    LCD_Init();
    USART1_Init();

    /* ------------ 打印LCD的ID值 ------------ */
    printf("ID = %#x\n", LCD_ReadID());

    /* ------------ 清屏（指定颜色） ------------ */
    LCD_ClearAll(BLUE);

    /* ------------ 显示单个字符 ------------ */
    LCD_WriteASCIIChar(10, 20, 12, 'A', WHITE, BLACK);
    LCD_WriteASCIIChar(10, 36, 16, 'A', BRED, GRED);
    LCD_WriteASCIIChar(10, 56, 24, 'A', GBLUE, RED);
    LCD_WriteASCIIChar(10, 84, 32, 'A', MAGENTA, GREEN);

    /* ------------ 显示英文字符串 ------------ */
    LCD_WriteASCIIString(10, 120, 32, "ABCDEFGHIJKLM\nNOPQRSTUVWXYZ", CYAN, YELLOW);
    LCD_WriteASCIIString(10, 188, 32, "atguitu \n hello todat is 2025/4/26", CYAN, YELLOW);

    /* ------------ 显示中文字符 ------------ */
    LCD_WriteChineseChar(10, 288, 32, 0, BROWN, RED);
    LCD_WriteChineseChar(10, 324, 32, 1, BROWN, RED);
    LCD_WriteChineseChar(10, 360, 32, 2, BROWN, RED);

    /* ------------ 显示图片 ------------ */
    LCD_DisplayImg(45, 20, 240, 74);
    // LCD_DisplayImg(0, 0, 320, 480);  // 美女图片  需要将lcd_font.h中的条件编译改为0

    /* ------------ 显示点 ------------ */
    LCD_DrawPoint(47, 288, 4, GRAY);

    /* ------------ 显示线 ------------ */
    LCD_DrawLine(300, 20, 300, 100, 4, RED);   //  竖线
    LCD_DrawLine(45, 104, 285, 104, 4, GREEN); //  横线
    LCD_DrawLine(250, 120, 285, 200, 4, GREEN); //  斜线

    /* ------------ 显示矩形 ------------ */
    LCD_DrawRect(55, 288, 250, 320, 4, WHITE);

    /* ------------ 显示圆 ------------ */
    LCD_DrawCircle(80, 360, 30, 3, BLACK);

    /* ------------ 显示圆升级版 ------------ */
    LCD_DrawCircle_Pro(80, 360, 50, 3, GREEN);

    /* ------------ 显示实心圆 ------------ */
    LCD_DrawFillCircle(100, 380, 50, 1, RED);

    /* ------------ 显示实心圆升级版 ------------ */
    LCD_DrawFillCircle_Pro(200, 380, 50, 1, RED);

    while (1)
    {
        // 按键控制LCD屏幕亮度
        LCD_Control_Backlight_level();
    }
}