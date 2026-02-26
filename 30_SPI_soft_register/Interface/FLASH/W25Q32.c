#include "W25Q32.h"

/**
 * @brief W25Q32初始化
 * 
 */
void W25Q32_Init(void)
{
    SPI_Init();
}

/**
 * @brief 读取JEDEC ID
 *
 * @return uint32_t
 */
uint32_t W25Q32_Read_JEDEC_ID(void)
{
    SPI_Start();
    SPI_SwapByte(0x9F);
    uint8_t ManuFacturerID = SPI_SwapByte(0x00); // 这里用于接收从设备传来的数据，所以给从设备传入任何形参都可以
    uint8_t MemoryTypeID = SPI_SwapByte(0x00);   // 这里用于接收从设备传来的数据，所以给从设备传入任何形参都可以
    uint8_t CapacityID = SPI_SwapByte(0x00);     // 这里用于接收从设备传来的数据，所以给从设备传入任何形参都可以
    SPI_Stop();

    return (ManuFacturerID << 16) | (MemoryTypeID << 8) | CapacityID;
}

/**
 * @brief 页写
 *
 * @param block_addr 块地址
 * @param sector_addr 段地址
 * @param page_addr 页地址
 * @param inner_addr 内部地址
 * @param pData 数据
 * @param len 数据长度
 * @return uint32_t
 */
void W25Q32_Page_Write(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t inner_addr, uint8_t *pData,
                       uint8_t len)
{
    SPI_Start();

    // 发送页写指令
    SPI_SwapByte(0x02);
    // 发送地址（高八位）
    SPI_SwapByte(block_addr);
    // 发送地址（中八位和低八位）
    SPI_SwapByte((sector_addr << 4) | (page_addr & 0x0F));
    // 发送内部地址
    SPI_SwapByte(inner_addr);

    // 发送数据
    for (uint8_t i = 0; i < len; i++)
    {
        SPI_SwapByte(pData[i]);
    }

    SPI_Stop();
}

/**
 * @brief 写使能
 * 
 */
void W25Q32_WriteEnable(void)
{
    SPI_Start();

    SPI_SwapByte(0x06);

    SPI_Stop();
}

/**
 * @brief 读数据
 * 
 * @param block_addr 块地址
 * @param sector_addr 段地址
 * @param page_addr 页地址
 * @param inner_addr 内部地址
 * @param pData 数据
 * @param len 数据长度
 */
void W25Q32_ReadData(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t inner_addr, uint8_t *buffer,
                     uint8_t len)
{
    SPI_Start();

    // 发送读数据指令
    SPI_SwapByte(0x03);
    // 发送地址（高8位 块地址）
    SPI_SwapByte(block_addr);
    // 发送地址（中8位和低8位 段地址和页地址）
    SPI_SwapByte((sector_addr << 4) | (page_addr & 0x0F));
    // 发送内部地址
    SPI_SwapByte(inner_addr);

    for (uint8_t i = 0; i < len; i++)
    {
        buffer[i] = SPI_SwapByte(0x00);  // 这里发什么都行，主要是要接收数据
    }
    
    SPI_Stop();
}