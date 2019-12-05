/* Includes ------------------------------------------------------------------*/
#include "servo.h"
#include "stm32f0xx.h"

void servo_init(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  uint32_t tmp =   RCC->AHBENR & RCC_AHBENR_GPIOBEN;
  (void)(tmp);

  SERVO_IOBANK->MODER = GPIO_MODER_MODER0_1; // Alternate function
  SERVO_IOBANK->OSPEEDR = GPIO_OSPEEDER_OSPEEDR0_0 |
                           GPIO_OSPEEDER_OSPEEDR0_1;
  SERVO_IOBANK->AFR[0] = GPIO_AFRL_AFRL1; // TIM1_CH2N

  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // Enable clock
  SERVO_TIM->PSC = 24000;
  SERVO_TIM->ARR = 800;
  SERVO_TIM->CCR2 = 400;
  SERVO_TIM->CCMR1 |= TIM_CCMR1_OC2M_2 |
                      TIM_CCMR1_OC2M_1|
                      TIM_CCMR1_OC2PE;
  SERVO_TIM->CCER |= TIM_CCER_CC2NE;
  SERVO_TIM->BDTR = TIM_BDTR_MOE; // |
                     //TIM_BDTR_OSSI |
                     //TIM_BDTR_OSSR;
  SERVO_TIM->CR1 |= TIM_CR1_ARPE;
  SERVO_TIM->EGR |= TIM_EGR_UG;

  SERVO_TIM->CR1 |= TIM_CR1_CEN;
  }

void servo_set_angle(unsigned char angle) {

}

