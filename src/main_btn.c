#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <queue.h>
#include "semphr.h"

static SemaphoreHandle_t countingSem;

void led_task()
{   
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        if(xSemaphoreTake(countingSem,(TickType_t) 10)==pdTRUE){
            printf("led task \n ");
            gpio_put(LED_PIN,1);
            vTaskDelay(100);
        }else{
        printf("led task taken\n ");
            gpio_put(LED_PIN,0);
            vTaskDelay(1);
        }
    }
}
void btnTask()
{
    gpio_init(20);
    gpio_set_dir(20,GPIO_IN);

    while(true){
       if(gpio_get(20)!=0){
        printf("task btn not\n ");
        xSemaphoreGive(countingSem);
        vTaskDelay(20);
       }else{
        printf("task btn yes\n ");
            vTaskDelay(1);
       }
    }
    


}

int main()
{
    stdio_init_all();

    countingSem = xSemaphoreCreateCounting(5,0);

    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(btnTask, "Btn_Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
}