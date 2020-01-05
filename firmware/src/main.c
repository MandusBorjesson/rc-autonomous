/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "main.h"
// #include "buzzer.h"
#include "servo.h"
#include "motor.h"
#include "uart.h"
#include "distance_sensor.h"

//#include "stm32f051x8.h"

typedef struct {
  /* Inputs */
  int k_p;
  int k_i;
  int k_d;
  int k_p_speed;
  int max_speed;
} car_settings;

typedef struct {
  /* Outputs */
  uint8_t key1;
  uint8_t key2;
  uint8_t s_p;
  uint8_t s_i;
  uint8_t s_d;
  uint8_t servo_angle;
  uint8_t motor_speed;
  uint8_t sensor_distance[SENSOR_COUNT];
  uint8_t battery_voltage;
} car_diag;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  car_diag diagnostics;
  uint8_t send_buf[sizeof(car_diag)];

  diagnostics.key1 = 0xAA;
  diagnostics.key2 = 0xBB;

  /* MCU Configuration--------------------------------------------------------*/
  sysclk_cfg();
  setup_main();
  motor_init();
  uart_init();
//  buzzer_init();
//  servo_init();
  if (sensor_init() != ADC_STAT_OK) {
    while(1); // Lock up
  }

  motor_set_ilim(150);
  motor_set_speed(0);

  while (1)
  {
    diagnostics.s_p = 0;
    diagnostics.s_i = 1;
    diagnostics.s_d = 2;
    diagnostics.servo_angle = 3;
    diagnostics.motor_speed = 4;

    adc_sample_channels();
    sensor_get_values(diagnostics.sensor_distance);
    diagnostics.battery_voltage = battery_get_voltage();

//    updateSteering();
//    updateSpeed();

    memcpy(&send_buf, &diagnostics, sizeof(send_buf));
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
