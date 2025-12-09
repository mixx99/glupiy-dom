#include "hsv.h"
#include "pwm_control.h"

#include <math.h>
#include <stdint.h>

static inline int clamp(int value, int min_value, int max_value);

rgb_t hsv_to_rgb(hsv_t hsv) {
  rgb_t result;

  double new_h = hsv.h;
  double new_s = hsv.s / 100.0f;
  double new_v = hsv.v / 100.0f;

  if (new_s <= 0.0f) {
    uint16_t val = (uint16_t)(new_v * +0.5f);
    result.r = val;
    result.g = val;
    result.b = val;
    return result;
  }

  if (new_h >= 360.0f)
    new_h = 0.0f;
  double hf = new_h / 60.0f;
  int i = (int)floorf(hf);
  double f = hf - i;
  double p = new_v * (1.0f - new_s);
  double q = new_v * (1.0f - new_s * f);
  double t = new_v * (1.0f - new_s * (1.0f - f));

  double rf = 0, gf = 0, bf = 0;
  switch (i) {
  case 0:
    rf = new_v;
    gf = t;
    bf = p;
    break;
  case 1:
    rf = q;
    gf = new_v;
    bf = p;
    break;
  case 2:
    rf = p;
    gf = new_v;
    bf = t;
    break;
  case 3:
    rf = p;
    gf = q;
    bf = new_v;
    break;
  case 4:
    rf = t;
    gf = p;
    bf = new_v;
    break;
  case 5:
  default:
    rf = new_v;
    gf = p;
    bf = q;
    break;
  }

  result.r = (uint16_t)(clamp((int)roundf(rf * TOP_VALUE), 0, TOP_VALUE));
  result.g = (uint16_t)(clamp((int)roundf(gf * TOP_VALUE), 0, TOP_VALUE));
  result.b = (uint16_t)(clamp((int)roundf(bf * TOP_VALUE), 0, TOP_VALUE));
  return result;
}

static inline int clamp(int value, int min_value, int max_value) {
  if (value < min_value)
    return min_value;
  if (value > max_value)
    return max_value;
  return value;
}
