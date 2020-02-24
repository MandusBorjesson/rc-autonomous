#include <stdlib.h>
#include "pid.h"

int8_t calc_p(int8_t err, uint8_t k_p) {
  return (err * k_p) / 255;
}

int8_t calc_i(int8_t err, uint8_t k_i) {
  static int integral = 0;

  integral += err;

  // Limit range
  integral = (integral > 127) ? 127 : integral;
  integral = (integral < -127) ? -127 : integral;

  return (integral * k_i) / 255;
}

int8_t calc_d(int8_t err, uint8_t k_d) {
  static int8_t old_err = 0;

  int derivative = err - old_err;

  // Limit range
  derivative = (derivative > 127) ? 127 : derivative;
  derivative = (derivative < -127) ? -127 : derivative;

  old_err = err;

  return (derivative * k_d) / 255;
}

int8_t calc_y(car_cfg* cfg, car_diag* diag) {
  int8_t err = (diag->sensor_distance - cfg->trg_dist) / 2;
  diag->err = err;
  diag->s_p = calc_p(err, cfg->k_p);
  diag->s_i = calc_i(err, cfg->k_i);
  diag->s_d = calc_d(err, cfg->k_d);

  int sum = diag->s_p + diag->s_i + diag->s_d;

  // Limit range
  sum = (sum > 127) ? 127 : sum;
  sum = (sum < -127) ? -127 : sum;

  return sum;
}

