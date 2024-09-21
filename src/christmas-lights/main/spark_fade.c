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

void init_one(DL_t *led, int count, float taumax){
    led->life=0;
    led->tau=esp_random()/(float)UINT32_MAX*taumax;
    uint32_t r=esp_random();
    uint32_t r2=esp_random();
    led->G = r%2==0?(r2%2==0?255:128):0;//G
    r=esp_random();
    r2=esp_random();
    led->R = r%2==0?(r2%2==0?255:128):0;//R
    r=esp_random();
    r2=esp_random();
    led->B = r%2==0?(r2%2==0?255:128):0; //B
    led->pos=(uint32_t)(esp_random()/(float)UINT32_MAX*count+1);
}

int spark_fade_init(uint8_t *data,int count){
    for(int i=0;i<SPARK_SIZE;++i){
        init_one(&leds[i],count,100);
    }
    memset(data,0,count*3);
    return 0;
}
int spark_fade_init_inv(uint8_t *data,int count){
    for(int i=0;i<SPARK_SIZE;++i){
        init_one(&leds[i],count,100);
    }
    memset(data,0,count*3);
    return 0;
}
int spark_fade_run(uint8_t *data,int count){
    for(int i=0;i<SPARK_SIZE;++i){
            
            float ex = exp(-leds[i].life/leds[i].tau);
            data[leds[i].pos*3+0]=(int) ((float)leds[i].G*ex);
            data[leds[i].pos*3+1]=(int) ((float)leds[i].R*ex);
            data[leds[i].pos*3+2]=(int) ((float)leds[i].B*ex);
            leds[i].life++;

            if(data[leds[i].pos*3+0]==0 && data[leds[i].pos*3+1]==0 && data[leds[i].pos*3+2]==0 ){
                init_one(&leds[i],count,100);
            }
        }

    return 1;
}
int spark_fade_run_inv(uint8_t *data,int count){
    for(int i=0;i<SPARK_SIZE;++i){

            float ex=1.0f-exp(-leds[i].life/leds[i].tau);
            data[leds[i].pos*3+0]=(int) ((float)leds[i].G*ex);
            data[leds[i].pos*3+1]=(int) ((float)leds[i].R*ex);
            data[leds[i].pos*3+2]=(int) ((float)leds[i].B*ex);
            leds[i].life++;
            
            if( ex >= .99 ){
                data[leds[i].pos*3+0]=0;
                data[leds[i].pos*3+1]=0;
                data[leds[i].pos*3+2]=0;
                init_one(&leds[i],count,100);
            }
        }

    return 1;
}
void spark_fade_clean(uint8_t *data ,int count){

}
void spark_fade_tweak(char *param ,char* val){

}