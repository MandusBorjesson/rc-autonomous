#ifndef FIRMWARE_INC_STM32F0XX_IT_H_
#define FIRMWARE_INC_STM32F0XX_IT_H_

#ifdef __cplusplus
extern "C" {
#endif

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void TIM16_IRQHandler(void);
void USART1_IRQHandler(void);
#ifdef __cplusplus
}
#endif

#endif  // FIRMWARE_INC_STM32F0XX_IT_H_
