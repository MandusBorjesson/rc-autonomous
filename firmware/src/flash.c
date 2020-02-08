#include <string.h>
#include "flash.h"
#include "stm32f0xx.h"

static int flash_unlock() {
  /* TODO: timeout handling */
  while ((FLASH->SR & FLASH_SR_BSY) != 0) {}

  if ((FLASH->CR & FLASH_CR_LOCK) != 0) {
    FLASH->KEYR = FLASH_FKEY1;
    FLASH->KEYR = FLASH_FKEY2;
  }
  return 0;
}

static int flash_page_erase(uint32_t offs) {
  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = offs;
  FLASH->CR |= FLASH_CR_STRT;

  /* TODO: timeout handling */
  while ((FLASH->SR & FLASH_SR_BSY) != 0) {}

  if ((FLASH->SR & FLASH_SR_EOP) != 0) {
    FLASH->SR = FLASH_SR_EOP;
  }
  else {
    /* TODO: error handling */
  }
  FLASH->CR &= ~FLASH_CR_PER;
  return 0;
}

static int flash_program(void* data, uint32_t len) {
  len = (len+1)/2; // Len is in bytes, div by 4 and round up to make uint16

  if ( len > USR_MEM_SZ )
    return -1;

  FLASH->CR |= FLASH_CR_PG;

  for (uint32_t i = 0; i < len; i++) {
    *((__IO uint16_t*)(USR_MEM_BASE) + i) = *((uint16_t*)data+i);

    /* TODO: timeout handling */
    while ((FLASH->SR & FLASH_SR_BSY) != 0) {}
  }

  if ((FLASH->SR & FLASH_SR_EOP) != 0) {
    FLASH->SR = FLASH_SR_EOP;
  }
  else
  {
    /* TODO: error handling */
  }
  FLASH->CR &= ~FLASH_CR_PG;
  return 0;
}

int flash_write(void* data, uint32_t len) {
  int err = 0;
  err |= flash_unlock();
  err |= flash_page_erase(USR_MEM_BASE);
  err |= flash_program(data, len);
  return err;
}

int flash_read(void* data, uint32_t len) {
  memcpy(data, (void*)USR_MEM_BASE, len);

  for (int i = 0; i < len; i++) {
    if (*((uint8_t*)data+i) != 0xFF)
      return 0;
  }

  // If all data is 0xFF, memory is most likely unprogrammed, return -1
  return -1;
}

