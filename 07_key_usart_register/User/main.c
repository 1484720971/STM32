#include <led.h>
#include <key.h>
#include <delay.h>
#include <usart.h>
#include <string.h>
#include <stdio.h>


int main(void)
{
	// USART1_Init();
	LED_Init();
	USART1_InitWithIt();

	while (1)
	{
		if (it_rx_idle_flag == 1)
		{
			it_rx_idle_flag = 0;
			printf("len = %d content = %s", it_rx_num, it_rx_buffer);

			// 清零
			memset(it_rx_buffer, 0, it_rx_num);
			it_rx_num = 0;
		}


		LED_On(LED1);
		Delay_ms(200);
		LED_Off(LED1);

		LED_On(LED2);
		Delay_ms(200);
		LED_Off(LED2);

		LED_On(LED3);
		Delay_ms(200);
		LED_Off(LED3);
	}
}
