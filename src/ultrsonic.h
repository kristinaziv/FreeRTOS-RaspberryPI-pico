#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"
#define trigPin 2
#define echoPin 3

void setupUltrasonicPins();
float getCm();
int timeout = 26100;

void setupUltrasonicPins()
{
    gpio_init(trigPin);
    gpio_init(echoPin);
    gpio_set_dir(trigPin, GPIO_OUT);
    gpio_set_dir(echoPin, GPIO_IN);
    gpio_put(trigPin, 0); 
}

uint64_t getPulse()
{
    
    gpio_put(trigPin, 1);
    sleep_us(10);  // 10 microseconds pulse
    gpio_put(trigPin, 0);

    uint64_t width = 0;

    while (gpio_get(echoPin) == 0) tight_loop_contents(); 
    absolute_time_t startTime = get_absolute_time();
    while (gpio_get(echoPin) == 1) {  
        width++;
        sleep_us(1);
        if (width > timeout) return 0;
    }
    absolute_time_t endTime = get_absolute_time();

    return absolute_time_diff_us(startTime, endTime);
}

float getCm() {
    uint64_t pulseLength = getPulse();
    if (pulseLength == 0) {
        return 0.0f;  
    }
 
    return (float)pulseLength / 58.0f;
}

