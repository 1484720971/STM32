#include "app_key_lcd_brightness_ctrl.h"
#include "led.h"

int main(void)
{
    key_control_lcd_brightness_10level_Init();

    while (1)
    {
        key_control_lcd_brightness_10level();
    }
}
