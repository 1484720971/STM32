#include "stdio.h"
#include "usart.h"
#include "fsmc_bank1_3_sram.h"

#if 0
/*
    1. __attribute__的作用：指定这个变量的位置(FSMC中bank1的3号插槽)
    2. __attribute__必须4字节对齐
    3. __attribute__只针对编译期可以确定地址的变量有效，对局部变量是无效的。因为局部变量是运行期创建的
    4. __attribute__编译器如果发现两个全局变量在同一地址 那么会自动避让 避让之后居然还不遵守四字节对齐原则
*/
uint8_t v1 __attribute__((at(0x68000000)));
uint8_t v2 __attribute__((at(0x68000004)));
uint8_t v4 __attribute__((at(0x68000004)));

int main(void)
{
    USART1_Init();
    FSMC_BANK1_3_SRAMInit();

    v1 = 20;
    printf("v1 loc = 0x%x val = %d\n", &v1, v1); // v1 loc = 0x68000000 val = 20

    v2 = 30;
    printf("v2 loc = 0x%x val = %d\n", &v2, v2); // v2 loc = 0x68000004 val = 30

    uint8_t v3 __attribute__((at(0x68000004)));
    v3 = 40;
    printf("v3 loc = 0x%x val = %d\n", &v3, v3); // v3 loc = 0x20000468 val = 40

    v4 = 120;
    printf("v4 loc = 0x%x val = %d\n", &v4, v4); // v4 loc = 0x68000005 val = 120

    // 5. 如何在没有全局变量的函数里实现对扩展sram的访问
    // 5.1 通过地址直接访问
    *(uint8_t *)(0x68000010) = 110;
    printf("0x68000010 val = %d\n", *(uint8_t *)(0x68000010));
    // 5.2 如果觉得直接写地址太丑了可以用指针美化一下。
    uint8_t * v5 = (uint8_t *)0x68000012;
    *v5 = 114;
    printf("0x68000012 val = %d\n", *v5);

    while (1)
    {
 
    }
}
#else

int main(void)
{
    USART1_Init();
    FSMC_BANK1_3_SRAMInit();

    *(uint8_t *)(0x68000000) = 0x33;
    *(uint8_t *)(0x68000001) = 0x44;

    printf("val = %x\r\n", *(uint16_t *)(0x68000001));
}

#endif