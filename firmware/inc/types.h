#ifndef __TYPES_H
#define __TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CMD_BUF_SZ 64
#define FIFO_SZ 16

typedef enum car_status {WAIT, STOP, RUN} car_status;

typedef struct car_cfg {
  /* Inputs */
  uint16_t k_p;
  uint16_t k_i;
  uint16_t k_d;
  uint8_t max_spd;
  uint16_t trg_dist;
  car_status car_state;
} car_cfg;

typedef struct car_diag {
  /* Outputs */
  uint16_t dist;
  int16_t err;
  int16_t s_p;
  int16_t s_i;
  int16_t s_d;
  int8_t servo_angle;
  int8_t motor_speed;
} car_diag;

typedef enum cmd_status {CMD_NONE, CMD_PEND, CMD_BUSY} cmd_status;

typedef struct command_line {
  char buf[CMD_BUF_SZ];
  char zero; // DO NOT MODIFY! Ensures buf is always zero-terminated
  uint16_t counter;
  uint16_t cur_pos;
  enum cmd_status status;
} command_line;

typedef struct fifo {
  char buf[FIFO_SZ];
  uint8_t head;
  uint8_t tail;
} fifo;

#ifdef __cplusplus
}
#endif

#endif /* __TYPES_H */
