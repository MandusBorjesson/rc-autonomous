
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __buzzer_H
#define __buzzer_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define BUZZ_TIM TIM14

void buzzer_init(void);

void buzzer_set_tone(int period_ms);

#ifdef __cplusplus
}
#endif
#endif /*__ buzzer_H */
