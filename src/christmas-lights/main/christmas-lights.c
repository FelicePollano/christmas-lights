#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "esp_random.h"
#include "cl.h"

#define LED1 16
#define LEDCOUNT 600



#define LED_TASK_PRIORITY 2

static uint8_t led_data[ LEDCOUNT * 3];
void app_main(void)
{
    Context_t context={
        .led_GPIO=LED1,
        .led_data = led_data,
        .led_count = LEDCOUNT
    };
    gpio_reset_pin(LED1);
    //gpio_set_direction(LED1,GPIO_MODE_OUTPUT);
    char* mainTask = pcTaskGetName(NULL);
    ESP_LOGI(mainTask,"Hello world!\n");
    // led driver
    xTaskCreate(led_driver,"led_driver",2048,&context,LED_TASK_PRIORITY, NULL);
    vTaskDelete(NULL);
}
