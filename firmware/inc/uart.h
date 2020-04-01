#ifndef __uart_H
#define __uart_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "stm32f0xx.h"
#include "types.h"

#define UART_USART USART1
#define UART_IOBANK GPIOA
#define UART_BAUD 9600
#define UART_REFCLK 24000000

#define KEY_RGT 0x0143 // Right key
#define KEY_LFT 0x0144 // Left key
#define KEY_ESC '\x1b' // Escape key
#define KEY_DEL '\x7f' // Delete key
#define KEY_BSP '\x08' // Backspace key
#define KEY_CR  '\r'   // Carriage return
#define KEY_ENT '\n'   // Newline
#define KEY_FF  '\x0c' // ctrl-l
#define KEY_ETX '\x03' // ctrl-c

#define ESC_UP  "\x1b[1A" // Keyboard 'UP' key
#define ESC_DWN "\x1b[1B" // Keyboard 'DOWN' key
#define ESC_RGT "\x1b[1C" // Keyboard 'RIGHT' key
#define ESC_LFT "\x1b[1D" // Keyboard 'LEFT' key
#define ESC_ERL "\x1b[2K" // Erase current line
#define ESC_ERD "\x1b[2J" // Erase screen
#define ESC_SCP "\x1b[s"  // Save cursor position
#define ESC_RCP "\x1b[u"  // Restore cursor position
#define ESC_CHD "\x9b\x3f\x32\x35\x6c" // Hide cursor
#define ESC_CSH "\x9b\x3f\x32\x35\x68" // Show cursor

void uart_init(void);

void uart_send(char *data);
void uart_send_sz(char *data, uint16_t size);

void uart_handle_key(char key, command_line *pl);

#ifdef __cplusplus
}
#endif

#endif /*__ __uart_H */
