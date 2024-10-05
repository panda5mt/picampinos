#include "hwinit.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

static struct repeating_timer timer;

// Timer interrupt (L-tika)
static bool _led_blink_timer_callback(struct repeating_timer *t)
{
    static bool led0_state = true;

    gpio_put(PICO_DEFAULT_LED_PIN, led0_state);
    led0_state = !led0_state;

    return true;
}

void hw_init() {}
// {
//     vreg_set_voltage(VREG_VOLTAGE_1_20);    // Increase core voltage to stabilize overclocking
//     sleep_ms(10);
//     set_sys_clock_khz(240000, true);        // Over clock 240MHz

//     gpio_init(PICO_DEFAULT_LED_PIN);
//     gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
// }

void hw_start_led_blink()
{
    add_repeating_timer_ms(-500, _led_blink_timer_callback, NULL, &timer);
}
