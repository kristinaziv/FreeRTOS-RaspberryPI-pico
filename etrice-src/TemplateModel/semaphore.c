#include "semaphore.h"

SemaphoreHandle_t adcMutex;
int initialized = 0;
void initMutex(){
    if(initialized==0){
        adcMutex=xSemaphoreCreateMutex();
        initialized=1;
    }
}