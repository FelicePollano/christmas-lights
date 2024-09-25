#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "cl.h"

//according to the docs this pin supports internal pull-up resistor ( and pull up is even the default at wakeup )
//I want mode chang button to act when grounded, in order to avoid having 3v3 volt extra wire to handle positive logic 
#define MODENEXT 5 
#define CCHANGE 9
// handle digital I/O
void dio(void* ctx){
    Context_t *context=(Context_t*)ctx;
    int mchanged=0;
    int cchanged=0;
    int current_mode=0;
    int current_color=0;
    ESP_ERROR_CHECK(gpio_set_direction(MODENEXT,GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(MODENEXT,GPIO_PULLUP_ONLY));
     ESP_ERROR_CHECK(gpio_set_direction(CCHANGE,GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(CCHANGE,GPIO_PULLUP_ONLY));
    while(1){
        int pin = gpio_get_level(MODENEXT);
        if(pin){
            mchanged=0;
        }else{
            if(!mchanged){
                 mchanged=1;
                 if(current_mode+1<context->modes_count){
                    current_mode++;
                 }else{
                    current_mode=0;
                 }
                 context->mode=&context->modes[current_mode];
            }
        }
        pin = gpio_get_level(CCHANGE);
        if(pin){
            cchanged=0;
        }else{
            if(!cchanged){
                 cchanged=1;
                 if(current_color+1<context->cpickers_count){
                    current_color++;
                 }else{
                    current_color=0;
                 }
                 context->cpicker=context->cpickers[current_color];
            }
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}