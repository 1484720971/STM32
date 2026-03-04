#include "lcd.h"
#include "usart.h"

int main(void)
{
    LCD_Init();
    USART1_Init();

    while (1)
    {
        LCD_Control_Backlight_level();
    }
}