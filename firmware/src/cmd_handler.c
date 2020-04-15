#include "cmd_handler.h"
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "flash.h"
#include "uart.h"
#include "pid.h"

uint16_t strtokval(char *val) {
  uint8_t decimal_point = 0;
  uint8_t offset = 0;
  uint32_t value = 0;

  while(*val) {
    if (isdigit(*val)) {
      value = value * 10 + (*val-'0');
      offset++;
    } else if (*val == '.') {
      if (decimal_point) {
        return 0;
      } else {
        decimal_point = offset;
      }
    } else if (isspace(*val)) {
        break;
    } else {
      return 0;
    }
    val++;
  }

  decimal_point = decimal_point ? decimal_point : offset;
  uint32_t tmp = value*K_ATT/(pow(10, offset - decimal_point));
  return (tmp > K_MAX) ? K_MAX : tmp;
}

void printkval(uint16_t val) {
  char buf[8] = {0};
  uart_send(itoa(val/K_ATT, buf, 10));
  uart_send(".");
  int decimal = ((val%K_ATT)*100) / K_ATT;
  itoa(decimal, buf, 10);
  if (decimal < 10) {
    uart_send("0");
  }
  uart_send(buf);
}

void print_xy(char *line, uint8_t x, uint8_t y) {
    char buf[8];
    uart_send("\x1b[");
    uart_send(itoa(y, buf, 10));
    uart_send(";");
    uart_send(itoa(x, buf, 10));
    uart_send("H");
    uart_send(line);
}

char match_cmd(char *cmd, char *match) {
  while (*match !=0) {
    if (*cmd++ != *match++) {
      if (*(match-1) != '*'){
        return 0;
      }
    }
  }
  if (*cmd == ' ' || *cmd == 0) {
    return 1;
  }
  return 0;
}

char* find_next_word(char *cmd) {
  if (!cmd) {
    return NULL;
  }
  // Move past current word
  while (isgraph(*cmd)) {
    cmd++;
    if (*cmd == 0) {
      return NULL;
    }
  }
  // Find start of next word
  while (isblank(*cmd)) {
    cmd++;
    if (*cmd == 0) {
      return NULL;
    }
  }
  return cmd;
}

void show_help(char *cmd) {
  if (match_cmd(cmd, "set")) {
      uart_send("\n\rUsage: 'set par val'");
      uart_send("\n\rUsed for setting parameters. Available parameters:");
      uart_send("\n\r");
      uart_send("\n\rParameter Range     Description");
      uart_send("\n\rk_p       0 - 16    Steering PID partial amplification");
      uart_send("\n\rk_i       0 - 16    Steering PID integral aplification");
      uart_send("\n\rk_d       0 - 16    Steering PID derivative amplification");
      uart_send("\n\rmax_spd   0 - 255   Max vehicle speed");
      uart_send("\n\rtrg_dist  0 - 65535 Target distance for steering PID");
  } else if (match_cmd(cmd, "get")) {
      uart_send("\n\rUsage: 'get'");
      uart_send("\n\rUsed for getting current configiration");
      uart_send("\n\r");
      uart_send("\n\rParameter Range     Description");
      uart_send("\n\rk_i       0 - 16    Steering PID integral aplification");
      uart_send("\n\rk_p       0 - 16    Steering PID partial amplification");
      uart_send("\n\rk_d       0 - 16    Steering PID derivative amplification");
      uart_send("\n\rtrg_dist  0 - 65535 Target distance for steering PID");
      uart_send("\n\rmax_spd   0 - 255   Max vehicle speed");
  } else if (match_cmd(cmd, "plot")) {
      uart_send("\n\rUsage: 'plot par1 par2 ...'");
      uart_send("\n\rUsed for plotting parameters. Available parameters:");
      uart_send("\n\r");
      uart_send("\n\rs_p         - Steering PID partial signal");
      uart_send("\n\rs_i         - Steering PID integral signal");
      uart_send("\n\rs_d         - Steering PID derivative signal");
      uart_send("\n\rdist        - Distance measured by sensors");
      uart_send("\n\rerr         - Control error");
      uart_send("\n\rmotor_speed - Speed of the motor");
      uart_send("\n\rservo_angle - Steering angle of the servo");
  } else if (match_cmd(cmd, "info")) {
      uart_send("\n\rUsage: 'info'");
      uart_send("\n\rUsed for getting information about the system");
  } else if (match_cmd(cmd, "start")) {
      uart_send("\n\rUsage: 'start'");
      uart_send("\n\rUsed to get the car into 'RUN' mode.");
  } else if (match_cmd(cmd, "stop")) {
      uart_send("\n\rUsage: 'stop'");
      uart_send("\n\rUsed to get the car into 'STOP' mode.");
  } else if (match_cmd(cmd, "save")) {
      uart_send("\n\rUsage: 'save'");
      uart_send("\n\rUsed for saving current confirugartion to non-volatile memory.");
  } else {
      uart_send("\n\rUsage: 'help cmd'");
      uart_send("\n\rGet information about commands");
      uart_send("\n\r");
      uart_send("\n\rAvailable commands:");
      uart_send("\n\r  set");
      uart_send("\n\r  get");
      uart_send("\n\r  plot");
      uart_send("\n\r  help");
      uart_send("\n\r  info");
      uart_send("\n\r  start");
      uart_send("\n\r  stop");
      uart_send("\n\r  save");
  }
}

