#include "cmd_handler.h"
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "flash.h"
#include "uart.h"
#include "pid.h"

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
}

void show_info(void) {
}

void set_parameter(char *par, car_cfg *pc) {
}

void get_parameters(car_cfg *pc) {
}

void plot_parameter(char *par) {
}

cmd_status handle_cmd(char *cmd, car_cfg *pc, car_diag *pd) {
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
      plot_parameter(arg);
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
