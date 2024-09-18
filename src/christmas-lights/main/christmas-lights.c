#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "esp_random.h"

#include "cl.h"
#include "spark_0.h"
#include "twirl_sparks.h"

#define LED1 16
#define LEDCOUNT 600



#define LED_TASK_PRIORITY 2

static uint8_t led_data[ LEDCOUNT * 3];
static Mode_t modes[]={
    {.init=spark_0init,.run=spark_0run,.clean=spark_0clean,.tweak=spark_0tweak} //simple random sparks of multicolor lights 
    ,{.init=twirls_init,.run=twirls_run,.clean=twirls_clean,.tweak=twirls_tweak} //random sparks of multicolor lights twirling 
};
void app_main(void)
{
    Context_t context={
        .led_GPIO=LED1,
        .led_data = led_data,
        .led_count = LEDCOUNT,
        .mode = &modes[1]
    };
    gpio_reset_pin(LED1);
    //gpio_set_direction(LED1,GPIO_MODE_OUTPUT);
    char* mainTask = pcTaskGetName(NULL);
    // led driver
    xTaskCreate(led_driver,"led_driver",2048,&context,LED_TASK_PRIORITY, NULL);
    ESP_LOGI(mainTask,"Initialization done!\n");
    vTaskDelete(NULL);
    
}
