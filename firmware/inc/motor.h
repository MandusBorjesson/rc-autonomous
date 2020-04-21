#ifndef FIRMWARE_INC_MOTOR_H_
#define FIRMWARE_INC_MOTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

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

#endif  // FIRMWARE_INC_MOTOR_H_
