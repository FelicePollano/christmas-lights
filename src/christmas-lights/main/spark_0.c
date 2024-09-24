#include <stdio.h>
#include "esp_random.h"
#include "cl.h"

#define SPARK_SIZE 30

static int spark[SPARK_SIZE];

int spark_0init(uint8_t *data,int count,RGB_t (*cpicker)()){
    //nothing to do
    return 0; //0 ms wait time
} 
void spark_0clean(uint8_t *data ,int count){
    //nothing to do
}

int spark_0run(uint8_t *data,int count,RGB_t (*cpicker)()){

    for(int i=0;i<SPARK_SIZE;++i){
            spark[i] = (uint32_t)(esp_random()/(float)UINT32_MAX*count);
    }
    for(int i=0;i<count;i++){
        uint8_t b = 0;
        for(int j=0;j<SPARK_SIZE;j++){
            if(i==spark[j]){
                b=1;
            }
        }
        if(b){
            RGB_t c = cpicker();
            data[i*3+0] = c.G;//G
            data[i*3+1] = c.R;//R
            data[i*3+2] = c.B; //B
            //esp_fill_random(led_data+i*3,3);
        }else{
            data[i*3+0] = 0;
            data[i*3+1] = 0;
            data[i*3+2] = 0;
        }
    }
    return 100; //100 ms wait time
} 
