#ifndef __pid_H
#define __pid_H

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

int16_t calc_p(int16_t err, uint16_t k_p);
int16_t calc_i(int16_t err, uint16_t k_i);
int16_t calc_d(int16_t err, uint16_t k_d);

int8_t calc_y(car_cfg* cfg, car_diag* diag);

int16_t limit_signal(int32_t s);
#ifdef __cplusplus
}
#endif

#endif /*__ __pid_H */
