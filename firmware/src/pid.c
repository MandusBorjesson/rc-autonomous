#include <stdlib.h>
#include "pid.h"
#include "main.h"

int16_t calc_p(int16_t err, uint16_t k_p) {
  return limit_signal( (err * k_p) / K_ATT );
}

int16_t calc_i(int16_t err, uint16_t k_i) {
  static int32_t integral = 0;

  integral = limit_signal(integral+err / (1000 / LOOP_INTERVAL_MS));

  return limit_signal( (integral * k_i) / K_ATT );
}

int16_t calc_d(int16_t err, uint16_t k_d) {
  static int16_t old_err = 0;

  int derivative = (err - old_err) * (1000 / LOOP_INTERVAL_MS);

  old_err = err;

  return limit_signal( (derivative * k_d) / K_ATT );
}

int8_t calc_y(car_cfg* cfg, car_diag* diag) {
  int16_t err = limit_signal(diag->sensor_distance - cfg->trg_dist);
  diag->err = err;
  diag->s_p = calc_p(err, cfg->k_p);
  diag->s_i = calc_i(err, cfg->k_i);
  diag->s_d = calc_d(err, cfg->k_d);

  return limit_signal( diag->s_p + diag->s_i + diag->s_d ) >> 8;
}

int16_t limit_signal(int32_t s) {
  if ( s > S_MAX )
    return S_MAX;
  if ( s < S_MIN )
    return S_MIN;
  return (uint16_t)s;
}
