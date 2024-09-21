#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "bootloader_random.h"

#include "cl.h"
#include "spark_0.h"
#include "twirl_sparks.h"
#include "spark_fade.h"

#define LED1 16
#define LEDCOUNT 600



#define LED_TASK_PRIORITY 2
#define DIO_TASK_PRIORITY 0

static uint8_t led_data[ LEDCOUNT * 3];
static Mode_t modes[]={
    {.init=spark_0init,.run=spark_0run,.clean=spark_0clean,.tweak=spark_0tweak} //simple random sparks of multicolor lights 
    ,{.init=twirls_init,.run=twirls_run,.clean=twirls_clean,.tweak=twirls_tweak} //random sparks of multicolor lights twirling 
    ,{.init=spark_fade_init,.run=spark_fade_run,.clean=spark_fade_clean,.tweak=spark_fade_tweak} //fading sparks
    ,{.init=spark_fade_init,.run=spark_fade_run_inv,.clean=spark_fade_clean,.tweak=spark_fade_tweak} //un-fading sparks 
};
void app_main(void)
{
    Context_t context={
        .led_GPIO=LED1,
        .led_data = led_data,
        .led_count = LEDCOUNT,
        .mode = &modes[0],
        .modes = modes,
        .modes_count = sizeof(modes)/sizeof(modes[0])
    };
    gpio_reset_pin(LED1);
    bootloader_random_enable();//remove when WIFI is enabled
    //gpio_set_direction(LED1,GPIO_MODE_OUTPUT);
    char* mainTask = pcTaskGetName(NULL);
    // led driver
    xTaskCreate(led_driver,"led_driver",2048,&context,LED_TASK_PRIORITY, NULL);
    // digital I/O
    xTaskCreate(dio,"dio",2048,&context,DIO_TASK_PRIORITY, NULL);
    ESP_LOGI(mainTask,"Initialization done!\n");
    vTaskDelete(NULL);
    
}
