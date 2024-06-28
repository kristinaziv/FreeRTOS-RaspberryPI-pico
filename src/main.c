#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <queue.h>
#include "hardware/adc.h"
#include "hardware/watchdog.h"
#include "semphr.h"

#include <ctype.h>  
#include <string.h>

#define TEMPERATURE_UNITS 'C'
#define WATER_SENSOR_PIN 13

static SemaphoreHandle_t adcMutex=NULL;
static QueueHandle_t xQueue=NULL;
static QueueHandle_t xWaterQueue=NULL;
static QueueHandle_t xMissile=NULL;

static SemaphoreHandle_t blinkingMutex=NULL;

static QueueHandle_t xBlinkQueue=NULL;
static bool ledBlinking = false;
static bool criticalReportedWl = false;



void watchdog_task(void *pvParams)
{
    const TickType_t xFrequency = 1000; 
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (true)
    {
        watchdog_update();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(xFrequency)); //1s
    }
}

void blink_task(void *params) {
    
    const uint DIODE_PIN = 15;
    gpio_init(DIODE_PIN);
    gpio_set_dir(DIODE_PIN, GPIO_OUT);
    bool blink = false;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(500);

    while (true) {
        if (xQueueReceive(xBlinkQueue, &blink, portMAX_DELAY) == pdTRUE) {
            while (ledBlinking) {
                gpio_put(DIODE_PIN, 1);
                vTaskDelayUntil(&xLastWakeTime, xFrequency); //500ms
                gpio_put(DIODE_PIN, 0);
                vTaskDelayUntil(&xLastWakeTime, xFrequency);//500ms
            }
        }
    }
}


void temp_task()
{   
    const float conversionFactor = 3.3f / (1 << 12);
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(150);

    float adc,tempC;
    xLastWakeTime = xTaskGetTickCount(); 

    while (true) {
        if(xSemaphoreTake(adcMutex,(TickType_t)10)==pdTRUE){
            adc_select_input(4); 
            adc = (float)adc_read() * conversionFactor;
            xSemaphoreGive(adcMutex);
        }
        tempC = 27.0f - (adc - 0.706f) / 0.001721f;
        xQueueSend(xQueue,&tempC,0U);
        vTaskDelayUntil( &xLastWakeTime, xFrequency); //150ms

    }
}
void water_level_task()
{
    const float conversionFactor = 3.3f / (1 << 12);
    float waterLevel;
    const TickType_t xFrequency = pdMS_TO_TICKS(200) ; 
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true) {        

        if(xSemaphoreTake(adcMutex,(TickType_t)10)==pdTRUE){
            adc_select_input(0);
            waterLevel = (float)adc_read() * conversionFactor;
            xSemaphoreGive(adcMutex);
        }
        xQueueSend(xWaterQueue, &waterLevel, 0U);
        vTaskDelayUntil(&xLastWakeTime, xFrequency); //200ms
    }
}

void print_to_serial_monitor_temp()
{

    float tempReceived;
    int criticalCnt = 0;
    const int threshold = 5;
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    bool criticalReported = false;
    const float tempTreshold=25.0;
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while(true){
        xQueueReceive(xQueue, &tempReceived, portMAX_DELAY);

        if(tempReceived > tempTreshold) {
            criticalCnt++;
            if (criticalCnt >= threshold && !criticalReported) {
                gpio_put(LED_PIN, 1);
                printf("CRITICAL: Temperatura je veca od %f: %f\n",tempTreshold, tempReceived);
                criticalReported = true;
            }
        } else {
            if (criticalReported) {
                gpio_put(LED_PIN, 0);
                printf("INFO: Temperatura se stabilizovala %f\n", tempReceived);
                criticalReported = false;
            }
            criticalCnt = 0; 
        }
    
}
}


