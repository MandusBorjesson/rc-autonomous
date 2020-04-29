#ifndef FIRMWARE_INC_MOTOR_H_
#define FIRMWARE_INC_MOTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MOTOR_DAC DAC
#define MOTOR_IOBANK GPIOA
#define MOTOR_TIM TIM3

#define WHEEL_TIM TIM2

void motor_init(void);

void motor_set_speed(int8_t speed);
uint16_t motor_get_speed(uint16_t period);
void motor_set_ilim(uint8_t ilim);

#ifdef __cplusplus
}
#endif

#endif  // FIRMWARE_INC_MOTOR_H_
