#include <stdlib.h>
#include "pid.h"
#include "main.h"

void calc_y(pid_cfg* cfg, pid_diag* diag, uint16_t val) {
  diag->err_p = limit_signal(cfg->trg - val);

  // s_p
  diag->s_p = limit_signal( (diag->err_p * cfg->k_p) / K_ATT );

  // s_i
  diag->err_i = limit_signal(diag->err_i+diag->err_p / (1000 / cfg->per));
  diag->s_i = limit_signal( (diag->err_i * cfg->k_i) / K_ATT );

  // s_d
  int16_t derivative = (diag->err_p - diag->err_d) * (1000 / cfg->per);
  diag->err_d = diag->err_p;
  diag->s_d = limit_signal( (derivative * cfg->k_d) / K_ATT );

  diag->out = limit_signal( diag->s_p + diag->s_i + diag->s_d );
}

int16_t limit_signal(int32_t s) {
  if ( s > S_MAX )
    return S_MAX;
  if ( s < S_MIN )
    return S_MIN;
  return (uint16_t)s;
}
