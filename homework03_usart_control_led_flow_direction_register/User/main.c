#include <led.h>
#include <usart.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	LED_Init();
	USART1_InitWithIt();

	uint8_t dir = 0; // 0: UP, 1: DOWN

	while (1)
	{
		// 1. 流水灯逻辑：这里的 dir 决定了灯的方向
		LED_On_All(dir);

		if (it_rx_num > 0)
		{
			// 使用 strstr 判断是否包含关键字，比 strcmp 更能容忍换行符
			if (strstr((char*)it_rx_buffer, "UP") != NULL)
			{
				dir = 0;
				printf("Direction set to UP\n");
			}
			else if (strstr((char*)it_rx_buffer, "DOWN") != NULL)
			{
				dir = 1;
				printf("Direction set to DOWN\n");
			}
			else
			{
				// 只有真正收到乱码指令才报错
				printf("Unknown Command: %s\n", it_rx_buffer);
			}

			// 处理完后立即清空，准备下次接收
			memset(it_rx_buffer, 0, it_rx_num);
			it_rx_num = 0;
		}
		printf("%s\n", it_rx_buffer);
	}
}