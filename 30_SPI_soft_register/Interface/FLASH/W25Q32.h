#ifndef __W25Q32_H__
#define __W25Q32_H__

#include "soft_spi.h"

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
void W25Q32_Page_Write(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t inner_addr, uint8_t *pData,
                       uint8_t len);

#endif /* __W25Q32_H__ */