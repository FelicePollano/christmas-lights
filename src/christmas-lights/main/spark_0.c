#include <stdio.h>
#include "esp_random.h"
#define SPARK_SIZE 30

static int spark[SPARK_SIZE];

int spark_0init(uint8_t *data,int count){
    //nothing to do
    return 0; //0 ms wait time
} 
void spark_0clean(uint8_t *data ,int count){
    //nothing to do
}
void spark_0tweak(char *param ,char* val){
}
int spark_0run(uint8_t *data,int count){

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
            uint32_t r=esp_random();
            uint32_t r2=esp_random();
            data[i*3+0] = r%2==0?(r2%2==0?255:128):0;//G
            r=esp_random();
            r2=esp_random();
            data[i*3+1] = r%2==0?(r2%2==0?255:128):0;//R
            r=esp_random();
            r2=esp_random();
            data[i*3+2] = r%2==0?(r2%2==0?255:128):0; //B
            //esp_fill_random(led_data+i*3,3);
        }else{
            data[i*3+0] = 0;
            data[i*3+1] = 0;
            data[i*3+2] = 0;
        }
    }
    return 100; //100 ms wait time
} 