void print_to_serial_monitor_wl()
{
    float waterLevelReceived;
    int criticalCount = 0;
    const int threshold = 5;
   criticalReportedWl=false;
    const float wltreshold=0.5;

    bool blink = false;

   while (true) {
        xQueueReceive(xWaterQueue, &waterLevelReceived, portMAX_DELAY);

        if (waterLevelReceived < wltreshold) {
            criticalCount++;
            if (criticalCount >= threshold && !criticalReportedWl) {
                blink = true;
                xQueueSend(xBlinkQueue, &blink, portMAX_DELAY);
                ledBlinking = true;
                printf("CRITICAL: Nivo vode je ispod praga %f: %f\n", wltreshold, waterLevelReceived);
                criticalReportedWl = true;
            }
        } else {
            if (criticalReportedWl) {
                blink = false;
                xQueueSend(xBlinkQueue, &blink, portMAX_DELAY);
                printf("Portmax delay %d",pdMS_TO_TICKS(portMAX_DELAY));
                ledBlinking = false;
                printf("INFO: Nivo vode se stabilizovao! %f\n", waterLevelReceived);
                criticalReportedWl = false;
            }
            criticalCount = 0; 
        }
    }
}

/*

void lcd_print_task(void *pvParams) {

    lcd_init();
    lcd_clear();
    //vTaskDelay(pdMS_TO_TICKS(600));

   LCDTaskParams *params = (LCDTaskParams *)pvParams;

   vTaskDelay(pdMS_TO_TICKS(10));
    printf("parametri su %s",params->text);
    while(1){
    lcd_set_cursor(params->col, params->row);
    
    printf("parametri su %s",params->text);
    lcd_print(params->text);
    
    printf("parametri su %s",params->text);
    vTaskDelay(2000);
    lcd_clear();
    }

   // vTaskDelete(NULL);
}*/


void usb_serial_task(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(800));//800ms

      //  printf("Tick time current %d",xLastWakeTime);
       // printf("Tick time xFrequency %d",pdMS_TO_TICKS(1000)); //1 ms = 100 Ticks
        int character = getchar_timeout_us(0);

        if (character != PICO_ERROR_TIMEOUT) {
            if ((char)character == 's') {
                if (xQueueSend(xMissile, &character, portMAX_DELAY) != pdPASS) {
                    printf("Greška pri slanju u red xMissile!\n");
                }
            }
        }
    }
}

void fire_missile() {
    int missileDirections;
    TickType_t xLastWakeTime = xTaskGetTickCount();


    while (true) {
        if (xQueueReceive(xMissile, &missileDirections, portMAX_DELAY) == pdPASS) {
            if(!criticalReportedWl){
                for (int i = 3; i > 0; i--) {
                    printf("ISPALJUJE SE PROJEKTIL!!! ... %d\n", i);
                    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(300)); //300ms
                }
            }else{
                  printf("Zbog nedovoljnog goriva nece biti ispaljen projektil.");
                  
            }
        }
    }
}
int main()
{
    stdio_init_all();
   
   /* const uint32_t watchdog_interval_ms = 5000;

    if (watchdog_caused_reboot()) {
        printf("WatchDog: Resetovanje...\n");
        return 0;
    } else {
        printf("INFO:Pokretanje..\n");
    }

    watchdog_enable(watchdog_interval_ms, 1);
*/
    xQueue=xQueueCreate(1,sizeof(float));
    xWaterQueue=xQueueCreate(1,sizeof(float));
    adcMutex=xSemaphoreCreateMutex();
    blinkingMutex=xSemaphoreCreateMutex();
    xBlinkQueue=xQueueCreate(1,sizeof(int));
    xMissile=xQueueCreate(1,sizeof(int));
    adc_init();
    adc_set_temp_sensor_enabled(true);


 /*   LCDTaskParams lcdParams = {
        .text = "Hello",
        .row = 0,
        .col = 0
    };
*/
   // xTaskCreate(lcd_print_task, "LCD_Print_Task", 256, &lcdParams, 1, NULL);


    xTaskCreate(usb_serial_task, "USBSerialTask", 256, NULL, 4, NULL);
    xTaskCreate(fire_missile, "FireMissileTask", 256, NULL, 5, NULL);




    xTaskCreate(temp_task, "TEMP_Task", 256, NULL, 2, NULL);
    xTaskCreate(print_to_serial_monitor_temp, "TEMP_USB_Task", 256, NULL, 2, NULL);

  //  xTaskCreate(watchdog_task, "WDT_Task", 256, NULL, 1, NULL);

  
    xTaskCreate(water_level_task, "WL_Task", 256, NULL, 3, NULL);
    xTaskCreate(print_to_serial_monitor_wl, "WL_USB_Task", 256, NULL, 3, NULL);

    xTaskCreate(blink_task, "BLINK_Task", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1){};
}