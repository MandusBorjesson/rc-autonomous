/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

//#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"

/* Private defines -----------------------------------------------------------*/
#define LOOP_INTERVAL_MS 100
#define MAIN_LOOP_TIMER TIM16

void Error_Handler(void);
void setup_main(void);
void enter_Sleep(void);

// #define VBAT_ADC_Pin LL_GPIO_PIN_3
// #define VBAT_ADC_GPIO_Port GPIOA
// #define M_VREF_Pin LL_GPIO_PIN_4
// #define M_VREF_GPIO_Port GPIOA
// #define M_IN1_Pin LL_GPIO_PIN_6
// #define M_IN1_GPIO_Port GPIOA
// #define M_IN2_Pin LL_GPIO_PIN_7
// #define M_IN2_GPIO_Port GPIOA
// #define SERVO_N_Pin LL_GPIO_PIN_0
// #define SERVO_N_GPIO_Port GPIOB
// #define BUZZ_Pin LL_GPIO_PIN_1
// #define BUZZ_GPIO_Port GPIOB
// #define IO_START_Pin LL_GPIO_PIN_2
// #define IO_START_GPIO_Port GPIOB
// #define LED_BOARD_Pin LL_GPIO_PIN_11
// #define LED_BOARD_GPIO_Port GPIOA
// #define A3_Pin LL_GPIO_PIN_15
// #define A3_GPIO_Port GPIOA
// #define A2_Pin LL_GPIO_PIN_3
// #define A2_GPIO_Port GPIOB
// #define A1_Pin LL_GPIO_PIN_4
// #define A1_GPIO_Port GPIOB
// #define SPI_RCK_Pin LL_GPIO_PIN_6
// #define SPI_RCK_GPIO_Port GPIOB
// #define SPI_OE_Pin LL_GPIO_PIN_7
// #define SPI_OE_GPIO_Port GPIOB
// #ifndef NVIC_PRIORITYGROUP_0
// #define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
//                                                                  4 bits for subpriority */
// #define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
//                                                                  3 bits for subpriority */
// #define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
//                                                                  2 bits for subpriority */
// #define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
//                                                                  1 bit  for subpriority */
// #define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
//                                                                  0 bit  for subpriority */
char run;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
