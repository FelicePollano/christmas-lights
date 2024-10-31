
#include <stdio.h>
#include <string.h>
#include "esp_random.h"
#include "cl.h"

int umpa=0;
int dir = 1;
int delta = 6; //empty space between lightet leds

int alt_init(uint8_t *data,int count,RGB_t (*cpicker)()){
    return 0;
}
int alt_run(uint8_t *data,int count,RGB_t (*cpicker)()){
    memset(data,0,count*3);
        
    for(int i=0;i<count*3;i+=3){
        if(((i/3)%delta)==umpa){
            RGB_t c = cpicker();
            data[i+0]= c.G;
            data[i+1]= c.R;
            data[i+2]= c.B;
        }
    }
    umpa+=dir;
    if(umpa==delta){
        umpa--;
        dir*=-1;
    }
    if(umpa == -1){
        umpa++;
        dir*=-1;
    }
    return 100;
}
int alt_run2(uint8_t *data,int count,RGB_t (*cpicker)()){
    memset(data,0,count*3);
        
    for(int i=0;i<count*3;i+=3){
        if(((i/3)%delta)==umpa){
            RGB_t c = cpicker();
            data[i+0]= c.G;
            data[i+1]= c.R;
            data[i+2]= c.B;
        }
    }
    umpa++;

    if(umpa==delta){
        umpa=0;
    }
    
    return 100;
}
int alt_run3(uint8_t *data,int count,RGB_t (*cpicker)()){
    memset(data,0,count*3);
        
    for(int i=0;i<count*3;i+=3){
        if(((i/3)%delta)==umpa){
            RGB_t c = cpicker();
            data[i+0]= c.G;
            data[i+1]= c.R;
            data[i+2]= c.B;
        }
    }
    int x =(uint8_t)(esp_random()/(float)UINT32_MAX*10);
    if(x==5){
        dir*=-1;
    }
    umpa+=dir;
    if(umpa==delta){
        umpa--;
        dir*=-1;
    }
    if(umpa == -1){
        umpa++;
        dir*=-1;
    }
    return 100;
}
void alt_clean(uint8_t *data ,int count){
}