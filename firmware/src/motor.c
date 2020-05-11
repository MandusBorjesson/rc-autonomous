#include <stdlib.h>
#include "motor.h"
#include "stm32f0xx.h"

void motor_init(void) {
  /* GPIO Setup */
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  uint32_t tmp =   RCC->AHBENR & RCC_AHBENR_GPIOAEN;
  (void)(tmp);

  // Wheel speed sensor
  GPIOA->MODER &= ~GPIO_MODER_MODER15_Msk;
  GPIOA->MODER |= (0x02 << GPIO_MODER_MODER15_Pos);
  GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL15_Msk;
  GPIOA->AFR[1] |= (0x02 << GPIO_AFRH_AFSEL15_Pos);

  // Motor PWM
  MOTOR_IOBANK->MODER |= GPIO_MODER_MODER6_1 |
                         GPIO_MODER_MODER7_1;  // Alternate function
  MOTOR_IOBANK->AFR[0] |= (0x1 << GPIO_AFRL_AFRL6_Pos) |
                          (0x1 << GPIO_AFRL_AFRL7_Pos);

  /* TIM2 Setup */
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  WHEEL_TIM->SMCR = TIM_SMCR_ECE;
  WHEEL_TIM->CR1 |= TIM_CR1_CEN;

  /* TIM3 Setup */
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  // Enable clock
  MOTOR_TIM->PSC = 8;  // Prescaler
  MOTOR_TIM->ARR = 127;  // Count to this value (resolution)
  MOTOR_TIM->CCR1 = 0;
  MOTOR_TIM->CCR2 = 0;

  MOTOR_TIM->SMCR |= TIM_SMCR_SMS_1 |
                     TIM_SMCR_SMS_2;

  MOTOR_TIM->CCMR1 |= TIM_CCMR1_OC1M_2 |  // Output compare 1 PWM mode 1
                      TIM_CCMR1_OC1M_1 |
                      TIM_CCMR1_OC2M_2 |  // Output compare 2 PWM mode 1
                      TIM_CCMR1_OC2M_1;
  MOTOR_TIM->CCER |= TIM_CCER_CC1E |
                     TIM_CCER_CC2E;
  MOTOR_TIM->BDTR |= TIM_BDTR_MOE;
  MOTOR_TIM->CR1 |= TIM_CR1_ARPE;
  MOTOR_TIM->EGR |= TIM_EGR_UG;

  MOTOR_TIM->CR1 |= TIM_CR1_CEN;

  /* DAC Setup */
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;
  DAC->CR |= DAC_CR_EN1;
  DAC->DHR8R1 = 0;
}

void motor_set_speed(int8_t speed) {
  uint32_t val = abs(speed);
  if (speed <= 0) {
    MOTOR_TIM->CCR1 = 0;
    MOTOR_TIM->CCR2 = val;
  } else {
    MOTOR_TIM->CCR1 = val;
    MOTOR_TIM->CCR2 = 0;
  }
}

uint16_t motor_get_speed(uint16_t period) {
  // calculate the speed by taking the number of wheel ticks (CNT) times the wheel
  // circumference (110mm), divided by number of ticks per revolution (3) and divide
  // by sample interval in seconds. Return value is in mm/s
  uint16_t tmp = (WHEEL_TIM->CNT * 110 * 1000) / (3 * period);
  WHEEL_TIM->CNT = 0;
  return tmp;
}

void motor_set_ilim(uint8_t ilim) {
  DAC->DHR8R1 = ilim;
}
