// std
#include <stdint.h>

// esl-nsdk
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "gpio_control.h"

#define DEVICE_LABEL 6613 // Device label

static inline uint64_t get_id();

// Returns device id (Label with #ABCD)
static inline uint64_t get_id() { return DEVICE_LABEL; }

int main(void) {
  init_gpio();

  uint32_t id = get_id();

  uint8_t digits[4];          // Our label-id is 4-digit number.
  digits[0] = id / 1000 % 10; // The highest digit
  digits[1] = id / 100 % 10;
  digits[2] = id / 10 % 10;
  digits[3] = id % 10; // The lowest digit
  uint32_t sequense[4] = {LED_RED, LED_GREEN, LED_BLUE, LED_YELLOW};

  int current_index = 0;
  int blink_counter = 0;
  while (1) {
    if (is_button_pressed()) {
      blink_and_stay_on_if_button_not_pressed(sequense[current_index]);
      blink_counter++;
      if (blink_counter >= digits[current_index]) {
        current_index = (current_index + 1) % 4;
        blink_counter = 0;
      }
    }
    nrf_delay_ms(BLINK_DELAY);
  }
}