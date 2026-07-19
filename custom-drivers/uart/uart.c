#include "uart.h"

void UART_Init(void)
{
    /* Code khởi tạo UART */
    /* Enable Clock GPIOA */
    RCC_APB2ENR |= (1 << 2);

    /* Enable Clock USART1 */
    RCC_APB2ENR |= (1 << 14);

    /*
        PA9  -> TX
        PA10 -> RX
    */

    /* PA9 : Alternate Function Push Pull 50MHz */

    GPIOA_CRH &= ~(0xF << 4);
    GPIOA_CRH |=  (0xB << 4);

    /* PA10 : Floating Input */

    GPIOA_CRH &= ~(0xF << 8);
    GPIOA_CRH |=  (0x4 << 8);

    /*
        Baudrate = 9600

        Clock = 8MHz

        USARTDIV = 8 000 000 / (16 × 9600) ≈ 52.08
         52 = 0x34
         BRR = (0x34 << 4) | 0x1 = 0x341
    */

    USART1_BRR = 0x341;

    /* Enable Receiver */

    USART1_CR1 |= (1 << 2);

    /* Enable Transmitter */

    USART1_CR1 |= (1 << 3);

    /* Enable USART */

    USART1_CR1 |= (1 << 13);
}

void UART_SendChar(char c)
{
    /* Code gửi 1 byte */
    /* Wait TXE = 1 */

    while(!(USART1_SR & (1 << 7)));

    USART1_DR = c;
}

void UART_SendString(char *str)
{
    while(*str)
    {
        UART_SendChar(*str);
        str++;
    }

}

char UART_ReadChar(void)
{
    /* Code nhận 1 byte */
    /* Wait RXNE = 1 */
    while(!(USART1_SR & (1 << 5)));

    return USART1_DR;
}
