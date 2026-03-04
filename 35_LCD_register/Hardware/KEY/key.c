#include "key.h"

/**
 * @brief KEY3初始化
 *
 * KEY3 -> PF9
 */
void KEY3_Init(void)
{

    // 1. GPIO配置
    // 1.1 使能PF9对应的GPIOF的时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    // 1.2 配置PF9的工作模式 -> 下拉输入(CNF:10  MODE:00  ODR:0)
    GPIOF->CRH |= GPIO_CRH_CNF9_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF9_0;
    GPIOF->CRH &= ~GPIO_CRH_MODE9;
    GPIOF->ODR &= ~GPIO_ODR_ODR9;

    // 2. AFIO配置
    // 2.1 使能PF9对应的AFIO的时钟
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    // 2.2 配置PF9对应的EXTI  EXTICR[2]对应的是EXTICR3
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI9_PF;

    // 3. EXTI配置
    // 3.1 配置边沿检测电路，检测上升沿，所以配置上升沿触发选择寄存器
    EXTI->RTSR |= EXTI_RTSR_TR9;
    // 3.2 配置软件中断事件寄存器(一般来说不需要配)
    // 3.3 配置请求挂起寄存器(不需要配置，硬件自动置1)
    // 3.4 配置中断屏蔽寄存器(配置后，会启用PF9的中断)
    EXTI->IMR |= EXTI_IMR_MR9;

    // 4. NVIC配置
    // 4.1 设置抢占优先级和排队优先级的分组    设置为3 全是抢占优先级
    NVIC_SetPriorityGrouping(3);
    // 4.2 设置具体优先级
    NVIC_SetPriority(EXTI9_5_IRQn, 2);
    // 4.3 使能中断请求
    NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/**
 * @brief KEY1初始化
 *
 * KEY1 -> PF7
 */
void KEY1_Init(void)
{
    // 1. GPIO配置
    // 1.1 使能PF7对应的GPIOF的时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    // 1.2 配置PF7的工作模式 -> 下拉输入(CNF:10  MODE:00  ODR:0)
    GPIOF->CRL |= GPIO_CRL_CNF7_1;
    GPIOF->CRL &= ~GPIO_CRL_CNF7_0;
    GPIOF->CRL &= ~GPIO_CRL_MODE7;
    GPIOF->ODR &= ~GPIO_ODR_ODR7;

    // 2. AFIO配置
    // 2.1 使能PF7对应的AFIO的时钟
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    // 2.2 配置PF7对应的EXTI  EXTICR[1]对应的是EXTICR2
    AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI7_PF;

    // 3. EXTI配置
    // 3.1 配置边沿检测电路，检测上升沿，所以配置上升沿触发选择寄存器
    EXTI->RTSR |= EXTI_RTSR_TR7;
    // 3.2 配置软件中断事件寄存器(一般来说不需要配)
    // 3.3 配置请求挂起寄存器(不需要配置，硬件自动置1)
    // 3.4 配置中断屏蔽寄存器(配置后，会启用PF7的中断)
    EXTI->IMR |= EXTI_IMR_MR7;

    // 4. NVIC配置
    // 4.1 设置抢占优先级和排队优先级的分组    设置为3 全是抢占优先级
    NVIC_SetPriorityGrouping(3);
    // 4.2 设置具体优先级
    NVIC_SetPriority(EXTI9_5_IRQn, 2);
    // 4.3 使能中断请求
    NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/**
 * @brief KEY5初始化
 *
 * KEY5 -> PF11
 */
void KEY5_Init(void)
{
    // 1. GPIO配置
    // 1.1 使能PF11对应的GPIOF的时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    // 1.2 配置PF11的工作模式 -> 上拉输入(CNF:10  MODE:00  ODR:1)
    GPIOF->CRH |= GPIO_CRH_CNF11_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF11_0;
    GPIOF->CRH &= ~GPIO_CRH_MODE11;
    GPIOF->ODR |= GPIO_ODR_ODR11;

    // 2. AFIO配置
    // 2.1 使能PF11对应的AFIO的时钟
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    // 2.2 配置PF11对应的EXTI  EXTICR[2]对应的是EXTICR3
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI11_PF;

    // 3. EXTI配置
    // 3.1 配置边沿检测电路，检测下降沿，所以配置下降沿触发选择寄存器
    EXTI->FTSR |= EXTI_FTSR_TR11;
    // 3.2 配置软件中断事件寄存器(一般来说不需要配)
    // 3.3 配置请求挂起寄存器(不需要配置，硬件自动置1)
    // 3.4 配置中断屏蔽寄存器(配置后，会启用PF11的中断)
    EXTI->IMR |= EXTI_IMR_MR11;

    // 4. NVIC配置
    // 4.1 设置抢占优先级和排队优先级的分组    设置为3 全是抢占优先级
    NVIC_SetPriorityGrouping(3);
    // 4.2 设置具体优先级
    NVIC_SetPriority(EXTI15_10_IRQn, 2);
    // 4.3 使能中断请求
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}