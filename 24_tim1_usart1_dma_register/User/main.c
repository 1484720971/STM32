#include "delay.h"
#include "stdio.h"
#include "usart.h"
#include "usart1_dma.h"

int main(void)
{
    // 串口初始化
    USART1_Init();

    // 使能USART1的DMA发送功能
    USART1_EnableDMATransmit();
    // 使能USART1的DMA接收功能
    USART1_EnableDMAReceive();

    // 串口1DAM发送功能初始化
    DMA_USART1_TxInit();
    // 串口1DAM接收功能初始化
    DMA_USART1_RxInit();

    uint8_t *str = "Hello atguigu";

    /* --------------------------- DMA从SRAM向串口发送数据 --------------------------- */
    DMA_USART1_TransmitData(str, 13);
    Delay_s(1);
    // 在DMA_USART1_TransmitData函数中打开了DMA的使能，想要再次发送的话需要关闭使能，再次调用这个函数
    DMA1_Channel4->CCR &= ~DMA_CCR1_EN;

    DMA_USART1_TransmitData(str, 13);

    /* --------------------------- DMA从串口向SRAM发送数据 --------------------------- */
    uint8_t buffer[100] = {0};
    DMA_USART1_ReceiveData(buffer, 8);

    // 等待DMA1的通道5传输完成 - 缺点，会阻塞程序
    // uint16_t timeout = UINT16_MAX;
    // while ((DMA1->ISR & DMA_ISR_TCIF5) == 0 && timeout)
    // {
    //     timeout--;
    // }

    while (1)
    {
        // DMA非阻塞方式接收数据 - 中断
        // if (TrCompleteFlag == 1)
        // {
        //     printf("%s\n", buffer);
        //     TrCompleteFlag = 0;
        // }

        // DMA非阻塞方式接收数据 - 标志位
        if (DMA1->ISR & DMA_ISR_TCIF5)
        {
            printf("%s\n", buffer);

            // 清除DMA_ISR寄存器中的对应TCIF标志。
            DMA1->IFCR = DMA_IFCR_CTCIF5;
        }
        
        
    }
}
