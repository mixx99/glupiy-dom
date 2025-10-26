#include "boards.h"
#include "nrf_delay.h"
#include <stdint.h>

#define DEVICE_LABEL 6613 // Device label

#define BLINK_DELAY 250
#define PAUSE_DELAY 750

enum COLOR { YELLOW, RED, GREEN, BLUE };

static inline void blink(int color, int number_to_blink);
static inline uint64_t get_id();

// Returns device id (Label with #ABCD)
static inline uint64_t get_id() { return DEVICE_LABEL; }

// Blink `number_to_blink` times with `color` color.
static inline void blink(int color, int number_to_blink) {
  while (number_to_blink--) {
    bsp_board_led_on(color);
    nrf_delay_ms(BLINK_DELAY);
    bsp_board_led_off(color);
    nrf_delay_ms(BLINK_DELAY);
  }
  bsp_board_led_off(color);
}

int main(void) {
  /* Configure board. */
  bsp_board_init(BSP_INIT_LEDS);

  uint32_t id = get_id();

  uint8_t digits[4]; // Our label-id is 4-digit number.
  digits[0] = id / 1000 % 10; // The highest digit
  digits[1] = id / 100 % 10;
  digits[2] = id / 10 % 10;
  digits[3] = id % 10;        // The lowest digit

  /* Toggle LEDs. */
  while (1) {
    for (int i = 0; i < LEDS_NUMBER; ++i) {
      blink(i, digits[i]);
    }
    nrf_delay_ms(PAUSE_DELAY);
  }
}