#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "cl.h"
#define IR_PIN 9
#define IR_RESOLUTION_HZ     10000000 // 1MHz resolution warn conflict with WS2812 tx?
#define TAG "IR"


/*
decoded data all ending in ff00
a35c a25d be41 bf40
a758 a659 ba45 bb44
ab54 aa55 b649 b748
af50 ae51 b24d b34c
e31c e21d e11e e01f
e718 e619 e51a e41b
eb14 ea15 e916 e817
ef10 ee11 ed12 ec13
f30c f20d f10e f00f
f708 f609 f50a f40b
fb04 fa05 f906 f807
*/

static uint32_t decode(rmt_rx_done_event_data_t rx_data){
    if(rx_data.num_symbols!=34){
        return 0; //invalid frame
    }
    uint32_t res=0;
    uint32_t bit=1;
    for(int i=1;i<34;++i){ //first and last symbols seems to be markers
        if(rx_data.received_symbols[i].duration1>=1000){
            res|=bit;
        }
        bit<<=1;
    }
    return res;
}

static bool rx_done_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;
}

void ir_receiver(void *c){
    int current_mode = 0;
    Context_t *context = (Context_t*)c;
    rmt_rx_channel_config_t rx_channel_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = IR_RESOLUTION_HZ,
        .mem_block_symbols = 64, 
        .gpio_num = IR_PIN,
    };
     rmt_channel_handle_t rx_channel = NULL;
    ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_channel_cfg, &rx_channel));

    ESP_LOGI(TAG, "register RX done callback");
    QueueHandle_t receive_queue = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));
    assert(receive_queue);
    rmt_rx_event_callbacks_t cbs = {
        .on_recv_done = rx_done_callback,
    };
    ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_channel, &cbs, receive_queue));

    rmt_receive_config_t receive_config = {
        .signal_range_min_ns = 1250,     // the shortest duration for  1250ns 
        .signal_range_max_ns = 12000000, // the longest duration for  signal  12000000ns 
    };
    ESP_ERROR_CHECK(rmt_enable(rx_channel));

    // save the received RMT symbols
    rmt_symbol_word_t raw_symbols[64]; // 64 symbols should be sufficient 
    
    // ready to receive
    ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));
    
    rmt_rx_done_event_data_t rx_data;
    while (1) {
        // wait for RX done signal
        if (xQueueReceive(receive_queue, &rx_data, pdMS_TO_TICKS(1000)) == pdPASS) {
            ESP_LOGI(TAG,"Received %d symbols\n",rx_data.num_symbols);
            /*
            for(int i=0;i<rx_data.num_symbols;++i){
                ESP_EARLY_LOGI(TAG,"Duration0 %d level0 %d \t Duration1 %d Level1 %d \n"
                ,rx_data.received_symbols[i].duration0
                ,rx_data.received_symbols[i].level0
                ,rx_data.received_symbols[i].duration1
                ,rx_data.received_symbols[i].level1
                );
            }*/
            uint32_t code = decode(rx_data);
             if(current_mode+1<context->modes_count){
                        current_mode++;
                    }else{
                        current_mode=0;
                    }
                    context->mode=&context->modes[current_mode];
            
            switch(code){
                case 0xfb04ff00:
                    if(current_mode+1<context->modes_count){
                        current_mode++;
                    }else{
                        current_mode=0;
                    }
                    context->mode=&context->modes[current_mode];
                    break;
                case 0xfa05ff00:
                    if(current_mode+1<context->modes_count){
                        current_mode++;
                    }else{
                        current_mode=0;
                    }
                    context->mode=&context->modes[current_mode];
                    break;
            }
            vTaskDelay(100/portTICK_PERIOD_MS );
            ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));
        } 
    }
}