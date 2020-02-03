/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f0xx.h"
#include "types.h"

/* Private defines -----------------------------------------------------------*/
#define LOOP_INTERVAL_MS 200
#define MAIN_LOOP_TIMER TIM16
#define SENSOR_COUNT 3

car_diag diagnostics;
car_diag* diag;

car_cfg config;
car_cfg* cfg;

uart_buf uart_buffer;
uart_buf* rx_buf;

void sysclk_cfg(void);
void setup_main(void);
void enter_sleep(void);
void enter_run(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
