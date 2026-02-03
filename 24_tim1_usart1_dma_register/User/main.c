#include "stdio.h"
#include "usart.h"
#include "usart1_dma.h"

int main(void)
{
    // 串口初始化
    USART1_Init();
    // 使能USART1的DMA发送功能
    USART1_EnableDMATransmit();

    // DMA初始化
    DMA_USART1_TxInit();

    uint8_t *str = "Hello atguigu";

    DMA_USART1_TransmitData(str, 13);

    while (1)
    {
    }
}
