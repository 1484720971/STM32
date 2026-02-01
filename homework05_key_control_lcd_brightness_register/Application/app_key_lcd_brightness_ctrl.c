#include "app_key_lcd_brightness_ctrl.h"

// 占空比
uint8_t duty = 0;
// 占空比修改标志位
uint8_t dutyFlag = 0;

/**
 * @brief 初始化按键控制LCD屏幕亮度
 *
 */
void key_control_lcd_brightness_10level_Init()
{
    // TIM3初始化
    TIM3_PWMLcd_Init();

    // KEY3初始化
    KEY3_Init();

    // KEY1初始化
    KEY1_Init();

    // KEY5初始化
    KEY5_Init();
}

/**
 * @brief 按键控制LCD屏幕亮度
 *
 */
void key_control_lcd_brightness_10level(void)
{
    if (dutyFlag == 1)
    {
        if (duty > 10)
        {
            duty = (duty == 11) ? 10 : 0;
        }

        TIM3_SetDuty(duty);

        // 清零占空比标志位
        dutyFlag = 0;
    }
}

// 中断服务函数
void EXTI9_5_IRQHandler(void)
{
    // 判断是否是中断9
    if (EXTI->PR & EXTI_PR_PR9)
    {
        // 清除中断标志位
        EXTI->PR |= EXTI_PR_PR9;

        // 延时消抖
        Delay_ms(5);

        // 判断这个按键是不是真的按下了
        if (GPIOF->IDR & GPIO_IDR_IDR9)
        {
            // LED翻转
            LED_Toggle(LED2);
        }
    }

    // 判断是否是中断7
    if (EXTI->PR & EXTI_PR_PR7)
    {
        // 清除中断标志位
        EXTI->PR |= EXTI_PR_PR7;

        // 延时消抖
        Delay_ms(5);

        // 判断这个按键是不是真的按下了
        if (GPIOF->IDR & GPIO_IDR_IDR7)
        {
            // 增加占空比
            duty++;
            // 占空比修改，标志位变1
            dutyFlag = 1;
        }
    }
}

void EXTI15_10_IRQHandler(void)
{
    // 判断是否是中断11
    if (EXTI->PR & EXTI_PR_PR11)
    {
        // 清除中断标志位
        EXTI->PR |= EXTI_PR_PR11;

        // 延时消抖
        Delay_ms(5);

        // 判断这个按键是不是真的按下了
        if ((GPIOF->IDR & GPIO_IDR_IDR11) == 0)
        {
            // 减少占空比
            duty--;
            // 占空比修改，标志位变1
            dutyFlag = 1;
        }
    }
}