#ifndef __USART1_DMA_H__
#define __USART1_DMA_H__

#include "stm32f10x.h"

/**
 * @brief 串口1DAM功能初始化
 * 
 */
void DMA_USART1_TxInit(void);

/**
 * @brief 配置DMA要发送的数据的地址和长度
 * 
 * @param pData 数据的地址
 * @param len 数据的长度
 */
void DMA_USART1_TransmitData(uint8_t *pData, uint16_t len);

#endif /* __USART1_DMA_H__ */