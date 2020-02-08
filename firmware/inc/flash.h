#ifndef __FLASH_H
#define __FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define USR_MEM_BASE 0x800FC00
#define USR_MEM_SZ 1000
#define FLASH_FKEY1 0x45670123
#define FLASH_FKEY2 0xCDEF89AB

int flash_write(void* data, uint32_t len);
int flash_read(void* data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H */
