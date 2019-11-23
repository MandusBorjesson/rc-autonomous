
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __motor_H
#define __motor_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#define MOTOR_DAC DAC
#define MOTOR_IOBANK GPIOA
#define MOTOR_TIM TIM3

void motor_init(void);

void motor_init_pins(void);
void motor_init_timer(void);
void motor_init_ilim(void);

void motor_set_speed(int8_t speed);
void motor_set_ilim(uint8_t ilim);

#ifdef __cplusplus
}
#endif
#endif /*__ __motor_H */
