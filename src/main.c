#include "gpiote_control.h"
#include "led_control.h"
#include "pwm_control.h"

int main(void) {
  init_gpio();
  init_gpiote();
  init_pwm();

  while (1) {
    __WFE();
  }
}
