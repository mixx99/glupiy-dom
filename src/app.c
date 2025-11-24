#include "app.h"

// std
#include <stdint.h>

#include "gpiote_control.h"
#include "led_control.h"

const static int DEVICE_LABEL = 6613; // Device label
static volatile int mode = BUTTON_NONE;

void play_smooth_sequence() {
  static uint32_t id = DEVICE_LABEL;

  static uint8_t digits[4];   // Our label-id is 4-digit number.
  digits[0] = id / 1000 % 10; // The highest digit
  digits[1] = id / 100 % 10;
  digits[2] = id / 10 % 10;
  digits[3] = id % 10; // The lowest digit
  static uint32_t sequense[4] = {LED_RED, LED_GREEN, LED_BLUE, LED_YELLOW};

  static int current_index = 0;
  static int blink_counter = 0;
  if (mode == BUTTON_PRESSED || mode == BUTTON_DOUBLE_CLICK) {
    smooth_blink(sequense[current_index], ONE_SEC_US);
    blink_counter++;
    if (blink_counter >= digits[current_index]) {
      current_index = (current_index + 1) % 4;
      blink_counter = 0;
    }
  }
}

void set_play_mode(uint32_t new_mode) {
  if (mode == BUTTON_DOUBLE_CLICK && new_mode == BUTTON_DOUBLE_CLICK) {
    mode = BUTTON_NONE;
    return;
  }
  mode = new_mode;
}