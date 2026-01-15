#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"

extern uint8_t it_rx_buffer[100];
extern uint8_t it_rx_num;
extern uint8_t it_rx_idle_flag;

/**
 * USART1初始化
 */
void USART1_Init(void);

/**
 * @brief USART1初始化带中断
 *
 */
void USART1_InitWithIt(void);

/**
 * USART1发送一个字节
 */
void USART1_TransmitByte(uint8_t byte);

/**
 * USART1接收一个字节
 */
uint8_t USART1_ReceiveByte();

/**
 * USART发送字符串
 */
void USART1_TransmitBytes(uint8_t* bytes, uint8_t len);

/**
 * USART接收字符串
 */
void USART1_ReceiveBytes(uint8_t* buffer, uint8_t len);

/**
 * @brief  通过空闲检测方式接收一串不固定长度的字节数据
 * @param  buffer 指向接收数据缓冲区的指针
 * @param  len 用于存储实际接收到的字节数的变量的指针
 * @retval None
 */
void USART1_ReceiveBytesToIdle(uint8_t* buffer, uint8_t* len);

#endif /* __USART_H__ */