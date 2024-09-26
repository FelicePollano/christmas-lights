#include <stdio.h>
#include <string.h>
#include "esp_random.h"
#include "cl.h"

#define SPARK_COUNT 50

typedef struct DL
{
    int direction;
    uint8_t R;
    uint8_t G;
    uint8_t B;
    int32_t pos;
    uint8_t bounches;
}DL_t;

static DL_t leds[SPARK_COUNT];

int twirls_init(uint8_t *data,int count,RGB_t (*cpicker)()){
    for(int i=0;i<SPARK_COUNT;++i){
        RGB_t color = cpicker();
        leds[i].direction = esp_random()%2 == 0 ? 1 : -1;
        leds[i].G = color.G;//G
        leds[i].R = color.R;//R
        leds[i].B = color.B; //B
        leds[i].pos=(uint32_t)(esp_random()/(float)UINT32_MAX*count+1);
        leds[i].bounches=0;
    }
    return 0; //0 ms wait time
} 
void twirls_clean(uint8_t *data ,int count){
    //nothing to do
}

int twirls_run(uint8_t *data,int count,RGB_t (*cpicker)()){
    memset(data,0,3*count);
    for(int i=0;i<SPARK_COUNT;++i){
        
        data[leds[i].pos*3+0]=leds[i].G;
        data[leds[i].pos*3+1]=leds[i].R;
        data[leds[i].pos*3+2]=leds[i].B;
       
        leds[i].pos+=leds[i].direction;
        if(leds[i].pos<0||leds[i].pos>count){
            RGB_t color = cpicker();
            leds[i].direction = esp_random()%2 == 0 ? 1 : -1;
            leds[i].G = color.G;//G
            leds[i].R = color.R;//R
            leds[i].B = color.B; //B
            leds[i].pos=(uint32_t)(esp_random()/(float)UINT32_MAX*count+1);
        }
    }
    return 15;
}
int twirls_run_bounch(uint8_t *data,int count,RGB_t (*cpicker)()){
    memset(data,0,3*count);
    for(int i=0;i<SPARK_COUNT;++i){
        
        data[leds[i].pos*3+0]=leds[i].G;
        data[leds[i].pos*3+1]=leds[i].R;
        data[leds[i].pos*3+2]=leds[i].B;
       
        leds[i].pos+=leds[i].direction;
        if(leds[i].pos<0||leds[i].pos>count){
            leds[i].bounches++;
            if( leds[i].bounches == 3 ){
                RGB_t color = cpicker();
                leds[i].G = color.G;//G
                leds[i].R = color.R;//R
                leds[i].B = color.B; //B
                leds[i].bounches = 0;
            }
            leds[i].direction = -leds[i].direction;
        }
    }
    return 40;
}