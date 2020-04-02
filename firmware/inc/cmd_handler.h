#ifndef __cmd_handler_H
#define __cmd_handler_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "types.h"

cmd_status handle_cmd(char *cmd, car_cfg *pc, car_diag *pd);

#ifdef __cplusplus
}
#endif

#endif /*__ __cmd_handler_H */
