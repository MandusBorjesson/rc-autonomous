/* Includes ------------------------------------------------------------------*/
#include "buzzer.h"
#include "stm32f0xx.h"

void buzzer_init(void) {
  BUZZ_TIM->CR2 = 0;
  BUZZ_TIM->DIER  = 0;

  BUZZ_TIM->CCMR1 = TIM_CCMR1_OC1M_1 |
                    TIM_CCMR1_OC1M_2;
  BUZZ_TIM->PSC = 240; // 100 kHz
  BUZZ_TIM->CCR1 = 20; // 5 kHz
  BUZZ_TIM->CR1 = TIM_CR1_CEN;
  }

void buzzer_set_tone(int period_ms) {

}

