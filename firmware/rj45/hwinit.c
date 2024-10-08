#include "hwinit.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
// #include "hardware/adc.h"
#include "pico/stdlib.h"
#include "system.h"

static struct repeating_timer timer;

// Timer interrupt (L-tika)
static bool _led_blink_timer_callback(struct repeating_timer *t)
{
    static bool led0_state = true;

    gpio_put(PICO_DEFAULT_LED_PIN, led0_state);
    led0_state = !led0_state;

    return true;
}

void hw_init()
{
    // // Core
    // vreg_set_voltage(VREG_VOLTAGE_1_20);    // Increase core voltage to stabilize overclocking
    // sleep_ms(10);
    // set_sys_clock_khz(240000, true);        // Over clock 240MHz

    // // ADC Settings
    // adc_init();
    // adc_gpio_init(DEF_SYS_HWPIN_ADC0);
    // adc_gpio_init(DEF_SYS_HWPIN_ADC1);

    // // LED
    // gpio_init(PICO_DEFAULT_LED_PIN);
    // gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // // DCDC PS Control
    // gpio_init(DEF_SYS_HWPIN_DCDC_PS);
    // gpio_set_dir(DEF_SYS_HWPIN_DCDC_PS, GPIO_OUT);
    // gpio_put(DEF_SYS_HWPIN_DCDC_PS, true);  // true:PWM mode(low ripple), false:PFM mode(default)
}

void hw_start_led_blink()
{
    add_repeating_timer_ms(-500, _led_blink_timer_callback, NULL, &timer);
}
