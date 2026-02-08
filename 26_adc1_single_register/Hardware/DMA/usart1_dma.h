#ifndef __USART1_DMA_H__
#define __USART1_DMA_H__

#include "stm32f10x.h"

extern uint8_t TrCompleteFlag;

/**
 * @brief 串口1DAM发送功能初始化
 * 
 */
void DMA_USART1_TxInit(void);

/**
 * @brief 串口1DMA接收功能初始化
 * 
 */
void DMA_USART1_RxInit(void);

/**
 * @brief 配置DMA串口发送数据
 * 
 * @param pData 数据的地址
 * @param len 数据的长度
 */
void DMA_USART1_TransmitData(uint8_t *pData, uint16_t len);

/**
 * @brief 配置DMA串口接收数据
 * 
 * @param pData 
 * @param len 
 */
void DMA_USART1_ReceiveData(uint8_t *pData, uint16_t len);

#endif /* __USART1_DMA_H__ */