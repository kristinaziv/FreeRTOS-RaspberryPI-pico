#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <queue.h>


void task1(void *params)
{   

    while (true) {
     printf("Task 1 is currently running\n");
     vTaskDelay(100); 
    }
}
void task2(void *params)
{   

    while (true) {
     printf("Task 2 is currently running\n");
     vTaskDelay(200);
    }
}

int main()
{
    stdio_init_all();

    xTaskCreate(task1, "Task_1", 256, NULL, 1, NULL);
    xTaskCreate(task2, "Task_2", 256, NULL, 2, NULL);
    vTaskStartScheduler();

    while(1){};
}