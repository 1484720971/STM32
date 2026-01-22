#include "M24C02.h"
#include <stdio.h>
#include "usart.h"

int main(void)
{
    USART1_Init();
    M24C02_Init();

    /* --------------- 发送单个字节 --------------- */
//     M24C02_SendByte(0x01, 'b');

    M24C02_SendBytes(0x00, "ABCDE", 5);

    /* --------------- 读取单个字节 --------------- */
//     printf("%c", M24C02_ReadByte(0x01));

    uint8_t buffer[10] = {0};
    M24C02_ReadBytes(0x00, 5, buffer);
    printf("%s", buffer);

    while (1)
    {
       
    }
}