/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "distance_sensor.h"
#include "main.h"
#include "motor.h"
#include "pid.h"
#include "servo.h"
#include "uart.h"

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  car_diag diagnostics;
  car_diag* diag = &diagnostics;

  car_cfg config;
  car_cfg* cfg = &config;

  uint8_t send_buf[sizeof(car_diag)];

  diag->key1 = 0xAA;
  diag->key2 = 0xBB;
  diag->servo_angle = 0;
  diag->motor_speed = 0;
  diag->battery_voltage = 0;

  cfg->k_p = 255;
  cfg->k_i = 0;
  cfg->k_d = 0;
  cfg->max_spd = 140;
  cfg->trg_dist = 100;

  /* Start delay */
  for (unsigned long int i = 0; i < 1000000; i++) {
    __NOP();
  }

  /* MCU Configuration */
  sysclk_cfg();
  setup_main();
  motor_init();
  uart_init();
//  buzzer_init();
  servo_init();
  if (sensor_init() != ADC_STAT_OK) {
    while(1); // Lock up
  }

  motor_set_ilim(255);
  motor_set_speed(0);

  while (1)
  {
    adc_sample_channels();
    diag->sensor_distance = sensor_get_value(ADC_SENS_FORW_OFFS);

    diag->servo_angle = calc_y(cfg, diag);

    servo_set_angle(diag->servo_angle);

    memcpy(&send_buf, diag, sizeof(send_buf));
    uart_send(send_buf, sizeof(send_buf));

    enter_sleep();
  }
}

/**
  * @brief System Clock Configuration, 24 MHz using HSI and PLL
  * @retval None
  */
void sysclk_cfg(void)
{
  // HSI Configuration
  RCC->CR |= RCC_CR_HSION;
  while ((RCC->CR & RCC_CR_HSIRDY) == 0) {}  // Wait for HSI
  RCC->CR &= ~RCC_CR_HSITRIM;
  RCC->CR |= 16u << RCC_CR_HSITRIM_Pos;
  RCC->CR2 |= RCC_CR2_HSI14DIS;
  RCC->CR2 |= RCC_CR2_HSI14ON;

  // HSI14 Configuration
  while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) {}  // Wait for HSI14
  RCC->CR2 &= ~RCC_CR2_HSI14TRIM;
  RCC->CR2 |= 16u << RCC_CR2_HSI14TRIM_Pos;

  // PLL Configuration
  RCC->CR &= ~RCC_CR_PLLON;
  while ((RCC->CR & RCC_CR_PLLRDY) != 0) {}  // Wait for PLL
  RCC->CFGR = RCC_CFGR_PLLMUL6 |
               RCC_CFGR_PLLSRC_HSI_DIV2;
  RCC->CFGR2 = RCC_CFGR2_PREDIV_1;
  RCC->CR |= RCC_CR_PLLON;
  while ((RCC->CR & RCC_CR_PLLRDY) == 0) {}  // Wait for PLL

  // CPU/Peripheral Configuration
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}  // Wait for PLL
  RCC->CFGR |= RCC_CFGR_PPRE_DIV1;
}

/**
  * @brief  Set up main loop timer, resonsible for the loop interval.
  * @retval None
  */
void setup_main(void) {
  RCC->APB2ENR |= RCC_APB2ENR_TIM16EN; // Enable clock
  MAIN_LOOP_TIMER->PSC = 2400;
  MAIN_LOOP_TIMER->ARR = LOOP_INTERVAL_MS;
  MAIN_LOOP_TIMER->DIER = TIM_DIER_UIE;
  MAIN_LOOP_TIMER->EGR |= TIM_EGR_UG;
  MAIN_LOOP_TIMER->CR1 |= TIM_CR1_ARPE |
                          TIM_CR1_CEN;

  NVIC_EnableIRQ(TIM16_IRQn);
  NVIC_SetPriority(TIM16_IRQn,2);
}

/**
  * @brief  Enter sleep mode (CPU off, peripherals running)
  * @retval None
  */
void enter_sleep(void)
{
    SCB->SCR &= ~( SCB_SCR_SLEEPDEEP_Msk );
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
    __WFI();
}

/**
  * @brief  Enter run mode (CPU and peripherals on)
  * @retval None
  */
void enter_run(void)
{
  SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
  __SEV();
}
