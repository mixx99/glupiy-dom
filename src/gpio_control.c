#include "gpio_control.h"

// esl-nsdk
#include "nrf_delay.h"

// Init all the GPIO pins.
void init_gpio() {
  nrf_gpio_cfg_output(LED_RED);
  nrf_gpio_cfg_output(LED_GREEN);
  nrf_gpio_cfg_output(LED_BLUE);
  nrf_gpio_cfg_output(LED_YELLOW);

  nrf_gpio_cfg_input(BUTTON, NRF_GPIO_PIN_PULLUP);

  turn_off_every_led();
}

// blink with `pin_color` color with BLINK_DELAY delay.
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
int is_button_pressed() { return nrf_gpio_pin_read(BUTTON) == 0; }

// This is optional function for optional task:
// `Think about how you can implement a mechanism so that if the button is
// released when the LED is on, it stays on.`
void blink_and_stay_on_if_button_not_pressed(uint32_t pin_color) {
  turn_off_every_led();
  LED_ON(pin_color);
  nrf_delay_ms(BLINK_DELAY);
  if (is_button_pressed())
    LED_OFF(pin_color);
}