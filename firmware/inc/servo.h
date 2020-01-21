
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __servo_H
#define __servo_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#define SERVO_TIM TIM14
#define SERVO_IOBANK GPIOB
#define SERVO_CENTER 749

void servo_init(void);

void servo_set_angle(int8_t angle);

#ifdef __cplusplus
}
#endif
#endif /*__ servo_H */
