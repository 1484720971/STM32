#include "systick.h"
#include "led.h"

int main(void)
{
    // SysTick初始化
    Systick_Init();
    // LED初始化
    LED_Init();

    while (1)
    {
    }
}