#ifndef __distance_sensor_H
#define __distance_sensor_H

#ifdef __cplusplus
 extern "C" {
#endif

#define SENSOR_ADC ADC1
#define ADC_SENS_OFFS 1
#define SENSOR_IOBANK GPIOA
#define TIMEOUT_MAX 1000000

enum adc_stat {
  ADC_STAT_EN_FAIL = -1,
  ADC_STAT_ADCAL_FAIL = -2,
  ADC_STAT_ISR_EN_FAIL = -3,
  ADC_STAT_RCC_FAIL = -4,
  ADC_STAT_CONV_FAIL = -5,
  ADC_STAT_OK = 0
};

uint8_t adc_buf[10];

int sensor_init(void);
int adc_sample_channels(void);
uint8_t sensor_get_value(uint8_t off);

#ifdef __cplusplus
}
#endif

#endif /*__ distance_sensor_H */
