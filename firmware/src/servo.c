#include "servo.h"
#include "stm32f0xx.h"

void servo_init(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  uint32_t tmp =   RCC->AHBENR & RCC_AHBENR_GPIOBEN;
  (void)(tmp);

  SERVO_IOBANK->MODER = 0b10 << GPIO_MODER_MODER1_Pos;

  SERVO_IOBANK->AFR[0] = 0 << GPIO_AFRL_AFRL0_Pos;

  RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;  // Enable clock

  SERVO_TIM->PSC = 50;
  SERVO_TIM->ARR = 8192;
  SERVO_TIM->CCR1 = 8192-SERVO_CENTER;
  SERVO_TIM->CCMR1 |= 0b110 << TIM_CCMR1_OC1M_Pos;
  SERVO_TIM->CCER |= TIM_CCER_CC1E;
  SERVO_TIM->BDTR = TIM_BDTR_MOE;
  SERVO_TIM->CR1 |= TIM_CR1_ARPE;
  SERVO_TIM->EGR |= TIM_EGR_UG;
  SERVO_TIM->CR1 |= TIM_CR1_CEN;
}

void servo_set_angle(int8_t angle) {
  SERVO_TIM->CCR1 = 8192 - SERVO_CENTER - angle;
}