void show_info(void) {
  uart_send("\n\r");
  uart_send("\n\roMMMMMNy/-                                                         ./smMMMMMd");
  uart_send("\n\r  +MMMMMMMMMMMMMNNNdyso/:--..```             ```..-:/+oyhmNNMMMMMMMMMMMMMh");
  uart_send("\n\r     `/ydNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMmy+.");
  uart_send("\n\r   +/`  `.:+yhmNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMmdyo:.`  `:o`");
  uart_send("\n\r  /MMms:.    ``.:+yhmNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMmdyo/-.`    `-odMMy");
  uart_send("\n\r yMMMMMMMMMMy              `-odMMMMMMMMMMMMMMmy:.              +MMMMMMMMMMm`");
  uart_send("\n\r:MMMMMMMMMN.     /dNMMMMMmo`   -dMMMMMMMMMMm/`   /dNMMMMNd+`    `dMMMMMMMMMs");
  uart_send("\n\r`NMMMMMMMM/     dMMMMMMMMMMM-    :MMMMMMMMs     NMMMMMMMMMMM`    `MMMMMMMMM/");
  uart_send("\n\r sMMMMMMMMy     oMMMMMMMMMMm`    -MMMMMMMM+     yMMMMMMMMMMd     /MMMMMMMMd`");
  uart_send("\n\r `hMMMMMMMM:     omMMMMMMNh.     sMMMMMMMMd`    `sNMMMMMMNy.    .mMMMMMMMm.");
  uart_send("\n\r   `oNMMMMMMMy.               .yMMMMMmdMMMMMd:`              .oNMMMMMMNy.");
  uart_send("\n\r       -smMMMMMMMNmhysoosydNMMMMNms:    .odNMMMMNdhssossydNMMMMMMMNy:");
  uart_send("\n\r          `/ymNMMMMMMMMMMMMMMMMMo          :MMMMMMMMMMMMMMMMMNNh+.");
  uart_send("\n\r               ./+syyyysso+/:dMMN.        `hMMMMN/:/+osyso/-`");
  uart_send("\n\r                           `yMMMMm-      .yMMMMd.");
  uart_send("\n\r  RC-Autonomous              /mMMMMy:` .sNMMMNo");
  uart_send("\n\r                                .sNMMMMMMMy-\r  ");
  uart_send(COMPILEDATE);
  uart_send("   ");
  uart_send("\n\r                                  `/mMMNo.");
  uart_send("\n\r\n\r  ");
  uart_send(GITCOMMIT);
  uart_send(" (");
  uart_send(GITBRANCH);
  uart_send(")");
}

