#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <queue.h>
#include "hardware/adc.h"
#include "hardware/watchdog.h"
#include "semphr.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "lcd.h"

#include <ctype.h>
#include "ultrsonic.h"
#include <string.h>


static SemaphoreHandle_t adcMutex = NULL;
static QueueHandle_t xQueue = NULL;
static QueueHandle_t xWaterQueue = NULL;
static QueueHandle_t xMissileDist = NULL;

static SemaphoreHandle_t blinkingMutex = NULL;
static QueueHandle_t xFire = NULL;
static QueueHandle_t xBlinkQueue = NULL;
static SemaphoreHandle_t xFireSem = NULL;
static SemaphoreHandle_t xLcdSem = NULL;

static SemaphoreHandle_t xLaunchCompleteSem;
static SemaphoreHandle_t xCheckTargetSem;
static bool ledBlinking = false;
static bool criticalReportedWl = false;

static bool criticalReportedTmp = false;

void watchdog_task(void *pvParams)
{
    const TickType_t xFrequency = 1000;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (true)
    {
        watchdog_update();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(xFrequency)); // 1s
    }
}



void lcd_print_status()
{
    if (criticalReportedWl)
        lcd_print(0, "CR: waterlvl low");
    else
        lcd_print(0, "Inf: waterlvl ok");

    if (criticalReportedTmp)
        lcd_print(1, "CR: temp lvow");
    else
        lcd_print(1, "Inf: temp ok");
}

void blink_task(void *params)
{

    const uint DIODE_PIN = 15;
    gpio_init(DIODE_PIN);
    gpio_set_dir(DIODE_PIN, GPIO_OUT);
    bool blink = false;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(500);

    while (true)
    {
        if (xQueueReceive(xBlinkQueue, &blink, portMAX_DELAY) == pdTRUE)
        {
            while (ledBlinking)
            {
                gpio_put(DIODE_PIN, 1);
                vTaskDelayUntil(&xLastWakeTime, xFrequency); // 500ms
                gpio_put(DIODE_PIN, 0);
                vTaskDelayUntil(&xLastWakeTime, xFrequency); // 500ms
            }
        }
    }
}

void temp_task()
{
    const float conversionFactor = 3.3f / (1 << 12);
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(150);

    float adc, tempC;
    xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
        if (xSemaphoreTake(adcMutex, (TickType_t)10) == pdTRUE)
        {   
          //  printf("trenutno TMP %d\n",xTaskGetTickCount());
            adc_select_input(4);
            adc = (float)adc_read() * conversionFactor;
            xSemaphoreGive(adcMutex);
            
        }
        tempC = 27.0f - (adc - 0.706f) / 0.001721f;
        
          //printf("trenutno TMP posle %d\n",xTaskGetTickCount());
        xQueueOverwrite(xQueue, &tempC);
        vTaskDelayUntil(&xLastWakeTime, xFrequency); // 150ms
    }
}

void water_level_task()
{
    const float conversionFactor = 3.3f / (1 << 12);
    float waterLevel;
    const TickType_t xFrequency = pdMS_TO_TICKS(150);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true)
    {

        if (xSemaphoreTake(adcMutex, (TickType_t)10) == pdTRUE)
        {   //printf("trenutno WL %d\n",xTaskGetTickCount());
            adc_select_input(0);
            waterLevel = (float)adc_read() * conversionFactor;
            //printf("trenutno posle  WL %d\n",xTaskGetTickCount());
            xSemaphoreGive(adcMutex);
        }
        xQueueOverwrite(xWaterQueue, &waterLevel);
        vTaskDelayUntil(&xLastWakeTime, xFrequency); // 200ms
    }
}

