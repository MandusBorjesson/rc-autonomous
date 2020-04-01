#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "flash.h"
#include "stm32f0xx.h"

void uart_init(void) {
  uint32_t tmp;

  /* USART Setup */
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  tmp = RCC->APB2ENR & RCC_APB2ENR_USART1EN;
  (void)(tmp);

  RCC->CR2 |= RCC_CFGR3_USART1SW_HSI;

  UART_USART->BRR = ( UART_REFCLK + (UART_BAUD/2) ) / UART_BAUD;

  UART_USART->CR2 &= ~(USART_CR2_STOP_1 | USART_CR2_STOP_0);

  UART_USART->CR1 = USART_CR1_TE |
                    USART_CR1_RE |
                    USART_CR1_UE |
                    USART_CR1_UESM |
                    USART_CR1_RXNEIE;

  /* GPIO Setup */
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  tmp =   RCC->AHBENR & RCC_AHBENR_GPIOAEN;
  (void)(tmp);

  UART_IOBANK->MODER &= ~( GPIO_MODER_MODER9_Msk |
                           GPIO_MODER_MODER10_Msk );
  UART_IOBANK->MODER |= (0x02 << GPIO_MODER_MODER9_Pos) |
                        (0x02 << GPIO_MODER_MODER10_Pos) ; // Alternate function

  UART_IOBANK->AFR[1] &= ~( GPIO_AFRH_AFSEL9_Msk |
                            GPIO_AFRH_AFSEL10_Msk );
  UART_IOBANK->AFR[1] |= (0x01 << GPIO_AFRH_AFSEL9_Pos) |
                         (0x01 << GPIO_AFRH_AFSEL10_Pos); // UART

  UART_IOBANK->OSPEEDR &= ~( GPIO_OSPEEDR_OSPEEDR9_Msk |
                             GPIO_OSPEEDR_OSPEEDR10_Msk );
  UART_IOBANK->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR9_Pos) |
                          (0x03 << GPIO_OSPEEDR_OSPEEDR10_Pos);

  UART_IOBANK->OTYPER &= ~( GPIO_OTYPER_OT_9 |
                            GPIO_OTYPER_OT_10 );

  UART_IOBANK->PUPDR &= ~( GPIO_PUPDR_PUPDR9_Msk |
                           GPIO_PUPDR_PUPDR10_Msk );

  NVIC_EnableIRQ(USART1_IRQn);
  NVIC_SetPriority(USART1_IRQn,2);
}

void uart_send(char* data) {
  while(*data != 0) {
    while((UART_USART->ISR & USART_ISR_TXE) != USART_ISR_TXE) {}
    UART_USART->TDR = (*data++ & (uint8_t)0xFFU);
  }
  while((UART_USART->ISR & USART_ISR_TC) != USART_ISR_TC) {}
}

void uart_send_sz(char *data, uint16_t size) {
  while(size-- > 0)
  {
    while((UART_USART->ISR & USART_ISR_TXE) != USART_ISR_TXE) {}
    UART_USART->TDR = (*data++ & (uint8_t)0xFFU);
  }
  while((UART_USART->ISR & USART_ISR_TC) != USART_ISR_TC) {}
}

void uart_handle_key(char key, command_line *pl) {
  uint16_t val = 0;
  static char parse_ansi = 0;

  if (key == KEY_ETX) {
    pl->status = CMD_NONE;
    uart_send(ESC_CSH);
  } else if (pl->status != CMD_NONE) {
    return;
  }

  if (parse_ansi) {
    if (key >= 'A' && key <= 'Z') {
       val = 0x0100 + key;
      parse_ansi = 0;
    } else {
      return;
    }
  } else {
    val = key;
  }

  switch (val)
  {
  case KEY_ESC:
    parse_ansi = 1;
    break;

  case KEY_LFT:
    if (pl->cur_pos > 0) {
      uart_send(ESC_LFT);
      pl->cur_pos--;
    }
    break;

  case KEY_RGT:
    if (pl->cur_pos < pl->counter) {
      uart_send(ESC_RGT);
      pl->cur_pos++;
    }
    break;

  case KEY_DEL:
    if (pl->cur_pos < pl->counter) {
      pl->cur_pos++;
    } else {
      break;
    }
  case KEY_BSP:
    if (pl->counter == 0 || pl->cur_pos == 0)
      break;

    for (int i = pl->cur_pos - 1; i < pl->counter; i++) {
      pl->buf[i] = pl->buf[i+1];
    }
    pl->buf[pl->counter] = 0;

    pl->cur_pos--;
    pl->counter--;

    uart_send(ESC_SCP);
    uart_send(ESC_ERL);
    uart_send("\r> ");
    uart_send_sz(pl->buf, pl->counter);
    uart_send(ESC_RCP);
    uart_send(ESC_LFT);
    break;

  case KEY_CR:
  case KEY_ENT:
    pl->status = CMD_PEND;
    break;

  case KEY_ETX:
    uart_send("\r\n> ");

    memset(pl->buf, 0, CMD_BUF_SZ);
    pl->cur_pos = 0;
    pl->counter = 0;
    break;

  case KEY_FF:
      uart_send(ESC_ERD);
      uart_send("> ");
      uart_send_sz(pl->buf, pl->counter);
      break;

  case ' ' ... '~':
    if (pl->counter + 1 >= CMD_BUF_SZ) {
      uart_send(ESC_SCP);
      uart_send(ESC_ERL);
      uart_send("\rERROR: Command buffer full.\n");
      uart_send("\r> ");
      uart_send_sz(pl->buf, pl->counter);
      uart_send(ESC_RCP);
      uart_send(ESC_DWN);
    } else {
      for (int i = pl->counter; i > pl->cur_pos; i--) {
        pl->buf[i] = pl->buf[i-1];
      }
      pl->buf[pl->cur_pos] = key;

      pl->counter++;
      pl->cur_pos++;
      uart_send(ESC_SCP);
      uart_send(ESC_ERL);
      uart_send("\r> ");
      uart_send_sz(pl->buf, pl->counter);
      uart_send(ESC_RCP);
      uart_send(ESC_RGT);
    }
    break;

  default:
    break;
  }
}