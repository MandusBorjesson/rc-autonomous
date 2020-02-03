#ifndef __TYPES_H
#define __TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define RX_BUF_SZ 16

typedef enum car_status {WAIT, STOP, RUN} car_status;

typedef struct car_cfg {
  /* Inputs */
  uint8_t k_p;
  uint8_t k_i;
  uint8_t k_d;
  uint8_t max_spd;
  uint8_t trg_dist;
  car_status car_state;
} car_cfg;

typedef struct car_diag {
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

typedef enum uart_state {NO_CMD, CMD_RDY, BUF_FULL} uart_state;

typedef struct uart_buf {
  char buf[RX_BUF_SZ];
  uint16_t counter;
  enum uart_state state;
} uart_buf;

#ifdef __cplusplus
}
#endif

#endif /* __TYPES_H */
