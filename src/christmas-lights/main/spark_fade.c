#include <stdio.h>
#include <string.h>
#include "esp_random.h"
#include <float.h>
#include <math.h>
#include "cl.h"

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

void init_one(DL_t *led, int count, float taumax,RGB_t (*cpicker)()){
    led->life=0;
    led->tau=esp_random()/(float)UINT32_MAX*taumax;
    RGB_t color = cpicker();
    led->G = color.G;//G
    led->R = color.R;//R
    led->B = color.B; //B
    led->pos=(uint32_t)(esp_random()/(float)UINT32_MAX*count+1);
}

int spark_fade_init(uint8_t *data,int count,RGB_t (*cpicker)()){
    for(int i=0;i<SPARK_SIZE;++i){
        init_one(&leds[i],count,100,cpicker);
    }
    memset(data,0,count*3);
    return 0;
}
int spark_fade_init_inv(uint8_t *data,int count,RGB_t (*cpicker)()){
    for(int i=0;i<SPARK_SIZE;++i){
        init_one(&leds[i],count,100,cpicker);
    }
    memset(data,0,count*3);
    return 0;
}
int spark_fade_run(uint8_t *data,int count,RGB_t (*cpicker)()){
    for(int i=0;i<SPARK_SIZE;++i){
            
            float ex = exp(-leds[i].life/leds[i].tau);
            data[leds[i].pos*3+0]=(int) ((float)leds[i].G*ex);
            data[leds[i].pos*3+1]=(int) ((float)leds[i].R*ex);
            data[leds[i].pos*3+2]=(int) ((float)leds[i].B*ex);
            leds[i].life++;

            if(data[leds[i].pos*3+0]==0 && data[leds[i].pos*3+1]==0 && data[leds[i].pos*3+2]==0 ){
                init_one(&leds[i],count,100,cpicker);
            }
        }

    return 1;
}
int spark_fade_run_inv(uint8_t *data,int count,RGB_t (*cpicker)()){
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
                init_one(&leds[i],count,100,cpicker);
            }
        }

    return 1;
}
void spark_fade_clean(uint8_t *data ,int count){

}
