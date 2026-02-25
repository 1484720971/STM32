#include "adc1.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"

extern uint8_t isConvert;

int main(void)
{
    // 串口初始化
    USART1_Init();

    // ADC1初始化
    ADC1_DoubleChannelDMAInit();
    // ADC1双通道开启测量
    uint16_t buffer[2] = {0};
    ADC1_DoubleChannelStart(buffer, 2);

    while (1)
    {
        printf("PC0 = %.2fV PC2 = %.2fV", buffer[0] * 3.3 / 4095, buffer[1] * 3.3 / 4095);
        Delay_s(1);
    }
}
