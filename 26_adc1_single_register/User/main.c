#include "adc1.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"

int main(void)
{
    // 串口初始化
    USART1_Init();

    // ADC1初始化
    ADC1_SingleChannelInit();

    while (1)
    {
        printf("V = %.2fV", ADC1_SingleReadV());
        Delay_s(1);
    }
}
