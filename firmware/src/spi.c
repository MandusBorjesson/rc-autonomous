#include "spi.h"
#include "stm32f0xx.h"

void spi_init(void) {
  uint32_t tmp;

  /* GPIO Setup */
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  tmp = RCC->AHBENR & RCC_AHBENR_GPIOAEN;
  (void)(tmp);

  // GPIOA: SCK
  GPIOA->MODER &= ~(0x03 << GPIO_MODER_MODER5_Pos);
  GPIOA->MODER |= (0x02 << GPIO_MODER_MODER5_Pos);
  GPIOA->AFR[0] &= ~(0x07 << GPIO_AFRL_AFSEL5_Pos);

  // GPIOB: MOSI
  GPIOB->MODER &= ~(0x03 << GPIO_MODER_MODER5_Pos);
  GPIOB->MODER |= (0x02 << GPIO_MODER_MODER5_Pos);
  GPIOB->AFR[0] &= ~(0x07 << GPIO_AFRL_AFSEL5_Pos);

  OE_IOBANK->MODER &= ~(0x03 << (OE_PIN_Pos*2));
  OE_IOBANK->MODER |= (0x01 << (OE_PIN_Pos*2));
  RCK_IOBANK->MODER &= ~(0x03 << (RCK_PIN_Pos*2));
  RCK_IOBANK->MODER |= (0x01 << (RCK_PIN_Pos*2));

  OE_IOBANK->ODR |= (1 << OE_PIN_Pos);
  RCK_IOBANK->ODR &= ~(1 << RCK_PIN_Pos);

  /* DMA Setup */
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  tmp = RCC->AHBENR & RCC_AHBENR_DMA1EN;
  (void)(tmp);

  DMA1_Channel3->CCR = DMA_CCR_MINC |
                       DMA_CCR_DIR |
                       DMA_CCR_TCIE;

  DMA1_Channel3->CPAR = (uint32_t) (&(SPI_EXT->DR));

  NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  NVIC_SetPriority(DMA1_Channel2_3_IRQn,0);

  /* SPI Setup */
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  tmp = RCC->APB2ENR & RCC_APB2ENR_SPI1EN;
  (void)(tmp);

  SPI_EXT->CR1 = SPI_CR1_MSTR |
              SPI_CR1_SSI |
              SPI_CR1_SSM |
              SPI_CR1_LSBFIRST |
              (7<<SPI_CR1_BR_Pos);
  SPI_EXT->CR2 = SPI_CR2_TXDMAEN;

  SPI_EXT->CR1 |= SPI_CR1_SPE;
}

void spi_send(char *data, uint16_t size) {
  RCK_IOBANK->ODR &= ~(1<<RCK_PIN_Pos);

  DMA1_Channel3->CCR &= ~DMA_CCR_EN;

  DMA1_Channel3->CMAR = (uint32_t) (data);
  DMA1_Channel3->CNDTR = size;
  DMA1->IFCR |= DMA_IFCR_CGIF3;

  DMA1_Channel3->CCR |= DMA_CCR_EN;
}
