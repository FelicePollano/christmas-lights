#include <stdio.h>
#include "cl.h"
#include "esp_random.h"

RGB_t hi_cpicker(){ //random color picker, high intensity only
     uint32_t r=esp_random();
     uint32_t r2=esp_random();
     RGB_t color;
     color.G= r%2==0?(r2%2==0?255:128):0;//G
     r=esp_random();
     r2=esp_random();
     color.R = r%2==0?(r2%2==0?255:128):0;//R
     r=esp_random();
     r2=esp_random();
     color.B = r%2==0?(r2%2==0?255:128):0; //B
     return color;
}

RGB_t rnd_cpicker(){ //totally random color picker
   
    RGB_t color;
    color.G= (uint8_t)(esp_random()/(float)UINT32_MAX*255);
    color.R = (uint8_t)(esp_random()/(float)UINT32_MAX*255);
    color.B = (uint8_t)(esp_random()/(float)UINT32_MAX*255);
    return color;
}