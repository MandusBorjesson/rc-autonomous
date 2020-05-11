#ifndef FIRMWARE_INC_MAIN_H_
#define FIRMWARE_INC_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx.h"
#include "types.h"

#define CLI_INTERVAL_MS 100
#define DST_INTERVAL_MS 20
#define SPD_INTERVAL_MS 250
#define SPI_INTERVAL_MS 50
#define MAIN_LOOP_TIMER TIM1
#define SENSOR_COUNT 3
#define STARTPIN_IOBANK GPIOB

#define WUP_CLI (1 << 0)
#define WUP_DST (1 << 1)
#define WUP_SPD (1 << 2)
#define WUP_SPI (1 << 3)

char WUP_REASON;

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

#endif  // FIRMWARE_INC_MAIN_H_
