#ifndef __W25Q32_H__
#define __W25Q32_H__

#include "soft_spi.h"

/**
 * @brief 读取JEDEC ID
 * 
 * @return uint32_t 
 */
uint32_t W25Q32_Read_JEDEC_ID(void);

#endif /* __W25Q32_H__ */