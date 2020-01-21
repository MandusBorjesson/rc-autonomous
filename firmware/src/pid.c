#include <stdlib.h>
#include "pid.h"

int8_t calc_p(int8_t err, uint8_t k_p) {
  int out = err * (k_p/2);
  return out / 128;
}

int8_t calc_i(int8_t err, uint8_t k_i) {
  return 0;
}

int8_t calc_d(int8_t err, uint8_t k_d) {
  return 0;
}

int8_t calc_y(car_cfg* cfg, car_diag* diag) {
  int8_t err = (diag->sensor_distance - cfg->trg_dist) / 2;
  diag->s_p = calc_p(err, cfg->k_p);
  diag->s_i = calc_i(err, cfg->k_i);
  diag->s_d = calc_d(err, cfg->k_d);

  int sum = diag->s_p + diag->s_i + diag->s_d;

  if (abs(sum) > cfg->max_spd) {
    sum = (sum > 0) ? cfg->max_spd : -cfg->max_spd;
  }

  return sum;
}