void print_to_serial_monitor_temp()
{

    float tempReceived;
    int criticalCnt = 0;
    const int threshold = 5;
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    const float tempTreshold = 25.0;
    bool okreported = false;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true)
    {
        if(xQueueReceive(xQueue, &tempReceived, portMAX_DELAY)==pdPASS){

          //printf("trenutno printTMP  %d\n",xTaskGetTickCount());
        if (tempReceived > tempTreshold)
        {
            criticalCnt++;
            if (criticalCnt >= threshold && !criticalReportedTmp)
            {
                gpio_put(LED_PIN, 1);

                lcd_print(1, "CR: temp low");
                printf("CRITICAL: Temperatura je veca od %f: %f\n", tempTreshold, tempReceived);
                criticalReportedTmp = true;
            }
        }
        else
        {
            if (criticalReportedTmp)
            {
                gpio_put(LED_PIN, 0);
                lcd_print(1, "Inf: temp ok");
                printf("INFO: Temperatura se stabilizovala %f\n", tempReceived);
                criticalReportedTmp = false;
            }
            else
            {
                if (!okreported)
                {
                    lcd_print(1, "Inf: temp ok");
                    printf("INFO: Nivo temperature je stabilan %f\n", tempReceived);
                    okreported = true;
                }
            }
            criticalCnt = 0;
        }
        
          //printf("trenutno printTMP posle %d\n",xTaskGetTickCount());
        }else{
            printf("greska pri ispisivanju temperature!\n");
        }
    }
}

void print_to_serial_monitor_wl()
{
    float waterLevelReceived;
    int criticalCount = 0;
    const int threshold = 5;
    criticalReportedWl = false;
    const float wltreshold = 0.5;
    bool okreported = false;
    bool blink = false;

    while (true)
    {
        xQueueReceive(xWaterQueue, &waterLevelReceived, portMAX_DELAY);

          //printf("trenutno printWL %d\n",xTaskGetTickCount());
        if (waterLevelReceived < wltreshold)
        {
            criticalCount++;
            if (criticalCount >= threshold && !criticalReportedWl)
            {
                blink = true;
                xQueueSend(xBlinkQueue, &blink, portMAX_DELAY);
                ledBlinking = true;
                lcd_print(0, "CR: fuelvl low");
                printf("CRITICAL: Nivo goriva je ispod praga %f: %f\n", wltreshold, waterLevelReceived);
                criticalReportedWl = true;
            }
        }
        else
        {
            if (criticalReportedWl)
            {
                blink = false;
                xQueueSend(xBlinkQueue, &blink, portMAX_DELAY);
                printf("Portmax delay %d", pdMS_TO_TICKS(portMAX_DELAY));
                ledBlinking = false;

                lcd_print(0, "Inf: fuelvl ok");
                printf("INFO: Nivo goriva se stabilizovao! %f\n", waterLevelReceived);
                criticalReportedWl = false;
            }
            else
            {
                if (!okreported)
                {
                    lcd_print(0, "Inf: fuelvl ok");
                    printf("INFO: Nivo goriva je stabilan! %f\n", waterLevelReceived);
                    okreported = true;
                }
            }
            criticalCount = 0;
        }
        
         // printf("trenutno printWL posle %d\n",xTaskGetTickCount());
    }
}

void uwb_task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    setupUltrasonicPins();

    while (1)
    {
        
          //printf("trenutno uwb posle %d\n",xTaskGetTickCount());
        float cm = getCm();
        //printf("Razdaljina %f cm\n", getCm());
        if (xQueueOverwrite(xMissileDist, &cm) != pdPASS)
        {
            printf("Greška pri slanju u reed xMissileDist!\n");
        }
        
          //printf("trenutno uwb %d\n",xTaskGetTickCount());
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(150)); 

        //  printf("Tick time current %d",xLastWakeTime);
        // printf("Tick time xFrequency %d",pdMS_TO_TICKS(1000)); //1 ms = 100 Ticks
    }
}
void calculateIfLaunching()
{

    TickType_t xLastWakeTime = xTaskGetTickCount();
    float distance;
    const int N = 10;
    int sequenceOfCriticals = N; // N=10 uzastopnih
    while (true)
    {
        if (xQueueReceive(xMissileDist, &distance, portMAX_DELAY) == pdPASS)
        {
            
          //printf("trenutno calculateiflaunching %d\n",xTaskGetTickCount());
            if (distance < 5.0)
            {
                if (sequenceOfCriticals == 0)
                {
                    // proracunavanje na osnovu distance
                    float missileParams = distance * 2;
                    printf("10 uzastopnih detekcija nadolazeceg projektila!\n");

                    if (xQueueOverwrite(xFire, &missileParams) != pdPASS)
                    {
                        printf("Failed to send missile parameters to queue.\n");
                    }else{
                   
                    }
                    sequenceOfCriticals = N;
                }
                else
                {

                    sequenceOfCriticals--;
                }
            }
            else
            {
                sequenceOfCriticals = N;
            }
            
          //printf("trenutno calculateiflaunching posle %d\n",xTaskGetTickCount());
        }
    }
}

