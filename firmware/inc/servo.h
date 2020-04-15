#ifndef FIRMWARE_INC_SERVO_H_
#define FIRMWARE_INC_SERVO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SERVO_TIM TIM14
#define SERVO_IOBANK GPIOB
#define SERVO_CENTER 700  // Greater -> left, Smaller -> right

void servo_init(void);

void servo_set_angle(int8_t angle);

#ifdef __cplusplus
}
#endif

#endif  // FIRMWARE_INC_SERVO_H_
