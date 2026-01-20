#include "M24C02.h"
#include "usart.h"
#include <stdio.h>

int main(void)
{
    M24C02_Init();
    USART1_Init();

    /* ------------- 写入单个字节 ------------- */
    M24C02_SendByte(0x00, 'a');

    /* ------------- 写入多个字节 ------------- */
    M24C02_SendBytes(0x01, "bcd", 3);

    /* ------------- 读取单个字节 ------------- */
    printf("%c\n", M24C02_ReadByte(0x00));

    /* ------------- 读取多个字节 ------------- */
    uint8_t buffer[5] = {0};
    M24C02_ReadBytes(0x00, 4, buffer);
    printf("%s\n", buffer);

    while (1)
    {
    }
}
