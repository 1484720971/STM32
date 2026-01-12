// 掩码的四种运算
#include <stdint.h>
#include <stdio.h>

#define LAMP_1_MASK (1 << 0) // 掩码: 0000 0001
#define LAMP_2_MASK (1 << 1) // 掩码: 0000 0010
#define LAMP_3_MASK (1 << 2) // 掩码: 0000 0100
#define LAMP_4_MASK (1 << 3) // 掩码: 0000 1000
#define LAMP_5_MASK (1 << 4) // 掩码: 0001 0000
#define LAMP_6_MASK (1 << 5) // 掩码: 0010 0000
#define LAMP_7_MASK (1 << 6) // 掩码: 0100 0000
#define LAMP_8_MASK (1 << 7) // 掩码: 1000 0000

uint8_t lamps = 0x69; // 0110 1001

/**
 * @brief 打印32位二进制数，从高到低位
 *
 * @param num
 */
void print_binary(uint8_t num)
{
    for (int i = 7; i >= 0; i--)
    {
        printf("%d", (num >> i) & 1); // 每次右移1位，然后与1进行AND运算得到最低位的值
        if (i % 4 == 0)
        {
            printf(" "); // 每4位打印一个空格，便于阅读
        }
    }
}

int main()
{
    // 按位与 取状态
    if (lamps & LAMP_6_MASK)
    {
        printf("LAMP_6 is on\n");
    }
    else
    {
        printf("LAMP_6 is off\n");
    }

    // 置位1 按位或
    printf("当前状态：");
    print_binary(lamps);
    lamps |= LAMP_5_MASK;
    printf("\n设置LAMP_5后状态:");
    print_binary(lamps);

    // 置为0 与非
    printf("\n当前状态:");
    print_binary(lamps);
    lamps &= ~LAMP_5_MASK;
    printf("\n置位0后状态:");
    print_binary(lamps);

    // 翻转 按位异或
    printf("\n当前状态:");
    print_binary(lamps);
    lamps ^= LAMP_6_MASK;
    printf("\n翻转LAMP_6后状态:");
    print_binary(lamps);

    return 0;
}
