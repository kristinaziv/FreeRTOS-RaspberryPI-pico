#include "FreeRTOS.h"
#include "semphr.h"
#ifndef SEMAPHORE_H1
#define SEMAPHORE_H1

extern SemaphoreHandle_t adcMutex;

void initMutex();

#endif