/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fifo.h"
#include "spi.h"
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

void DMA1_Channel2_3_IRQHandler(void) {
  if ((DMA1->ISR & DMA_ISR_TCIF3) == DMA_ISR_TCIF3){
    RCK_IOBANK->ODR |= (1<<RCK_PIN_Pos);
  }
  DMA1->IFCR |= DMA_IFCR_CGIF3 | DMA_IFCR_CGIF2;
}

void TIM1_CC_IRQHandler (void) {
  if((TIM1->SR & TIM_SR_CC1IF) != 0) {
    TIM1->CCR1 += CLI_INTERVAL_MS;
    TIM1->SR &= ~TIM_SR_CC1IF;
    WUP_REASON |= WUP_CLI;
  }
  if ((TIM1->SR & TIM_SR_CC2IF) != 0) {
    TIM1->CCR2 += DST_INTERVAL_MS;
    TIM1->SR &= ~TIM_SR_CC2IF;
    WUP_REASON |= WUP_DST;
  }
  if ((TIM1->SR & TIM_SR_CC3IF) != 0) {
    TIM1->CCR3 += SPD_INTERVAL_MS;
    TIM1->SR &= ~TIM_SR_CC3IF;
    WUP_REASON |= WUP_SPD;
  }
  if ((TIM1->SR & TIM_SR_CC4IF) != 0) {
    TIM1->CCR4 += SPI_INTERVAL_MS;
    TIM1->SR &= ~TIM_SR_CC4IF;
    WUP_REASON |= WUP_SPI;
  }
  enter_run();
}

void USART1_IRQHandler(void) {
  fifo_put((char)USART1->RDR, &rx_fifo);
  USART1->RQR |= USART_RQR_RXFRQ;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
