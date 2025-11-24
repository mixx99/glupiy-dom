#ifndef GPIOTE_CONTROL_H_
#define GPIOTE_CONTROL_H_

enum BUTTON_LAST_STATUS {
  BUTTON_NONE,
  BUTTON_PRESSED,
  BUTTON_RELEASED,
  BUTTON_DOUBLE_CLICK,
};

void init_gpiote();
int get_button_status();

#endif