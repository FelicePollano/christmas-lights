#include <stdio.h>
#include <string.h>
#include "esp_random.h"

#define SPARK_COUNT 50

typedef struct DL
{
    int direction;
    uint8_t R;
    uint8_t G;
    uint8_t B;
    int32_t pos;
}DL_t;

static DL_t leds[SPARK_COUNT];

int twirls_init(uint8_t *data,int count){
    for(int i=0;i<SPARK_COUNT;++i){
        leds[i].direction = esp_random()%2 == 0 ? 1 : -1;
        uint32_t r=esp_random();
        uint32_t r2=esp_random();
        leds[i].G = r%2==0?(r2%2==0?255:128):0;//G
        r=esp_random();
        r2=esp_random();
        leds[i].R = r%2==0?(r2%2==0?255:128):0;//R
        r=esp_random();
        r2=esp_random();
        leds[i].B = r%2==0?(r2%2==0?255:128):0; //B
        leds[i].pos=(uint32_t)(esp_random()/(float)UINT32_MAX*count+1);
    }
    return 0; //0 ms wait time
} 
void twirls_clean(uint8_t *data ,int count){
    //nothing to do
}
void twirls_tweak(char *param ,char* val){
}
int twirls_run(uint8_t *data,int count){
    memset(data,0,3*count);
    for(int i=0;i<SPARK_COUNT;++i){
        if(data[leds[i].pos*3+0]==0){
            data[leds[i].pos*3+0]=leds[i].G;
            data[leds[i].pos*3+1]=leds[i].R;
            data[leds[i].pos*3+2]=leds[i].B;
        }else{
            data[leds[i].pos*3+0]=255;
            data[leds[i].pos*3+1]=255;
            data[leds[i].pos*3+2]=255;
        }
        leds[i].pos+=leds[i].direction;
        if(leds[i].pos<0||leds[i].pos>count){
            leds[i].direction = esp_random()%2 == 0 ? 1 : -1;
            uint32_t r=esp_random();
            uint32_t r2=esp_random();
            leds[i].G = r%2==0?(r2%2==0?255:128):0;//G
            r=esp_random();
            r2=esp_random();
            leds[i].R = r%2==0?(r2%2==0?255:128):0;//R
            r=esp_random();
            r2=esp_random();
            leds[i].B = r%2==0?(r2%2==0?255:128):0; //B
            leds[i].pos=(uint32_t)(esp_random()/(float)UINT32_MAX*count+1);
        }
    }
    return 15;
}