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

	// 查看变量C的地址
	printf("address = %x", &c);
	while (1)
	{
		
	}
}
