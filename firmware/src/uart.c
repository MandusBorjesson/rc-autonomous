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

void uart_send(uint8_t *data, uint16_t size) {
  uint16_t counter = size;
  while(counter > 0)
  {
    counter--;
    // Wait for USART
    while((UART_USART->ISR & USART_ISR_TXE) != USART_ISR_TXE) {}
    UART_USART->TDR = (*data++ & (uint8_t)0xFFU);
  }
  while((UART_USART->ISR & USART_ISR_TC) != USART_ISR_TC) {}
}

void uart_handle_cmd(uart_buf* pb, car_cfg* pc) {
  char cmd[RX_BUF_SZ] = {0};
  int val = 0;
  char *off;

  if (pb->state == BUF_FULL) {
    memset(pb->buf, 0, RX_BUF_SZ);
    pb->counter = 0;
    pb->state = NO_CMD;
  }

  off = strchr(pb->buf,'=');

  if (off != NULL) {
    memcpy(cmd, pb->buf, off - pb->buf);
    val = atoi(off+1);
  } else {
    memcpy(cmd, pb->buf, RX_BUF_SZ);
  }

  if (strcmp(cmd, "start") == 0)
        pc->car_state = RUN;
  else if (strcmp(cmd, "stop") == 0)
        pc->car_state = STOP;
  else if (strcmp(cmd, "store_cfg") == 0)
        flash_write(pc, sizeof(car_cfg));
  else if (strcmp(cmd, "k_p") == 0)
        pc->k_p = val;
  else if (strcmp(cmd, "k_i") == 0)
        pc->k_i = val;
  else if (strcmp(cmd, "k_d") == 0)
        pc->k_d = val;
  else if (strcmp(cmd, "max_spd") == 0)
        pc->max_spd = val;
  else if (strcmp(cmd, "trg_dist") == 0)
        pc->trg_dist = val;

  memset(pb->buf, 0, RX_BUF_SZ);
  pb->counter = 0;
  pb->state = NO_CMD;
}