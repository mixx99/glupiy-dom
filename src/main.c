// std
#include <stdint.h>

#include "app.h"
#include "gpiote_control.h"
#include "led_control.h"

int main(void) {
  init_gpio();
  init_gpiote();

  while (1) {
    play_smooth_sequence();
  }
}