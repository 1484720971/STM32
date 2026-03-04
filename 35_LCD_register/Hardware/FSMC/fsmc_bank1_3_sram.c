#include "fsmc_bank1_3_sram.h"

/**
 * @brief FSMC_Init
 *
 */
void FSMC_Init(void)
{
    // 1. 使能时钟
    // 1.1 GPIO时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;

    // 1.2 FSMC时钟使能
    RCC->AHBENR |= RCC_AHBENR_FSMCEN;

    // 2. 配置GPIO
    FSMC_GPIO_Init();

    // 3. FSMC初始化
    // 3.1 fsmc的配置 Bank1的 3区 BCR3
    // 3.1.1 存储块使能
    FSMC_Bank1->BTCR[6] |= FSMC_BCR3_MBKEN;
    // 3.1.2 设置存储类型 00=SRAM ROM
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR3_MTYP;
    // 3.1.3 禁止闪存访问
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR3_FACCEN;
    // 3.1.4 地址和数据复用: 不复用
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR3_MUXEN;
    // 3.1.5 数据总线的宽度 16位宽度=01
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR3_MWID_1;
    FSMC_Bank1->BTCR[6] |= FSMC_BCR3_MWID_0;
    // 3.1.6 写使能;
    FSMC_Bank1->BTCR[6] |= FSMC_BCR3_WREN;

    // 3.2 fsmc的 时序 
    // 3.2.1 地址建立时间 对同步读写来说,永远一个周期
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR3_ADDSET;
    // 3.2.2 地址保持时间 对同步读写来说,永远一个周期
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR3_ADDHLD;
    // 3.2.3 数据保持时间 手册不能低于55ns 我们设置1us
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR3_DATAST;
    FSMC_Bank1->BTCR[7] |= (71 << 8);
}

void FSMC_GPIO_Init(void)
{
    /* 1 配置 PG0 地址端口(FSMC-A10)的输出模式 复用推挽输出CNF:10 50MHz速度 MODE:11*/
    /* =============MODE=============== */
    GPIOG->CRL |= GPIO_CRL_MODE0;

    /* =============CNF=============== */
    GPIOG->CRL |= GPIO_CRL_CNF0_1;
    GPIOG->CRL &= ~GPIO_CRL_CNF0_0;

    /*
      2 数据端口 复用推挽输出 D0 - D15
        在实际应用中，即使数据线被配置为输出模式，FSMC控制器仍然能够管理数据线的方向，使其在需要时成为输入线。
        这种自动切换是由FSMC控制器硬件管理的，不需要软件干预。
        因此，即使GPIO配置为复用推挽输出，FSMC依然可以实现读取操作。
    */
    /* =============MODE=============== */
    GPIOD->CRL |= (GPIO_CRL_MODE0 |
        GPIO_CRL_MODE1);
    GPIOD->CRH |= (GPIO_CRH_MODE8 |
        GPIO_CRH_MODE9 |
        GPIO_CRH_MODE10 |
        GPIO_CRH_MODE14 |
        GPIO_CRH_MODE15);

    GPIOE->CRL |= (GPIO_CRL_MODE7);
    GPIOE->CRH |= (GPIO_CRH_MODE8 |
        GPIO_CRH_MODE9 |
        GPIO_CRH_MODE10 |
        GPIO_CRH_MODE11 |
        GPIO_CRH_MODE12 |
        GPIO_CRH_MODE13 |
        GPIO_CRH_MODE14 |
        GPIO_CRH_MODE15);

    /* =============CNF=============== */
    GPIOD->CRL |= (GPIO_CRL_CNF0_1 |
        GPIO_CRL_CNF1_1);
    GPIOD->CRL &= ~(GPIO_CRL_CNF0_0 |
        GPIO_CRL_CNF1_0);

    GPIOD->CRH |= (GPIO_CRH_CNF8_1 |
        GPIO_CRH_CNF9_1 |
        GPIO_CRH_CNF10_1 |
        GPIO_CRH_CNF14_1 |
        GPIO_CRH_CNF15_1);
    GPIOD->CRH &= ~(GPIO_CRH_CNF8_0 |
        GPIO_CRH_CNF9_0 |
        GPIO_CRH_CNF10_0 |
        GPIO_CRH_CNF14_0 |
        GPIO_CRH_CNF15_0);

    GPIOE->CRL |= (GPIO_CRL_CNF7_1);
    GPIOE->CRL &= ~(GPIO_CRL_CNF7_0);

    GPIOE->CRH |= (GPIO_CRH_CNF8_1 |
        GPIO_CRH_CNF9_1 |
        GPIO_CRH_CNF10_1 |
        GPIO_CRH_CNF11_1 |
        GPIO_CRH_CNF12_1 |
        GPIO_CRH_CNF13_1 |
        GPIO_CRH_CNF14_1 |
        GPIO_CRH_CNF15_1);
    GPIOE->CRH &= ~(GPIO_CRH_CNF8_0 |
        GPIO_CRH_CNF9_0 |
        GPIO_CRH_CNF10_0 |
        GPIO_CRH_CNF11_0 |
        GPIO_CRH_CNF12_0 |
        GPIO_CRH_CNF13_0 |
        GPIO_CRH_CNF14_0 |
        GPIO_CRH_CNF15_0);

    /* 3 其他控制端口 复用推挽输出 */
    GPIOD->CRL |= (GPIO_CRL_MODE4 |
        GPIO_CRL_MODE5);
    GPIOD->CRL |= (GPIO_CRL_CNF4_1 |
        GPIO_CRL_CNF5_1);
    GPIOD->CRL &= ~(GPIO_CRL_CNF4_0 |
        GPIO_CRL_CNF5_0);

    /* 4 片选引脚 */
    GPIOG->CRH |= (GPIO_CRH_MODE12);
    GPIOG->CRH |= (GPIO_CRH_CNF12_1);
    GPIOG->CRH &= ~(GPIO_CRH_CNF12_0);

    /* 5 复位引脚 */
    GPIOG->CRH |= GPIO_CRH_MODE15;
    GPIOG->CRH &= ~GPIO_CRH_CNF15;

    /* 6 背光引脚 */
    GPIOB->CRL |= GPIO_CRL_MODE0;
    GPIOB->CRL &= ~GPIO_CRL_CNF0;
}