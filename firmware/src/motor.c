/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "motor.h"
#include "stm32f0xx.h"

void motor_init(void) {
  motor_init_pins();
  motor_init_ilim();
  motor_init_timer();
}

void motor_init_pins(void) {
RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  MOTOR_IOBANK->MODER |= GPIO_MODER_MODER6_1 |
                         GPIO_MODER_MODER7_1; // Alternate function
  MOTOR_IOBANK->AFR[0] |= (0x1 << GPIO_AFRL_AFRL6_Pos) |
                          (0x1 << GPIO_AFRL_AFRL7_Pos);
}

void motor_init_timer(void) {
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Enable clock
  MOTOR_TIM->PSC = 8;  // Prescaler
  MOTOR_TIM->ARR = 127; // Count to this value (resolution)
  MOTOR_TIM->CCR1 = 0;
  MOTOR_TIM->CCR2 = 0;

  MOTOR_TIM->SMCR |= TIM_SMCR_SMS_1 |
                     TIM_SMCR_SMS_2;

  MOTOR_TIM->CCMR1 |= TIM_CCMR1_OC1M_2 | // Output compare 1 PWM mode 1
                      TIM_CCMR1_OC1M_1 |
                      TIM_CCMR1_OC2M_2 | // Output compare 2 PWM mode 1
                      TIM_CCMR1_OC2M_1;
  MOTOR_TIM->CCER |= TIM_CCER_CC1E |
                     TIM_CCER_CC2E;
  MOTOR_TIM->BDTR |= TIM_BDTR_MOE;
  MOTOR_TIM->CR1 |= TIM_CR1_ARPE;
  MOTOR_TIM->EGR |= TIM_EGR_UG;

  MOTOR_TIM->CR1 |= TIM_CR1_CEN;
}

void motor_init_ilim(void) {
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;
  DAC->CR |= DAC_CR_EN1;
  DAC->DHR8R1 = 0;
}


void motor_set_speed(int8_t speed) {
  uint32_t val = abs(speed);
  if (speed <= 0) {
    MOTOR_TIM->CCR1 = val;
    MOTOR_TIM->CCR2 = 0;
  } else {
    MOTOR_TIM->CCR1 = 0;
    MOTOR_TIM->CCR2 = val;
  }
}

void motor_set_ilim(uint8_t ilim) {
  DAC->DHR8R1 = ilim;
}
