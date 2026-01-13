#include <led.h>
#include <key.h>
#include <delay.h>
#include <usart.h>
#include <string.h>
#include <stdio.h>

const uint8_t c = 128;

int main(void)
{
	USART1_Init();

	uint8_t buffer[100] = { 0 };
	uint8_t len = 0;
	while (1)
	{
		USART1_ReceiveBytesToIdle(buffer, &len);

		printf("len = %d, content = %s\n", len, buffer);

		memset(buffer, 0, len);	// 清空缓冲区
	}
}
