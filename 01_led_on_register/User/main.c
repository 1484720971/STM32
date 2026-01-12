#include <stdint.h>
#include "stm32f10x.h"

#define GPIOA_CRL 0x40010800
#define GPIOA_CRH 0x40010800 + 0x04

#define MY_RCC_APB2ENR_IOPAEN (0x01 << 2)
#define MY_GPIO_CRL_MODE0_0 (0x01 << 0)
#define MY_GPIO_CRL_MODE0_1 (0x01 << 1)
#define MY_GPIO_CRL_CNF0_0 (0x01 << 0)
#define MY_GPIO_CRL_CNF0_1 (0x01 << 1)
#define MY_GPIO_ODR0 (0x01 << 0)
#define MY_GPIO_ODR8 (0x01 << 8)

int main(void)
{
	// 1. 开启RCC_APB2的GPIOA的时钟使能              
	// *(uint32_t *)(0x40021000 + 0x18) = 0x4;
	
	// 2. 设置GPIOA的PA0为推挽输出
	// *(uint32_t *)(GPIOA_CRL) = 0x44444433; 
	
	// 3. 设置PA0为低电平
	// *(uint32_t *)(GPIOA_CRH) = 0x0;
	
	// 第二次改版
	// RCC -> APB2ENR |= MY_RCC_APB2ENR_IOPAEN;
	
	// GPIOA -> CRL &= ~(MY_GPIO_CRL_CNF0_0 | MY_GPIO_CRL_CNF0_1);
	// GPIOA -> CRL |= (MY_GPIO_CRL_MODE0_0 | MY_GPIO_CRL_MODE0_1);
	
	// GPIOA -> ODR &= ~MY_GPIO_ODR0;
	
	// 第三次改版
	// 开启GOIOA的时钟使能
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// 配置为推挽输出
	GPIOA->CRL &= ~GPIO_CRL_CNF0;
	GPIOA->CRL |= GPIO_CRL_MODE0;
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	GPIOA->CRL |= GPIO_CRL_MODE1;
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	GPIOA->CRH |= GPIO_CRH_MODE8;
	
	// 设置PA0、PA1、PA8输出低电平
	GPIOA->ODR &= ~GPIO_ODR_ODR0;
	GPIOA->ODR &= ~GPIO_ODR_ODR1;
	GPIOA->ODR &= ~GPIO_ODR_ODR8;
	return 0;
}
