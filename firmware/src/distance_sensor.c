#include <stdint.h>
#include "distance_sensor.h"
#include "stm32f0xx.h"

const uint8_t val_2_dist[256] = {
  254, 251, 248, 245, 242, 239, 236, 234, 231, 228, 225, 222, 219, 217, 214, 211,
  208, 205, 202, 200, 193, 187, 181, 175, 168, 162, 156, 150, 143, 137, 131, 125,
  118, 112, 106, 100, 98, 96, 94, 92, 90, 88, 86, 84, 82, 80, 78, 76,
  74, 72, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 49, 48, 48,
  47, 46, 46, 45, 44, 44, 43, 42, 42, 41, 40, 40, 39, 39, 38, 37,
  37, 36, 35, 35, 34, 33, 33, 32, 31, 31, 30, 30, 29, 29, 29, 28,
  28, 28, 28, 27, 27, 27, 27, 26, 26, 26, 26, 25, 25, 25, 25, 24,
  24, 24, 24, 23, 23, 23, 23, 22, 22, 22, 22, 21, 21, 21, 21, 20,
  20, 20, 20, 19, 19, 19, 19, 18, 18, 18, 18, 17, 17, 17, 17, 16,
  16, 16, 16, 15, 15, 15, 15, 14, 14, 14, 14, 13, 13, 13, 13, 12,
  12, 12, 12, 11, 11, 11, 11, 10, 10, 10, 10, 9, 9, 9, 9, 9,
  9, 9, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7,
  7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5,
  5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3,
  3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
  1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int sensor_init(void) {
  /* Configure IOs */
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

  SENSOR_IOBANK->MODER |= (3 << GPIO_MODER_MODER3_Pos); // Analog

  /* Calibrate ADC */
  unsigned long int timeout;

  if ((ADC1->CR & ADC_CR_ADEN) != 0) {
    ADC1->CR |= ADC_CR_ADDIS;
  }

  timeout = 0;
  while ((ADC1->CR & ADC_CR_ADEN) != 0) {
    timeout++;
    if(timeout > TIMEOUT_MAX) {
      return ADC_STAT_EN_FAIL;
    }
  }

  ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN;
  ADC1->CR |= ADC_CR_ADCAL;

  timeout = 0;
  while ((ADC1->CR & ADC_CR_ADCAL) != 0) {
    timeout++;
    if(timeout > TIMEOUT_MAX) {
      return ADC_STAT_ADCAL_FAIL;
    }
  }

  /* ADC Clock selection */
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->CR2 |= RCC_CR2_HSI14ON;

  timeout = 0;
  while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0)
  {
    timeout++;
    if(timeout > TIMEOUT_MAX) {
      return ADC_STAT_RCC_FAIL;
    }
  }
  ADC1->CFGR2 &= (~ADC_CFGR2_CKMODE);

  /* ADC Enable sequence */
  if ((ADC1->ISR & ADC_ISR_ADRDY) != 0) {
    ADC1->ISR |= ADC_ISR_ADRDY; // Clear interrupt if set
  }
  ADC1->CR |= ADC_CR_ADEN;

  timeout = 0;
  while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) {
    timeout++;
    if(timeout > TIMEOUT_MAX) {
      return ADC_STAT_ISR_EN_FAIL;
    }
  }

  return ADC_STAT_OK;
}

int adc_sample_channels(void) {
  ADC1->CHSELR = ADC_CHSELR_CHSEL0 |
                 ADC_CHSELR_CHSEL1 |
                 ADC_CHSELR_CHSEL2 |
                 ADC_CHSELR_CHSEL3 |
                 ADC_CHSELR_CHSEL4 |
                 ADC_CHSELR_CHSEL5 |
                 ADC_CHSELR_CHSEL6 |
                 ADC_CHSELR_CHSEL7 |
                 ADC_CHSELR_CHSEL8 |
                 ADC_CHSELR_CHSEL9;
  ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;
  ADC1->CFGR1 |= ADC_CFGR1_SCANDIR;
  ADC->CCR |= ADC_CCR_VREFEN;

  ADC1->CR |= ADC_CR_ADSTART;

  for (int i = 0; i < 10; i++) {
    unsigned int timeout = 0;
    while ((ADC1->ISR & ADC_ISR_EOC) == 0) {
      timeout++;
      if(timeout > TIMEOUT_MAX) {
        return ADC_STAT_CONV_FAIL;
      }
    }
    adc_buf[i] = (ADC1->DR) >> 4; // Get value, 12 bits -> 8 bits
  }

  return ADC_STAT_OK;
}

uint8_t sensor_get_value(uint8_t off) {
  return val_2_dist[adc_buf[off]];
}
