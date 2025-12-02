#ifndef hsv_t_H_
#define hsv_t_H_

#include <stdint.h>

typedef struct {
  uint16_t r;
  uint16_t g;
  uint16_t b;
} rgb_t;

typedef struct {
  double h;
  int s;
  int v;
} hsv_t;

rgb_t hsv_to_rgb(hsv_t hsv);

#endif
