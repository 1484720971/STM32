#include "W25Q32.h"
#include "usart.h"
#include "stdio.h"

int main(void)
{
    USART1_Init();

    uint32_t id = W25Q32_Read_JEDEC_ID();    

    printf("ManuFacturerID: 0x%X\n", (id & 0xFF0000) >> 16);
    printf("MemoryTypeID: 0x%X\n", (id & 0xFF00) >> 8);
    printf("CapacityID: 0x%X\n", id & 0xFF);

    while (1)
    {

    }
}
