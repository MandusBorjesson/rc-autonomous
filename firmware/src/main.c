#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "distance_sensor.h"
#include "cmd_handler.h"
#include "fifo.h"
#include "flash.h"
#include "main.h"
#include "motor.h"
#include "pid.h"
#include "servo.h"
#include "spi.h"
#include "uart.h"

int main(void) {
  memset(&diagnostics, 0, sizeof(car_diag));
  memset(&config, 0, sizeof(car_cfg));
  memset(&cmd_line, 0, sizeof(command_line));
  memset(&rx_fifo, 0 , sizeof(fifo));

  if ( flash_read(&config, sizeof(car_cfg)) != 0 ) {
    /* Print error */
  }

  config.dst.per = LOOP_INTERVAL_MS;
  config.spd.per = LOOP_INTERVAL_MS;
  config.car_state = WAIT;

  /* Start delay */
  for (uint64_t i = 0; i < 1000000; i++) {
    __NOP();
  }

  /* MCU Configuration */
  sysclk_cfg();
  setup_main();
  motor_init();
  uart_init();
  servo_init();
  startpin_init();
  spi_init();
  if (sensor_init() != ADC_STAT_OK) {
    while (1) {}  // Lock up
  }

  motor_set_ilim(255);
  motor_set_speed(0);

  while (1) {
    OE_IOBANK->ODR &= ~(1 << OE_PIN_Pos);

    uint32_t counter = 0xC00003;
    spi_send((char*)&counter, 3);

    adc_sample_channels();
    diagnostics.dist = sensor_get_value(ADC_SENS_OFFS);

    calc_y(&(config.dst), &(diagnostics.dst), diagnostics.dist);
    calc_y(&(config.spd), &(diagnostics.spd), diagnostics.speed);

    char key;
    while (fifo_pop(&key, &rx_fifo) == NONE) {
      uart_handle_key(key, &cmd_line);
    }

    if (cmd_line.status != CMD_NONE) {
      cmd_line.status = handle_cmd(cmd_line.buf, &config,
                                   &diagnostics, cmd_line.status);
      if (cmd_line.status == CMD_NONE) {
        uart_send("\r\n> ");
        memset(cmd_line.buf, 0, CMD_BUF_SZ);
        cmd_line.cur_pos = 0;
        cmd_line.counter = 0;
      }
    }

    if ( startpin_get() == 0 ) {
      config.car_state = RUN;
    }

    if (config.car_state == RUN) {
      motor_set_speed((int8_t)(diagnostics.spd.out/256));
      servo_set_angle((int8_t)(diagnostics.dst.out/256));
    } else {
      motor_set_speed(0);
      servo_set_angle(0);
    }
    OE_IOBANK->ODR |= (1 << OE_PIN_Pos);

    enter_sleep();
  }
}

void sysclk_cfg(void) {
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

void setup_main(void) {
  RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;  // Enable clock
  MAIN_LOOP_TIMER->PSC = 2400;
  MAIN_LOOP_TIMER->ARR = LOOP_INTERVAL_MS;
  MAIN_LOOP_TIMER->DIER = TIM_DIER_UIE;
  MAIN_LOOP_TIMER->EGR |= TIM_EGR_UG;
  MAIN_LOOP_TIMER->CR1 |= TIM_CR1_ARPE |
                          TIM_CR1_CEN;

  NVIC_EnableIRQ(TIM16_IRQn);
  NVIC_SetPriority(TIM16_IRQn, 2);
}

void enter_sleep(void) {
    SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk);
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
    __WFI();
}

void enter_run(void) {
  SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
  __SEV();
}

void startpin_init(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

  // Enable pull-up on start pin
  STARTPIN_IOBANK->PUPDR = 0b01 << GPIO_PUPDR_PUPDR2_Pos;
}

int startpin_get(void) {
  return ((STARTPIN_IOBANK->IDR & GPIO_IDR_2) > 0) ? 1 : 0;
}
