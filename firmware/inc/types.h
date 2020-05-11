#ifndef FIRMWARE_INC_TYPES_H_
#define FIRMWARE_INC_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CMD_BUF_SZ 64
#define FIFO_SZ 16

typedef enum car_status {WAIT, STOP, RUN} car_status;

typedef struct pid_cfg {
  uint16_t per;
  uint16_t trg;
  uint16_t k_p;
  uint16_t k_i;
  uint16_t k_d;
} pid_cfg;

typedef struct pid_diag {
  int16_t err_p;
  int32_t err_i;
  int16_t err_d;
  int16_t s_p;
  int16_t s_i;
  int16_t s_d;
  int16_t out;
} pid_diag;

typedef struct car_cfg {
  /* Inputs */
  car_status car_state;
  pid_cfg dst;
  pid_cfg spd;
} car_cfg;

typedef struct car_diag {
  /* Outputs */
  uint16_t speed;
  uint16_t dist;
  pid_diag dst;
  pid_diag spd;
} car_diag;

typedef enum cmd_status {CMD_NONE, CMD_PEND, CMD_BUSY} cmd_status;

typedef struct command_line {
  char buf[CMD_BUF_SZ];
  char zero;  // DO NOT MODIFY! Ensures buf is always zero-terminated
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

#endif  // FIRMWARE_INC_TYPES_H_
