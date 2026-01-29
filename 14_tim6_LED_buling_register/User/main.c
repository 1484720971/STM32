#include "led.h"
#include "tim6.h"
#include "usart.h"
#include "systick.h"
#include "stdio.h"

int main(void)
{
    // USART1初始化
    USART1_Init();
    // LED初始化
    LED_Init();

    printf("--------------Start--------------\n");

    // SysTick初始化
    Systick_Init();
    // TIM6初始化
    TIM6_Init();


    while (1)
    {

    }
}
