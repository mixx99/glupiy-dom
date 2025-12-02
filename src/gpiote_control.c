#include "gpiote_control.h"
#include "led_control.h"
#include "pwm_control.h"

// std
#include <stdlib.h>

// esl-nsdk
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "nrf_error.h"
#include "nrfx_gpiote.h"

#define TIMEOUT_TIME 50               // debounce
#define TIMEOUT_DOUBLE_CLICK_TIME 200 // max delay between clicks

static volatile bool waiting_second_click = 0;
static volatile bool ignore_release_once = 0;
static bool double_click_active = 0;

APP_TIMER_DEF(debounce_timer);
APP_TIMER_DEF(double_click_timer);
APP_TIMER_DEF(hold_timer);

void hold_timeout_handler(void *p_context) {
  if (!nrfx_gpiote_in_is_set(BUTTON)) {
    handle_pwm(BUTTON_PRESSED);
  } else {
    app_timer_stop(hold_timer);
  }
}

void button_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
  app_timer_start(debounce_timer, APP_TIMER_TICKS(TIMEOUT_TIME), NULL);
}

void debounce_timeout_handler(void *p_context) {
  bool pressed = !nrfx_gpiote_in_is_set(BUTTON);

  if (pressed) {
    if (!waiting_second_click) {
      waiting_second_click = true;
      if (pressed) {
        handle_pwm(BUTTON_PRESSED);
        app_timer_start(hold_timer, APP_TIMER_TICKS(20), NULL);
      } else {
        app_timer_stop(hold_timer);
        handle_pwm(BUTTON_RELEASED);
      }

      app_timer_start(double_click_timer,
                      APP_TIMER_TICKS(TIMEOUT_DOUBLE_CLICK_TIME), NULL);
    } else {
      waiting_second_click = false;

      if (!double_click_active) {
        handle_pwm(BUTTON_DOUBLE_CLICK);
        double_click_active = 1;
      } else {
        handle_pwm(BUTTON_NONE);
        double_click_active = 0;
      }

      ignore_release_once = true;

      app_timer_stop(double_click_timer);
    }
  } else {
    if (ignore_release_once) {
      ignore_release_once = false;
      return;
    }

    handle_pwm(BUTTON_RELEASED);
  }
}

void double_click_timeout_handler(void *p_context) {
  waiting_second_click = false;

  handle_pwm(BUTTON_RELEASED);
}

void init_gpiote() {
  nrfx_err_t err = nrfx_gpiote_init();
  if (err != NRFX_SUCCESS)
    exit(1);

  nrfx_clock_init(NULL);
  nrfx_clock_lfclk_start();
  while (!nrfx_clock_lfclk_is_running())
    ;

  nrfx_gpiote_in_config_t config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
  config.pull = NRF_GPIO_PIN_PULLUP;

  err = nrfx_gpiote_in_init(BUTTON, &config, button_handler);
  if (err != NRFX_SUCCESS)
    exit(1);

  nrfx_gpiote_in_event_enable(BUTTON, true);

  ret_code_t timer_err = app_timer_init();
  if (timer_err != NRF_SUCCESS)
    exit(1);

  app_timer_create(&debounce_timer, APP_TIMER_MODE_SINGLE_SHOT,
                   debounce_timeout_handler);
  app_timer_create(&double_click_timer, APP_TIMER_MODE_SINGLE_SHOT,
                   double_click_timeout_handler);
  app_timer_create(&hold_timer, APP_TIMER_MODE_REPEATED, hold_timeout_handler);
}
