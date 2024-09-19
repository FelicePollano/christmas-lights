#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "cl.h"

//according to docs this pin supports internall pull-up resistor ( and pull up is even the default at wakeup )
//I want mode chang button to act when grouded, in order to avoid having 3v3 volt extra wire to handle positive logic 
#define MODECHANGE 5 
// handle digital I/O
void dio(void* ctx){
    Context_t *context=(Context_t*)ctx;
    TickType_t  tref=0;
    int changed=0;
    int current_mode=0;
    ESP_ERROR_CHECK(gpio_set_direction(MODECHANGE,GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(MODECHANGE,GPIO_PULLUP_ONLY));
    while(1){
        int mc = gpio_get_level(MODECHANGE);
        if(mc){
            tref=xTaskGetTickCount();
            changed=0;
        }else{
           // if((xTaskGetTickCount()-tref)/portTICK_PERIOD_MS>100){ //avoid glitches
                if(!changed){
                    changed=1;
                    if(current_mode+1<context->modes_count){
                        current_mode++;
                    }else{
                        current_mode=0;
                    }
                    context->mode=&context->modes[current_mode];
                }
            //}
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}