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

    while (1)
    {
        // 按键控制LCD屏幕亮度
        LCD_Control_Backlight_level();
    }
}