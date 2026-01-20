#include "led_schedule.h"

/**
 * @brief 点灯计划初始化
 *
 */
void led_schedule_init(void)
{
    // USART初始化
    USART1_InitWithIt();
    // LED初始化
    LED_Init();
    // EEPROM初始化
    M24C02_Init();
}


/**
 * @brief 点灯计划 - 接收&存储数据
 *
 * @param plan_buf 下发的指令
 * @param recv_len 下发指令的长度
 * @return uint8_t 0 - 发送失败    1 - 发送成功
 */
uint8_t led_schedule_recv_plan(uint8_t* plan_buf, uint8_t recv_len)
{
    if (recv_len != 5)
    {
        printf("Please input 5-length string!\n");
        return 0;
    }

    for (uint8_t i = 0; i < recv_len; i++)
    {
        if (*(plan_buf + i) <= '0' || *(plan_buf + i) >= '4')
        {
            printf("Only 1,2,3 are valid!\n");
            return 0;
        }
    }

    M24C02_SendBytes(0x00, plan_buf, 5);
    return 1;
}

/**
 * @brief 点灯计划 - 根据EEPROM中的指令点亮LED
 *
 */
void led_schedule_run_cycle(void)
{
    uint8_t plan_buf[5] = { 0 };
    M24C02_ReadBytes(0x00, 5, plan_buf);
    for (uint8_t i = 0; i < 5; i++)
    {
        if (plan_buf[i] == '1')
        {
            LED_On(LED1);
            Delay_s(1);
            LED_Off(LED1);
            Delay_s(1);
        }
        else if (plan_buf[i] == '2')
        {
            LED_On(LED2);
            Delay_s(1);
            LED_Off(LED2);
            Delay_s(1);
        }
        else if (plan_buf[i] == '3')
        {
            LED_On(LED3);
            Delay_s(1);
            LED_Off(LED3);
            Delay_s(1);
        }
        else
        {
            
        }
    }
}