#include "adc1.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"

<<<<<<< HEAD
extern uint8_t isConvert;

=======
>>>>>>> ad50eb83a47416599e01a9ce237e478d27fe0c80
int main(void)
{
    // 串口初始化
    USART1_Init();

    // ADC1初始化
    ADC1_DoubleChannelDMAInit();
    // ADC1双通道开启测量
<<<<<<< HEAD
    uint16_t buffer[2] = {0};
=======
    uint16_t buffer[2] = { 0 };
>>>>>>> ad50eb83a47416599e01a9ce237e478d27fe0c80
    ADC1_DoubleChannelStart(buffer, 2);

    while (1)
    {
<<<<<<< HEAD
        printf("PC0 = %.2fV PC2 = %.2fV", buffer[0] * 3.3 / 4095, buffer[1] * 3.3 / 4095);
        Delay_s(1);
=======
        // printf("PC0 = %.2fV PC2 = %.2fV", buffer[0] * 3.3 / 4095, buffer[1] * 3.3 / 4095);
        // Delay_s(1);

// 检查DMA传输计数
        uint16_t dma_remain = DMA1->CNDTR;

        // 检查ADC状态
        uint32_t adc_sr = ADC1->SR;

        // 打印调试信息
        printf("DMA剩余:%d, ADC_SR:0x%02lX, B0:%d, B1:%d\n",
            dma_remain, adc_sr, buffer[0], buffer[1]);

        Delay_s(1);

>>>>>>> ad50eb83a47416599e01a9ce237e478d27fe0c80
    }
}
