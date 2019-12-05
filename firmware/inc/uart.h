
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __uart_H
#define __uart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#define UART_USART USART1
#define UART_IOBANK GPIOA
#define UART_BAUD 9600
#define UART_REFCLK 24000000

void uart_init(void);

void uart_send(uint8_t *data, uint16_t size);

#ifdef __cplusplus
}
#endif
#endif /*__ __uart_H */
