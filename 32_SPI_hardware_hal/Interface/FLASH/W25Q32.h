#ifndef __W25Q32_H__
#define __W25Q32_H__

#include "hard_spi.h"


/**
 * @brief 读取JEDEC ID
 *
 * @return uint32_t
 */
uint32_t W25Q32_Read_JEDEC_ID(void);

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
                       uint8_t len);

/**
 * @brief 写使能
 * 
 */
void W25Q32_WriteEnable(void);

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
void W25Q32_ReadData(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t inner_addr, uint8_t *pData,
                     uint8_t len);

/**
 * @brief 段擦除
 * 
 * @param block_addr 块地址
 * @param sector_addr 段地址
 */
void W25Q32_SectorErase(uint8_t block_addr, uint8_t sector_addr);

/**
 * @brief 根据状态寄存器来判断忙不忙
 * 
 */
void W25Q32_WaitNoBusy(void);
#endif /* __W25Q32_H__ */