void set_parameter(char *par, car_cfg *pc) {
  char* val = find_next_word(par);
  if(!par || !val) {
    show_help("set");
    return;
  }

  if (match_cmd(par, "k_*")) {
    if(par[2] == 'p') {
      pc->k_p = strtokval(val);
    } else if (par[2] == 'i') {
      pc->k_i = strtokval(val);
    } else if (par[2] == 'd') {
      pc->k_d = strtokval(val);
    } else {
      uart_send("\n\rUnknown parameter!");
    }
  } else if (match_cmd(par, "max_spd")) {
    pc->max_spd = (uint8_t)atol(val);
  } else if (match_cmd(par, "trg_dist")) {
    pc->trg_dist = (uint16_t)atol(val);
  } else {
    uart_send("\n\rUnknown parameter!");
  }
}

void get_parameters(car_cfg *pc) {
  char buf[8] = {};

  uart_send("\n\rcar_cfg");

  uart_send("\n\r k_p: ");
  printkval(pc->k_p);
  uart_send("\n\r k_i: ");
  printkval(pc->k_i);
  uart_send("\n\r k_d: ");
  printkval(pc->k_d);

  uart_send("\n\r max_spd: ");
  uart_send(itoa(pc->max_spd, buf, 10));

  uart_send("\n\r trg_dist: ");
  uart_send(itoa(pc->trg_dist, buf, 10));

  uart_send("\n\r car_state: ");
  switch (pc->car_state)
  {
  case WAIT:
    uart_send("WAIT");
    break;
  case STOP:
    uart_send("STOP");
    break;
  case RUN:
    uart_send("RUN");
    break;
  default:
    uart_send("UNKNOWN");
    break;
  }
}

void plot_parameter(char *base, car_diag *pd, cmd_status stat) {
  char *par = find_next_word(base);
  static uint8_t old_x[8] = {0};
  uint8_t y_val = 0;
  uint8_t x_val;
  uint8_t zero;

  if (stat == CMD_PEND) {
    uart_send(ESC_ERD);
    uart_send(ESC_CHD);
  }

  while(par) {
    x_val = 0;
    y_val++;
    zero = 32;

    if (match_cmd(par, "dist")) {
        x_val = pd->dist / 1024;
        zero = 0;
    } else if (match_cmd(par, "err")) {
        x_val = pd->err / 1024 + 32;
    } else if (match_cmd(par, "s_*")) {
      if(par[2] == 'p') {
        x_val = pd->s_p / 1024 + 32;
      } else if (par[2] == 'i') {
        x_val = pd->s_i / 1024 + 32;
      } else if (par[2] == 'd') {
        x_val = pd->s_d / 1024 + 32;
      }
    } else if (match_cmd(par, "servo_angle")) {
        x_val = pd->servo_angle / 4 + 32;
    } else if (match_cmd(par, "motor_speed")) {
        x_val = pd->motor_speed / 4 + 32;
    }

    print_xy("0", zero+6, y_val);
    print_xy(" ", old_x[y_val]+5, y_val);
    print_xy("*", x_val+5, y_val);

    if (stat == CMD_PEND) {
      uart_send("\r");
      uart_send_sz(par, 3);
      uart_send(" |");
      print_xy("|", 69, y_val);
    }

    old_x[y_val] = x_val;
    par = find_next_word(par);
  }
}

cmd_status handle_cmd(char *cmd, car_cfg *pc, car_diag *pd, cmd_status stat) {
  char *base = NULL;
  char *arg = NULL;

  if (isgraph(*cmd)) {
    base = cmd;
  } else {
    base = find_next_word(cmd);
    if ( base == NULL )
      return CMD_NONE;
  }

  arg = find_next_word(base);

  if (match_cmd(base, "help")) {
      show_help(arg);
  } else if (match_cmd(base, "info")) {
      show_info();
  } else if (match_cmd(base, "set")) {
      set_parameter(arg, pc);
  } else if (match_cmd(base, "get")) {
      get_parameters(pc);
  } else if (match_cmd(base, "plot")) {
      plot_parameter(base, pd, stat);
      return CMD_BUSY;
  } else if (match_cmd(base, "start")) {
      pc->car_state = RUN;
  } else if (match_cmd(cmd, "stop")) {
      pc->car_state = STOP;
  } else if (match_cmd(cmd, "save")) {
      flash_write(pc, sizeof(car_cfg));
  } else if (strlen(cmd) != 0) {
     uart_send("\n\rUnknown command!");
  }
  return CMD_NONE;
}
