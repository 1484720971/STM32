#include <led.h>
#include <delay.h>

int main(void)
{
	LED_Init();

	while (1)
	{
		LED_On(LED1);
		Delay_s(1);
		LED_Off(LED1);

		LED_On(LED2);
		Delay_s(1);
		LED_Off(LED2);

		LED_On(LED3);
		Delay_s(1);
		LED_Off(LED3);	
	}
}
