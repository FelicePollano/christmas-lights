#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"
#include "cl.h"

#define TICK_RESOLUTION 10000000

#define SPARK_SIZE 30

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
    int k = 0;
    while(1){
        int spark[SPARK_SIZE];
        for(int i=0;i<SPARK_SIZE;++i){
            spark[i] = (uint32_t)(esp_random()/(float)UINT32_MAX*context->led_count);
        }
        for(int i=0;i<context->led_count;i++){
            bool b = false;
            for(int j=0;j<SPARK_SIZE;j++){
                if(i==spark[j]){
                    b=true;
                }
            }
            if(b){
                uint32_t r=esp_random();
                uint32_t r2=esp_random();
                context->led_data[i*3+0] = r%2==0?(r2%2==0?255:128):0;//G
                r=esp_random();
                r2=esp_random();
                context->led_data[i*3+1] = r%2==0?(r2%2==0?255:128):0;//R
                r=esp_random();
                r2=esp_random();
                context->led_data[i*3+2] = r%2==0?(r2%2==0?255:128):0; //B
                //esp_fill_random(led_data+i*3,3);
            }else{
                context->led_data[i*3+0] = 0;
                context->led_data[i*3+1] = 0;
                context->led_data[i*3+2] = 0;
            }
        }
        k++;
        if(k==context->led_count){
            k=0;
        }
        ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, context->led_data, context->led_count*3, &tx_config));
        ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
        vTaskDelay(10);
        
    }
}