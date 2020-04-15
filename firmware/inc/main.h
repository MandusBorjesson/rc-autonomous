#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx.h"
#include "types.h"

#define LOOP_INTERVAL_MS 200
#define MAIN_LOOP_TIMER TIM16
#define SENSOR_COUNT 3
#define STARTPIN_IOBANK GPIOB

car_diag diagnostics;
car_cfg config;
command_line cmd_line;
fifo rx_fifo;

void sysclk_cfg(void);
void setup_main(void);
void enter_sleep(void);
void enter_run(void);
void startpin_init(void);
int startpin_get(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
