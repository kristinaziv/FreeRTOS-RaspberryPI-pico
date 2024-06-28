#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <queue.h>

static QueueHandle_t xQueue=NULL;

void led_task()
{   
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    uint uIValuetoSend=0;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        uIValuetoSend=1;
        xQueueSend(xQueue,&uIValuetoSend,0U);
        vTaskDelay(100);

        gpio_put(LED_PIN, 0);
        uIValuetoSend=0;   
        xQueueSend(xQueue,&uIValuetoSend,0U);
        vTaskDelay(100);
    }
}
void printStateToSerialMonitor(void *pvParams)
{
    uint uiReceived;

    while(true){
        xQueueReceive(xQueue,&uiReceived,portMAX_DELAY);
        if(uiReceived == 1){
            printf("LED IS ON!\n");
        }else{
            printf("LED IS OFF!\n");
        }
    }
    


}

int main()
{
    stdio_init_all();

    xQueue=xQueueCreate(1,sizeof(uint));

    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(printStateToSerialMonitor, "Usb_Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
}