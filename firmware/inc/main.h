/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Private defines -----------------------------------------------------------*/
#define LOOP_INTERVAL_MS 200
#define MAIN_LOOP_TIMER TIM16

void Error_Handler(void);
void sysclk_cfg(void);
void setup_main(void);
void enter_sleep(void);
void enter_run(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */