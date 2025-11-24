#include "led_control.h"

// esl-nsdk
#include "nrf_delay.h"
#include "nrfx_systick.h"

// Init all the GPIO pins and systick.
void init_gpio() {
  nrf_gpio_cfg_output(LED_RED);
  nrf_gpio_cfg_output(LED_GREEN);
  nrf_gpio_cfg_output(LED_BLUE);
  nrf_gpio_cfg_output(LED_YELLOW);

  nrf_gpio_cfg_input(BUTTON, NRF_GPIO_PIN_PULLUP);

  nrfx_systick_init();

  turn_off_every_led();
}

// Simple blink with `pin_color` color with BLINK_DELAY delay.
void blink(uint32_t pin_color) {
  turn_off_every_led();
  LED_ON(pin_color);
  nrf_delay_ms(BLINK_DELAY);
  LED_OFF(pin_color);
}

// Turns off every led.
void turn_off_every_led() {
  LED_OFF(LED_RED);
  LED_OFF(LED_GREEN);
  LED_OFF(LED_BLUE);
  LED_OFF(LED_YELLOW);
}

// Returns 1 if button pressed, zero otherwise.
// int is_button_pressed() { return nrf_gpio_pin_read(BUTTON) == 0; }

void smooth_blink(uint32_t pin_color, uint32_t us_total_period) {
  nrfx_systick_state_t start_time, current_time, last_oper;
  uint32_t period = 100; // us
  uint32_t t_on = 0;
  uint32_t diff = 1;
  nrfx_systick_get(&start_time);
  nrfx_systick_get(&last_oper);
  while (1) {
    nrfx_systick_get(&current_time);
    LED_ON(pin_color);
    while (!nrfx_systick_test(&current_time, t_on))
      ;

    nrfx_systick_get(&current_time);
    LED_OFF(pin_color);
    while (!nrfx_systick_test(&current_time, period - t_on))
      ;

    if (nrfx_systick_test(&last_oper, us_total_period / 2 / period)) {
      t_on += diff;
      nrfx_systick_get(&last_oper);
    }
    if (t_on == 100)
      diff = -1;
    if (t_on > 100)
      t_on = 100;
    if (t_on == 0 && diff == -1) {
      LED_OFF(pin_color);
      return;
    }
  }
}