
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __pid_H
#define __pid_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "main.h"

int8_t calc_p(int8_t err, uint8_t k_p);
int8_t calc_i(int8_t err, uint8_t k_i);
int8_t calc_d(int8_t err, uint8_t k_d);

void calc_y(car_cfg* cfg, car_diag* diag);

#ifdef __cplusplus
}
#endif
#endif /*__ __pid_H */
