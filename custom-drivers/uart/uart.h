#ifndef UART_H
#define UART_H

#include <stdint.h>
/* Base Address */
#define RCC_BASE        0x40021000UL
#define GPIOA_BASE      0x40010800UL
#define USART1_BASE     0x40013800UL

/* RCC Register */
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18))

/* GPIO Register */
#define GPIOA_CRH       (*(volatile uint32_t *)(GPIOA_BASE + 0x04))

/* USART Register */
#define USART1_SR       (*(volatile uint32_t *)(USART1_BASE + 0x00))
#define USART1_DR       (*(volatile uint32_t *)(USART1_BASE + 0x04))
#define USART1_BRR      (*(volatile uint32_t *)(USART1_BASE + 0x08))
#define USART1_CR1      (*(volatile uint32_t *)(USART1_BASE + 0x0C))

void UART_Init(void);

void UART_SendChar(char c);

void UART_SendString(char *str);

char UART_ReadChar(void);

#endif
