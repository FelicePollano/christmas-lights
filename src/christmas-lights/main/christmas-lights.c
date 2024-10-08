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
#include "alt.h"

#define LED1 16
#define LEDCOUNT 600



#define LED_TASK_PRIORITY 2
#define DIO_TASK_PRIORITY 0

static uint8_t led_data[ LEDCOUNT * 3];
static Mode_t modes[]={
    {.init=spark_0init,.run=spark_0run,.clean=spark_0clean} //simple random sparks of multicolor lights 
    ,{.init=twirls_init,.run=twirls_run,.clean=twirls_clean} //random sparks of multicolor lights twirling 
    ,{.init=twirls_init,.run=twirls_run_bounch,.clean=twirls_clean} //random sparks of multicolor lights twirling and bounching at the end 
    ,{.init=spark_fade_init,.run=spark_fade_run,.clean=spark_fade_clean} //fading sparks
    ,{.init=spark_fade_init_inv,.run=spark_fade_run_inv,.clean=spark_fade_clean} //un-fading sparks 
    ,{.init=alt_init,.run=alt_run,.clean=alt_clean} //alternate up down 
    ,{.init=alt_init,.run=alt_run2,.clean=alt_clean} //alternate  down 
    ,{.init=alt_init,.run=alt_run3,.clean=alt_clean} //alternate  up/down with some random
};
static  RGB_t (*cpickers[])()={
    hi_cpicker,
    rnd_cpicker,
    c1_cpicker, 
    c2_cpicker,
    c3_cpicker,
    c4_cpicker,
    c5_cpicker,
    c6_cpicker,
    c7_cpicker,
    c8_cpicker,
    c9_cpicker,
    c10_cpicker,
    c11_cpicker,
    c12_cpicker,
    c13_cpicker,
    c14_cpicker,
    c15_cpicker
};
void app_main(void)
{
    //wifi_connect(NULL); /* not reliable on single core board when using RMT for led driving :(
    Context_t context={
        .led_GPIO=LED1,
        .led_data = led_data,
        .led_count = LEDCOUNT,
        .mode = &modes[0],
        .modes = modes,
        .modes_count = sizeof(modes)/sizeof(modes[0]),
        .cpickers_count = sizeof(cpickers)/sizeof(cpickers[0]),
        .cpicker=cpickers[0],
        .cpickers=cpickers
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
