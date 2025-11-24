#ifndef GPIO_H_
#define GPIO_H_

// esl-nsdk
#include "nrf_gpio.h"

#define BLINK_DELAY 250
#define LONG_DELAY 750

// https://devzone.nordicsemi.com/cfs-file/__key/support-attachments/beef5d1b77644c448dabff31668f3a47-ab48524591d842238bd2e0a2929469c4/pca10059_5F00_schematic_5F00_and_5F00_pcb.pdf
#define LED_RED NRF_GPIO_PIN_MAP(0, 8)
#define LED_GREEN NRF_GPIO_PIN_MAP(1, 9)
#define LED_BLUE NRF_GPIO_PIN_MAP(0, 12)
#define LED_YELLOW NRF_GPIO_PIN_MAP(0, 6)
#define BUTTON NRF_GPIO_PIN_MAP(1, 6)

#define LED_ON(pin) nrf_gpio_pin_write(pin, 0)
#define LED_OFF(pin) nrf_gpio_pin_write(pin, 1)

#define ONE_SEC_US 1000000
#define ONE_SEC_MS 1000

void init_gpio();
void blink(uint32_t pin_color);
void turn_off_every_led();
// int is_button_pressed(); // TODO: Delete?
void smooth_blink(uint32_t pin_color, uint32_t us_total_period);

#endif