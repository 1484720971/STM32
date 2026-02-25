#include "W25Q32.h"

/**
 * @brief 读取JEDEC ID
 * 
 * @return uint32_t 
 */
uint32_t W25Q32_Read_JEDEC_ID(void)
{
    SPI_Init();

    SPI_Start();
    SPI_SwapByte(0x9F);
    uint8_t ManuFacturerID =  SPI_SwapByte(0x00); // 这里用于接收从设备传来的数据，所以给从设备传入任何形参都可以
    uint8_t MemoryTypeID = SPI_SwapByte(0x00); // 这里用于接收从设备传来的数据，所以给从设备传入任何形参都可以
    uint8_t CapacityID = SPI_SwapByte(0x00); // 这里用于接收从设备传来的数据，所以给从设备传入任何形参都可以
    SPI_Stop();

    return (ManuFacturerID << 16) | (MemoryTypeID << 8) | CapacityID;
}