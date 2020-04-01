#ifndef __fifo_H
#define __fifo_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "types.h"

typedef enum fifo_err {NONE, FULL, EMPTY} fifo_err;

fifo_err fifo_put(char val, fifo* pf);
fifo_err fifo_pop(char* val, fifo* pf);

#ifdef __cplusplus
}
#endif

#endif /*__ __fifo_H */
