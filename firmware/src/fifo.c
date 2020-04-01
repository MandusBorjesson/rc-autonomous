#include "fifo.h"

fifo_err fifo_put(char val, fifo* pf) {
   if (pf->head + 1 == pf->tail) return FULL;
   pf->head = (pf->head + 1) % FIFO_SZ;
   pf->buf[pf->head] = val;
   return NONE;
}

fifo_err fifo_pop(char* val, fifo* pf) {
   if (pf->head == pf->tail) return EMPTY;
   pf->tail = (pf->tail + 1) % FIFO_SZ;
   *val = pf->buf[pf->tail];
   return NONE;
}
