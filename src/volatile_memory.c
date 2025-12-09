#include "volatile_memory.h"
#include "led_control.h"

// esl-nsdk 
#include "nrfx_nvmc.h"
#include "app_error.h"

#define HSV_FLASH_ADDR (0x7F000)

static uint32_t pack_hsv(hsv_t hsv) {
  uint16_t h = (uint16_t)hsv.h;
  uint8_t s = (uint8_t)hsv.s;
  uint8_t v = (uint8_t)hsv.v;

  return ((uint32_t)h << 16) | ((uint32_t)s << 8) | (uint32_t)v;
}

static hsv_t unpack_hsv(uint32_t data) {
  hsv_t hsv;

  hsv.h = (double)((data >> 16) & 0xFFFF);
  hsv.s = (int)((data >> 8) & 0xFF);
  hsv.v = (int)(data & 0xFF);

  return hsv;
}

void set_start_hsv(hsv_t hsv) {
  uint32_t packed = pack_hsv(hsv);
  uint32_t *flash_ptr = (uint32_t *)HSV_FLASH_ADDR;

  if (*flash_ptr == packed)
    return;

  nrfx_nvmc_page_erase(HSV_FLASH_ADDR);

  nrfx_nvmc_word_write(HSV_FLASH_ADDR, packed);

  while (!nrfx_nvmc_write_done_check())
    ;
}

hsv_t get_start_hsv() {
  hsv_t hsv;
  uint32_t raw = *(uint32_t *)HSV_FLASH_ADDR;

  if (raw == 0xFFFFFFFF) {
    hsv.s = 100;
    hsv.v = 100;
    hsv.h = (DEVICE_LABEL % 100);
  } else {
    hsv = unpack_hsv(raw);

    if (hsv.h < 0)
      hsv.h = 0;
    if (hsv.h > 360)
      hsv.h = 360;

    if (hsv.s < 0)
      hsv.s = 0;
    if (hsv.s > 100)
      hsv.s = 100;

    if (hsv.v < 0)
      hsv.v = 0;
    if (hsv.v > 100)
      hsv.v = 100;
  }
  return hsv;
}
