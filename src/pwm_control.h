#ifndef PWM_CONTROL_H_
#define PWM_CONTROL_H_

#include "gpiote_control.h"

// std
#include <stdint.h>

// esl-nsdk
#include "nrfx_pwm.h"

#define NUMBER_OF_VALUES 4
#define TOP_VALUE 255

void init_pwm();
void handle_pwm(BUTTON_STATUS status);
uint16_t *get_values();
nrf_pwm_sequence_t *get_sequence();
const nrfx_pwm_t *get_instance();

#endif
