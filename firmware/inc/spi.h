#ifndef FIRMWARE_INC_SPI_H_
#define FIRMWARE_INC_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f0xx.h"

#define SPI_EXT SPI1

#define OE_IOBANK GPIOB
#define RCK_IOBANK GPIOB

#define OE_PIN_Pos 7
#define RCK_PIN_Pos 6

void spi_init(void);

void spi_send(char *data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif  // FIRMWARE_INC_SPI_H_
