#include <led.h>
#include <key.h>
#include <delay.h>
#include <usart.h>
#include <string.h>

int main(void)
{
	USART1_Init();

	uint8_t *str = "Hello World";
	uint8_t buffer[100] = {0};

	USART1_TransmitBytes(str, strlen((const char *)str));

	USART1_ReceiveBytes(buffer, 7);
	USART1_TransmitBytes(buffer, 7);
	while (1)
	{
		// USART1_ReceiveBytes();
	}
}
