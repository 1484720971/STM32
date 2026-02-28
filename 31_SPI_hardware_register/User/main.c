#include "W25Q32.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"

int main(void)
{
    USART1_Init();
    W25Q32_Init();

    uint32_t id = W25Q32_Read_JEDEC_ID();

    printf("ManuFacturerID: 0x%X\n", (id & 0xFF0000) >> 16);
    printf("MemoryTypeID: 0x%X\n", (id & 0xFF00) >> 8);
    printf("CapacityID: 0x%X\n", id & 0xFF);

    // W25Q32写使能
    W25Q32_WriteEnable();

    // 向W25Q32写入数据
    W25Q32_PageWrite(0x00, 0x02, 0x00, 0x00, "1234", 4);

    // 读取W25Q32中的数据
    uint8_t buffer[10] = {0};
    W25Q32_ReadData(0x00, 0x02, 0x00, 0x00, buffer, 6);
    printf("%s\n", buffer);

    // 段擦除
    W25Q32_SectorErase(0x00, 0x02);

    // 擦除后再次读取W25Q32中的数据
    W25Q32_ReadData(0x00, 0x02, 0x00, 0x00, buffer, 6);
    printf("%s\n", buffer);

    while (1)
    {
    }
}
