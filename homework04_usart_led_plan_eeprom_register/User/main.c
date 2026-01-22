#include "led_schedule.h"
#include <string.h>
#include <stdio.h>
/*
    1. 电脑通过串口 给单机 下发点灯计划  例如 13322  单片机上的灯 LED1 亮1秒 灭1秒 LED3 亮1秒 灭1秒 LED3 亮一秒 灭1秒 133221332213322->
    2. 要求这个计划要持久化。
    3. 计划是定长5位。

    // 计数
    串口
    I2C epprom
*/

int main(void)
{
    uint8_t flag = 0;

    led_schedule_init();

    while (1)
    {
        if (it_rx_idle_flag == 1)
        {
            flag = led_schedule_recv_plan(it_rx_buffer, it_rx_num);

            if (flag == 0)
            {
                printf("Please input again\n%s\t%d", it_rx_buffer, it_rx_num);
                Delay_s(1);
            }

            memset(it_rx_buffer, 0, it_rx_num);
            it_rx_num = 0;
            it_rx_idle_flag = 0;
        } 

        led_schedule_run_cycle();
    }
}
