#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "cl.h"

#define TICK_RESOLUTION 10000000

/// this code, for the RMT part is based on
/// Esp-Idf official examples https://github.com/espressif/esp-idf/tree/master/examples/peripherals/rmt/led_strip/main
/// is however a sensible simplified version, expecially for the encoder part.


void led_driver(void *ctx){
    Context_t *context=(Context_t*)ctx;
    rmt_channel_handle_t led_chan = NULL;
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
        .gpio_num = context->led_GPIO,
        .mem_block_symbols = 64, // increase the block size can make the LED less flickering
        .resolution_hz = TICK_RESOLUTION,
        .trans_queue_depth = 4, // set the number of transactions that can be pending in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));
    rmt_encoder_handle_t led_encoder;
    
    rmt_bytes_encoder_config_t encoder_config={
        .bit0={
            .duration0=3,
            .level0=1,
            .duration1=9,
            .level1=0,
        },
        .bit1={
            .duration0=9,
            .level0=1,
            .duration1=3,
            .level1=0,
        },
        .flags.msb_first = 1
    };
    
    ESP_ERROR_CHECK(rmt_new_bytes_encoder(&encoder_config, &led_encoder));
    ESP_ERROR_CHECK(rmt_enable(led_chan));
    
    rmt_transmit_config_t tx_config = {
        .loop_count = 0, // no transfer loop
        .flags={
            .eot_level=0
        }
    };
    Mode_t *current_mode = NULL;
    int delay;
    while(1){
        if(current_mode!=context->mode){
            if(current_mode){
                current_mode->clean(context->led_data,context->led_count);
            }
            current_mode = context->mode;
            delay = context->mode->init(context->led_data,context->led_count);
        }else{
            delay = context->mode->run(context->led_data,context->led_count);
        }
        ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, context->led_data, context->led_count*3, &tx_config));
        ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
        vTaskDelay(delay/portTICK_PERIOD_MS);
    }
}