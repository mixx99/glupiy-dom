#include "pwm_control.h"
#include "gpiote_control.h"
#include "hsv.h"
#include "led_control.h"

// esl-nsdk
#include "app_timer.h"
#include "nrf_pwm.h"
#include "nrfx_pwm.h"

typedef enum {
  NO_INPUT = 0,
  HUE_MODIFICATION,
  SATURATION_MODIFICATION,
  BRIGHTNESS_MODIFICATION,
} mode_t;

APP_TIMER_DEF(mode_timer);

static hsv_t hsv;
static rgb_t rgb;
static int current_mode;

static nrfx_pwm_config_t config = NRFX_PWM_DEFAULT_CONFIG;
static const nrfx_pwm_t pwm = NRFX_PWM_INSTANCE(0);
static uint16_t values[NUMBER_OF_VALUES];
static nrf_pwm_sequence_t sequence = {.values.p_common = values,
                                      .length = NRF_PWM_VALUES_LENGTH(values),
                                      .repeats = 0,
                                      .end_delay = 0};

static const int YELLOW_ID = 0;
static const int RED_ID = 1;
static const int GREEN_ID = 2;
static const int BLUE_ID = 3;
static int yellow_state = 0;
static volatile uint32_t pwm_tick = 0;

static const int YELLOW_LONG_DELAY = 30;
static const int YELLOW_SHORT_DELAY = 10;

static inline void init_hsv_and_rgb();
static inline void handle_double_click();
static inline void handle_pressed();
static inline void handle_hue_mode();
static inline void handle_saturation_mode();
static inline void handle_brightness_mode();
static void mode_timer_handler(void *p_context);
static inline void update_mode_indicator();

static inline void init_hsv_and_rgb() {
  hsv.s = 100;
  hsv.v = 100;
  hsv.h = (DEVICE_LABEL % 100);
  rgb = hsv_to_rgb(hsv);
  values[RED_ID] = rgb.r;
  values[GREEN_ID] = rgb.g;
  values[BLUE_ID] = rgb.b;
}

static void mode_timer_handler(void *p_context) {
  static int i = 1;
  switch (current_mode) {

  case NO_INPUT:
    values[YELLOW_ID] = 0;
    break;

  case HUE_MODIFICATION:
    if (values[YELLOW_ID] <= 0)
      i = 1;
    if (values[YELLOW_ID] >= TOP_VALUE)
      i = -1;
    values[YELLOW_ID] += i * 10;
    // yellow_state = !yellow_state;
    // values[YELLOW_ID] = yellow_state ? TOP_VALUE : 0;
    break;

  case SATURATION_MODIFICATION:
    if (values[YELLOW_ID] <= 0)
      i = 1;
    if (values[YELLOW_ID] >= TOP_VALUE)
      i = -1;
    values[YELLOW_ID] += i * 10;
    // yellow_state = !yellow_state;
    // values[YELLOW_ID] = yellow_state ? TOP_VALUE : 0;
    break;

  case BRIGHTNESS_MODIFICATION:
    values[YELLOW_ID] = 100;
    break;
  }
}

uint16_t *get_values() { return values; }

nrf_pwm_sequence_t *get_sequence() { return &sequence; }

const nrfx_pwm_t *get_instance() { return &pwm; }

void init_pwm() {
  config.output_pins[YELLOW_ID] = LED_YELLOW;
  config.output_pins[RED_ID] = LED_RED;
  config.output_pins[GREEN_ID] = LED_GREEN;
  config.output_pins[BLUE_ID] = LED_BLUE;
  config.top_value = TOP_VALUE;
  config.load_mode = NRF_PWM_LOAD_INDIVIDUAL;
  // config.base_clock = NRF_PWM_CLK_125kHz; // TODO: Check later.

  values[0] = 0;
  init_hsv_and_rgb();

  app_timer_create(&mode_timer, APP_TIMER_MODE_REPEATED, mode_timer_handler);
  app_timer_start(mode_timer, APP_TIMER_TICKS(50), NULL);

  nrfx_pwm_init(&pwm, &config, NULL);
  nrfx_pwm_simple_playback(get_instance(), get_sequence(), 1,
                           NRFX_PWM_FLAG_LOOP);
}

static inline void handle_double_click() {
  current_mode = (current_mode + 1) % 4;
  update_mode_indicator();
}

void update_mode_indicator() {
  app_timer_stop(mode_timer);

  switch (current_mode) {

  case NO_INPUT:
    values[YELLOW_ID] = 0;
    return;

  case HUE_MODIFICATION:
    app_timer_start(mode_timer, APP_TIMER_TICKS(YELLOW_LONG_DELAY), NULL);
    break;

  case SATURATION_MODIFICATION:
    app_timer_start(mode_timer, APP_TIMER_TICKS(YELLOW_SHORT_DELAY), NULL);
    break;

  case BRIGHTNESS_MODIFICATION:
    values[YELLOW_ID] = TOP_VALUE;
    return;
  }
}

static inline void handle_pressed() {
  switch (current_mode) {
  case NO_INPUT:
    break;
  case HUE_MODIFICATION:
    handle_hue_mode();
    break;
  case SATURATION_MODIFICATION:
    handle_saturation_mode();
    break;
  case BRIGHTNESS_MODIFICATION:
    handle_brightness_mode();
  }
}

static inline void handle_hue_mode() {
  if (hsv.h >= 360)
    hsv.h = 0;
  hsv.h += 1;
  rgb = hsv_to_rgb(hsv);
  values[RED_ID] = rgb.r;
  values[GREEN_ID] = rgb.g;
  values[BLUE_ID] = rgb.b;
}

static inline void handle_saturation_mode() {
  static int i = 1;
  if (hsv.s >= 100)
    i = -1;
  if (hsv.s <= 0)
    i = 1;
  hsv.s += i;
  rgb = hsv_to_rgb(hsv);
  values[RED_ID] = rgb.r;
  values[GREEN_ID] = rgb.g;
  values[BLUE_ID] = rgb.b;
}

static inline void handle_brightness_mode() {
  static int i = 1;
  if (hsv.v >= 100)
    i = -1;
  if (hsv.v <= 0)
    i = 1;
  hsv.v += i;
  rgb = hsv_to_rgb(hsv);
  values[RED_ID] = rgb.r;
  values[GREEN_ID] = rgb.g;
  values[BLUE_ID] = rgb.b;
}

void handle_pwm(BUTTON_STATUS status) {
  switch (status) {
  case BUTTON_NONE:
    break;
  case BUTTON_PRESSED:
    handle_pressed();
    break;
  case BUTTON_DOUBLE_CLICK:
    handle_double_click();
    break;
  default:
    break;
  }
}
