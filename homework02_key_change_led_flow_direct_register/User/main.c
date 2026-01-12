#include <led.h>
#include <key.h>
#include <delay.h>

uint16_t leds[3] = {LED1, LED2, LED3};
uint8_t i        = 0;



Direction dir = UP;

int main(void)
{
    LED_Init();
    KEY3_Init();

    while (1) {
        LED_On(leds[i]);
        Delay_ms(500);
        LED_Off(leds[i]);

        if (dir == UP) {
            i = (i + 1) % 3;
        } else {
            i = (i + 2) % 3;
        }
    }
}
