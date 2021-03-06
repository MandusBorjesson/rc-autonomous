#ifndef FIRMWARE_INC_PID_H_
#define FIRMWARE_INC_PID_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "types.h"

// Signal attenuation, having a gain K_ATT will result in no amplification
#define K_ATT 4096
#define K_MAX 65535

// Max and min signal value, pid signals are limited to [S_MIN S_MAX]
#define S_MAX 32767
#define S_MIN -S_MAX

void calc_y(pid_cfg* cfg, pid_diag* diag, uint16_t val);

int16_t limit_signal(int32_t s);
#ifdef __cplusplus
}
#endif

#endif  // FIRMWARE_INC_PID_H_
