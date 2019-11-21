/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>

#include "main.h"
// #include "buzzer.h"
#include "servo.h"
#include "motor.h"

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
  int s_p;
  int s_i;
  int s_d;
  int servo_angle;
  int motor_speed;
  int sensor_distance[3];
  double battery_voltage;
} car_diag;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

/* MCU Configuration--------------------------------------------------------*/
  sysclk_cfg();
  setup_main();
  motor_init();
//  buzzer_init();
//  servo_init();

  motor_set_ilim(150);
  motor_set_speed(0);

  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  GPIOB->MODER = GPIO_MODER_MODER2_0;

  while (1)
  {
   GPIOB->ODR ^= GPIO_ODR_2;
//    getSensorData();
//    getBatteryVoltage();
//    updateSteering();
//    updateSpeed();
//    sendDiagnostics();
    enter_sleep();
  }
}

/**
  * @brief System Clock Configuration, 24 MHz using HSI and PLL
  * @retval None
  */
void sysclk_cfg(void)
{
/*-------------------------- HSI/HSI14 Configuration -----------------------*/
  RCC->CR |= RCC_CR_HSION;

  while ((RCC->CR & RCC_CR_HSIRDY) == 0) {}

  // Clock calibration
  RCC->CR &= ~RCC_CR_HSITRIM;
  RCC->CR |= 16u << RCC_CR_HSITRIM_Pos;

  RCC->CR2 |= RCC_CR2_HSI14DIS;
  RCC->CR2 |= RCC_CR2_HSI14ON;

  // Wait for HSI14
  while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) {}

  // Clock calibration
  RCC->CR2 &= ~RCC_CR2_HSI14TRIM;
  RCC->CR2 |= 16u << RCC_CR2_HSI14TRIM_Pos;

/*-------------------------------- PLL Configuration -----------------------*/
  RCC->CR &= ~RCC_CR_PLLON;

  // Wait for PLL
  while ((RCC->CR & RCC_CR_PLLRDY) != 0) {}

  RCC->CFGR = RCC_CFGR_PLLMUL6 |
               RCC_CFGR_PLLSRC_HSI_DIV2;
  RCC->CFGR2 = RCC_CFGR2_PREDIV_1;

  RCC->CR |= RCC_CR_PLLON;

  // Wait for PLL
  while ((RCC->CR & RCC_CR_PLLRDY) == 0) {}

/*--------------------- CPU/Peripheral Configuration -----------------------*/
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  // Wait for PLL
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}

  RCC->CFGR |= RCC_CFGR_PPRE_DIV1;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {}

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