void fire_missile()
{
    float missileDirections;
  //  TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
           if (xQueueReceive(xFire, &missileDirections, portMAX_DELAY) == pdPASS)
            {
                
            
         // printf("trenutno fire  %d\n",xTaskGetTickCount());
                if (!criticalReportedWl)
                {    // ispaljujem projektil
                    for (int i = 3; i > 0; i--)
                    {
                        printf("ISPALJUJE SE PROJEKTIL!!! ... %d\n", i);
                        vTaskDelay( pdMS_TO_TICKS(800)); // 300ms
                    }                
                    vTaskDelay(pdMS_TO_TICKS(500)); 
                    xSemaphoreGive(xCheckTargetSem);
                    xSemaphoreTake(xLaunchCompleteSem, portMAX_DELAY);
                }
                else
                { 

                  // lcd_clear();
                   // lcd_print(0,"error not enough");
                   // lcd_print(1,"     fuel   ");
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    //lcd_clear();
                    //lcd_print_status();
          
                    printf("Zbog nedovoljnog goriva ne moze se ispaliti projektil.\n");
                   // 
                }
                
         // printf("trenutno fire posle  %d\n",xTaskGetTickCount());
                 
            }
    }
}
void check_target()
{
    float distance;

    while (true)
    {
        while (true) {
        if (xSemaphoreTake(xCheckTargetSem, portMAX_DELAY) == pdPASS) {
            
            printf("Provera pretnje nakon ispaljivanja projektila...\n");

           /*if (xQueueReceive(xMissileDist, &distance, portMAX_DELAY) == pdPASS) {
       
                if (distance < 5.0) {
                    printf("ALARM! Cilj je još uvek prisutan!\n");
                } else {
                    printf("Cilj je neutralisan.\n");
                }
            }*/
            

            vTaskDelay(pdMS_TO_TICKS(250)); 

            if (xQueueReceive(xMissileDist, &distance, portMAX_DELAY) == pdPASS) {

              //     printf("trenutno checkTarget  %d\n",xTaskGetTickCount());
                if (distance < 5.0) {
                    printf("ALARM:CRITICAL Cilj je još uvek prisutan!!!!\n");
                } else {
                    printf("Cilj je neutralisan.\n");
                }
                
        //  printf("trenutno checkTarget posle  %d\n",xTaskGetTickCount());
            }

            xSemaphoreGive(xLaunchCompleteSem);
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

    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    lcd_init();

    adc_init();
    adc_set_temp_sensor_enabled(true);

    xQueue = xQueueCreate(1, sizeof(float));
    xWaterQueue = xQueueCreate(1, sizeof(float));
    adcMutex = xSemaphoreCreateMutex();
    blinkingMutex = xSemaphoreCreateMutex();
    xBlinkQueue = xQueueCreate(1, sizeof(int));
    xMissileDist = xQueueCreate(1, sizeof(float));

    xLaunchCompleteSem = xSemaphoreCreateBinary();
    xCheckTargetSem = xSemaphoreCreateBinary();

   // xFireSem = xSemaphoreCreateBinary();


    xFire = xQueueCreate(1, sizeof(float));
    xLcdSem = xSemaphoreCreateBinary();
    xSemaphoreGive(xLcdSem); 

    xTaskCreate(uwb_task, "UWBSerialTask", 256, NULL, 4, NULL);
    xTaskCreate(calculateIfLaunching, "CalculateSerialTask", 256, NULL, 4, NULL);
    xTaskCreate(fire_missile, "FireMissileTask", 256, NULL, 6, NULL);


    xTaskCreate(check_target, "CheckTarget", configMINIMAL_STACK_SIZE, NULL, 7, NULL);

    xTaskCreate(temp_task, "TEMP_Task", 256, NULL, 3, NULL);
    xTaskCreate(print_to_serial_monitor_temp, "TEMP_USB_Task", 256, NULL, 3, NULL);

    //  xTaskCreate(watchdog_task, "WDT_Task", 256, NULL, 1, NULL);

    xTaskCreate(water_level_task, "WL_Task", 256, NULL, 3, NULL);
    xTaskCreate(print_to_serial_monitor_wl, "WL_USB_Task", 256, NULL, 3, NULL);

    xTaskCreate(blink_task, "BLINK_Task", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1)
    {
    };
}