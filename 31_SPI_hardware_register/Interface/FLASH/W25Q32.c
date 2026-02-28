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
void W25Q32_PageWrite(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t inner_addr, uint8_t *pData,
                       uint8_t len)
{
    W25Q32_WaitNoBusy();

    W25Q32_WriteEnable();

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
    W25Q32_WaitNoBusy();

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

/**
 * @brief 段擦除
 * 
 * @param block_addr 块地址
 * @param sector_addr 段地址
 */
void W25Q32_SectorErase(uint8_t block_addr, uint8_t sector_addr)
{
    W25Q32_WaitNoBusy();

    W25Q32_WriteEnable();

    SPI_Start();

    SPI_SwapByte(0x20);

    // 传输24位地址
    SPI_SwapByte(block_addr);
    SPI_SwapByte(sector_addr << 4);
    SPI_SwapByte(0x00);

    SPI_Stop();
}

/**
 * @brief 根据状态寄存器来判断忙不忙
 * 
 */
void W25Q32_WaitNoBusy(void)
{
    SPI_Start();

    SPI_SwapByte(0x05);

    // 轮询状态寄存器
    while (1)
    {
        // 接收发送来的状态寄存器的值
        uint8_t status = SPI_SwapByte(0x00);  // 这是主要是接收，传入任何参数都可以

        // 判断最低位，状态寄存器是否为0
        if ((status & 0x01) == 0)
        {
            // 忙完了
            break;
        }
    }
    
    SPI_Stop();
}