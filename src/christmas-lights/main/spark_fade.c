#include <stdio.h>
#include <string.h>
#include "esp_random.h"
#include <float.h>
#include <math.h>

#define SPARK_SIZE 120

typedef struct DL
{
    int life;
    float tau;
    uint8_t R;
    uint8_t G;
    uint8_t B;
    int32_t pos;
}DL_t;


DL_t leds[SPARK_SIZE];

int spark_fade_init(uint8_t *data,int count){
    for(int i=0;i<SPARK_SIZE;++i){
        leds[i].life=0;
        leds[i].tau=esp_random()/(float)UINT32_MAX*100.0f;
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
    memset(data,0,count*3);
    return 0;
}
int spark_fade_run(uint8_t *data,int count){
    for(int i=0;i<SPARK_SIZE;++i){
            
            data[leds[i].pos*3+0]=(int) ((float)leds[i].G*exp(-leds[i].life/leds[i].tau));
            data[leds[i].pos*3+1]=(int) ((float)leds[i].R*exp(-leds[i].life/leds[i].tau));
            data[leds[i].pos*3+2]=(int) ((float)leds[i].B*exp(-leds[i].life/leds[i].tau));
            leds[i].life++;
            if(data[leds[i].pos*3+0]==0 && data[leds[i].pos*3+1]==0 && data[leds[i].pos*3+2]==0 ){
                leds[i].life=0;
                leds[i].tau=esp_random()/(float)UINT32_MAX*100.0f;
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

    return 1;
}
void spark_fade_clean(uint8_t *data ,int count){

}
void spark_fade_tweak(char *param ,char* val){

}