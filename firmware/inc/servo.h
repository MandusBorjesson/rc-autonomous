
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __servo_H
#define __servo_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#define SERVO_TIM TIM1
#define SERVO_IOBANK GPIOB

void servo_init(void);

void servo_set_angle(unsigned char angle);

#ifdef __cplusplus
}
#endif
#endif /*__ servo_H */
