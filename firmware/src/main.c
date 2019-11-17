/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>

#include "main.h"
// #include "dac.h"
// #include "spi.h"
// #include "tim.h"
// #include "usart.h"
// #include "gpio.h"
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

/* Private function prototypes -----------------------------------------------*/
//void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

/* MCU Configuration--------------------------------------------------------*/
//  SystemClock_Config();
//  setup_main();
//  buzzer_init();
//  servo_init();
  motor_init();
  int8_t tmp = 0;
  int dir = 1;
  volatile int dummy;
//  SERVO_IOBANK->MODER = GPIO_MODER_MODER0_0; // Alternate function
//  SERVO_IOBANK->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_0 |
//                           GPIO_OSPEEDER_OSPEEDR0_1;

  motor_set_ilim(150);
  motor_set_speed(0);
  while (1)
  {
    for (int i = 0; i < 1000; i++) {
      for (int j = 0; j < 30; j++) {
        dummy++;
      }
    }
    tmp += dir;
    if (abs(tmp) >= 127) {
      dir = -dir;
    }
    //motor_set_speed(tmp);

    //enter_Sleep();
    //tmp = SERVO_TIM->CNT;
//    SERVO_IOBANK->ODR ^= GPIO_ODR_0;
//    getSensorData();
//    getBatteryVoltage();
//    updateSteering();
//    updateSpeed();
//    sendDiagnostics();
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */

// void SystemClock_Config(void)
// {
//   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//   RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

//   /* Initializes the CPU, AHB and APB busses clocks */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
//   RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//   RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
//   RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//   RCC_OscInitStruct.HSI14CalibrationValue = 16;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//   RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
//   RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
//   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//   {
//     Error_Handler();
//   }

//   /* Initializes the CPU, AHB and APB busses clocks */
//   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                               |RCC_CLOCKTYPE_PCLK1;
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

//   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
//   PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
//   if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//   {
//     Error_Handler();
//   }
// }

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {}

void enter_Sleep( void )
{
    /* Configure low-power mode */
    SCB->SCR &= ~( SCB_SCR_SLEEPDEEP_Msk );  // low-power mode = sleep mode
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;     // reenter low-power mode after ISR

    /* Ensure Flash memory stays on */
//    FLASH->ACR &= ~FLASH_ACR_SLEEP_PD;
    __WFI();  // enter low-power mode
}

void setup_main(void) {
  RCC->APB2ENR |= RCC_APB2ENR_TIM16EN; // Enable clock
  MAIN_LOOP_TIMER->PSC = 24000;
  MAIN_LOOP_TIMER->CCR1 = LOOP_INTERVAL_MS;
  MAIN_LOOP_TIMER->DIER = TIM_DIER_UIE;
  MAIN_LOOP_TIMER->CR2 = 0;
  MAIN_LOOP_TIMER->CR1 = TIM_CR1_CEN;
}
