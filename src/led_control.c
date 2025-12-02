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
