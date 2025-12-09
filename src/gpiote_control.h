#ifndef GPIOTE_CONTROL_H_
#define GPIOTE_CONTROL_H_

typedef enum {
  BUTTON_NONE,
  BUTTON_PRESSED,
  BUTTON_RELEASED,
  BUTTON_DOUBLE_CLICK,
} BUTTON_STATUS;

void init_gpiote();

#endif
