#include <pigpio.h>
#include <QDebug>
#include "GPIO.h"

void gpio_init()
{
    if (gpioInitialise() != PI_INIT_FAILED) {
        qFatal("gpio init failed");
    }
}

void gpio_cleanup()
{
    gpioTerminate();
}
