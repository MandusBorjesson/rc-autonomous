/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f0xx.h"

/* Private defines -----------------------------------------------------------*/
#define LOOP_INTERVAL_MS 200
#define MAIN_LOOP_TIMER TIM16
#define SENSOR_COUNT 3

typedef struct {
  /* Inputs */
  uint8_t k_p;
  uint8_t k_i;
  uint8_t k_d;
  uint8_t max_spd;
  uint8_t trg_dist;
} car_cfg;

typedef struct {
  /* Outputs */
  uint8_t key1;
  uint8_t key2;
  int8_t s_p;
  int8_t s_i;
  int8_t s_d;
  int8_t servo_angle;
  int8_t motor_speed;
  uint8_t sensor_distance;
  uint8_t battery_voltage;
} car_diag;

void sysclk_cfg(void);
void setup_main(void);
void enter_sleep(void);
void enter_run(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
