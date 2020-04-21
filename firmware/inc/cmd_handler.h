#ifndef FIRMWARE_INC_CMD_HANDLER_H_
#define FIRMWARE_INC_CMD_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

cmd_status handle_cmd(char *cmd, car_cfg *pc, car_diag *pd, cmd_status stat);

#ifdef __cplusplus
}
#endif

#endif  // FIRMWARE_INC_CMD_HANDLER_H_
