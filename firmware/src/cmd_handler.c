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

  while (*val) {
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
      if (*(match-1) != '*') {
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
      uart_send("\n\rAbreviations: [dst/spd]: distance from wall/speed");
      uart_send("\n\r");
      uart_send("\n\rParameter       Range      Description");
      uart_send("\n\r[dst/spd].k_p   0 - 16     PID partial amplification");
      uart_send("\n\r[dst/spd].k_i   0 - 16     PID integral aplification");
      uart_send("\n\r[dst/spd].k_d   0 - 16     PID derivative amplification");
      uart_send("\n\r[dst/spd].trg   0 - 65535  PID target");
  } else if (match_cmd(cmd, "get")) {
      uart_send("\n\rUsage: 'get'");
      uart_send("\n\rUsed for getting current configiration");
  } else if (match_cmd(cmd, "plot")) {
      uart_send("\n\rUsage: 'plot par1 par2 ...'");
      uart_send("\n\rUsed for plotting parameters. Available parameters:");
      uart_send("\n\r");
      uart_send("\n\r[dst/spd].s_p  -  PID partial signal");
      uart_send("\n\r[dst/spd].s_i  -  PID integral signal");
      uart_send("\n\r[dst/spd].s_d  -  PID derivative signal");
      uart_send("\n\r[dst/spd].err  -  PID error");
      uart_send("\n\r[dst/spd].out  -  PID output");
      uart_send("\n\rspeed          -  Current car speed in mm/s");
      uart_send("\n\rdist           -  Current sensor distance");
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
      uart_send("\n\rUsed for saving current configuration to non-volatile memory.");
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
  if (!par || !val) {
    show_help("set");
    return;
  }

  pid_cfg *tmp;

  if (match_cmd(par, "dst.***")) {
    tmp = &(pc->dst);
  } else if (match_cmd(par, "spd.***")) {
    tmp = &(pc->spd);
  } else {
    uart_send("\n\rUnknown parameter!");
    return;
  }

  if (match_cmd(par+4, "k_p")) {
    tmp->k_p = strtokval(val);
  } else if (match_cmd(par+4, "k_i")) {
    tmp->k_i = strtokval(val);
  } else if (match_cmd(par+4, "k_d")) {
    tmp->k_d = strtokval(val);
  } else if (match_cmd(par+4, "trg")) {
    tmp->trg = (uint16_t)atol(val);
  } else {
    uart_send("\n\rUnknown parameter!");
  }
}

void get_parameters(car_cfg *pc) {
  char buf[8];

  uart_send("\n\rdst.k_p: ");
  printkval(pc->dst.k_p);
  uart_send("\n\rdst.k_i: ");
  printkval(pc->dst.k_i);
  uart_send("\n\rdst.k_d: ");
  printkval(pc->dst.k_d);
  uart_send("\n\rdst.trg: ");
  uart_send(itoa(pc->dst.trg, buf, 10));
  uart_send("\n\r");
  uart_send("\n\rspd.k_p: ");
  printkval(pc->spd.k_p);
  uart_send("\n\rspd.k_i: ");
  printkval(pc->spd.k_i);
  uart_send("\n\rspd.k_d: ");
  printkval(pc->spd.k_d);
  uart_send("\n\rspd.trg: ");
  uart_send(itoa(pc->spd.trg, buf, 10));
  uart_send("\n\r");
  uart_send("\n\rcar_state: ");
  switch (pc->car_state) {
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
  static uint8_t old_x[16] = {0};
  uint8_t y_val = 0;
  uint8_t x_val;
  uint8_t zero;

  if (stat == CMD_PEND) {
    uart_send(ESC_ERD);
    uart_send(ESC_CHD);
  }

  while (par) {
    x_val = 0;
    y_val++;
    zero = 32;

    pid_diag *tmp = NULL;

    if (match_cmd(par, "dist")) {
        x_val = pd->dist / 1024;
        zero = 0;
    } else if (match_cmd(par, "speed")) {
        x_val = pd->speed / 1024 + 32;
    } else if (match_cmd(par, "dst.***")) {
        tmp = &(pd->dst);
    } else if (match_cmd(par, "spd.***")) {
        tmp = &(pd->spd);
    }

    if(tmp) {
      if (match_cmd(par+4, "s_p")) {
        x_val = tmp->s_p / 1024 + 32;
      } else if (match_cmd(par+4, "s_i")) {
        x_val = tmp->s_i / 1024 + 32;
      } else if (match_cmd(par+4, "s_d")) {
        x_val = tmp->s_d / 1024 + 32;
      } else if (match_cmd(par+4, "err")) {
        x_val = tmp->err_p / 1024 + 32;
      } else if (match_cmd(par+4, "out")) {
        x_val = tmp->out / 1024 + 32;
      }
    }

    print_xy("0", zero+PLOT_NAME_SZ+2, y_val);
    print_xy(" ", old_x[y_val]+PLOT_NAME_SZ+2, y_val);
    print_xy("*", x_val+PLOT_NAME_SZ+2, y_val);

    if (stat == CMD_PEND) {
      uart_send("\r");
      uart_send_until(par, ' ');
      print_xy("|", PLOT_NAME_SZ+1, y_val);
      print_xy("|", PLOT_NAME_SZ+66, y_val);
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
