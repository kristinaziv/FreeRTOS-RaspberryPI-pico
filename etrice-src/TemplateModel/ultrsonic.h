#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"
#pragma once
#define trigPin 2
#define echoPin 3

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

void setupUltrasonicPins();
float getCm();
uint64_t getPulse();
#endif
