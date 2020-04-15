/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fifo.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx.h"

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void) {}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void) {
  while (1) {}
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void) {}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void) {}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void) {}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void TIM16_IRQHandler(void) {
  TIM16->SR &= ~TIM_SR_UIF;  // Clear interrupt
  enter_run();
}

void USART1_IRQHandler(void) {
  fifo_put((char)USART1->RDR, &rx_fifo);
  USART1->RQR |= USART_RQR_RXFRQ;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
