#include <stdint.h>
#include <math.h>
#include "distance_sensor.h"
#include "stm32f0xx.h"

int sensor_init(void) {
  /* Configure IOs */
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

  SENSOR_IOBANK->MODER |= (3 << GPIO_MODER_MODER3_Pos);  // Analog

  /* Calibrate ADC */
  uint64_t timeout;

  if ((ADC1->CR & ADC_CR_ADEN) != 0) {
    ADC1->CR |= ADC_CR_ADDIS;
  }

  timeout = 0;
  while ((ADC1->CR & ADC_CR_ADEN) != 0) {
    timeout++;
    if (timeout > TIMEOUT_MAX) {
      return ADC_STAT_EN_FAIL;
    }
  }

  ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN;
  ADC1->CR |= ADC_CR_ADCAL;

  timeout = 0;
  while ((ADC1->CR & ADC_CR_ADCAL) != 0) {
    timeout++;
    if (timeout > TIMEOUT_MAX) {
      return ADC_STAT_ADCAL_FAIL;
    }
  }

  /* ADC Clock selection */
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->CR2 |= RCC_CR2_HSI14ON;

  timeout = 0;
  while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) {
    timeout++;
    if (timeout > TIMEOUT_MAX) {
      return ADC_STAT_RCC_FAIL;
    }
  }
  ADC1->CFGR2 &= (~ADC_CFGR2_CKMODE);

  /* ADC Enable sequence */
  if ((ADC1->ISR & ADC_ISR_ADRDY) != 0) {
    ADC1->ISR |= ADC_ISR_ADRDY;  // Clear interrupt if set
  }
  ADC1->CR |= ADC_CR_ADEN;

  timeout = 0;
  while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) {
    timeout++;
    if (timeout > TIMEOUT_MAX) {
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
      if (timeout > TIMEOUT_MAX) {
        return ADC_STAT_CONV_FAIL;
      }
    }
    adc_buf[i] = (ADC1->DR) << 4;  // Get value, 12 bits -> 16 bits
  }

  return ADC_STAT_OK;
}

uint16_t sensor_get_value(uint8_t off) {
  // This formula will linearize the measured value to a value approximately
  // equal to distance(cm) * 366.
  double out = 1000000000 * pow(adc_buf[off], -1.2);

  if (out < 0)
    return 0;

  if (out >= 65535)
    return 65535;

  return floor(out);
}
